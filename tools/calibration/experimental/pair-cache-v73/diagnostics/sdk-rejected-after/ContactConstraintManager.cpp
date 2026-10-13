// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#include <Jolt/Jolt.h>
#include <cstdio>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>

#include <Jolt/Physics/Constraints/ContactConstraintManager.h>
#include <Jolt/Physics/Constraints/CalculateSolverSteps.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/PhysicsUpdateContext.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/IslandBuilder.h>
#include <Jolt/Physics/DeterminismLog.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/QuickSort.h>
#ifdef JPH_DEBUG_RENDERER
	#include <Jolt/Renderer/DebugRenderer.h>
#endif // JPH_DEBUG_RENDERER

JPH_NAMESPACE_BEGIN

using namespace literals;

#ifdef JPH_DEBUG_RENDERER
bool ContactConstraintManager::sDrawContactPoint = false;
bool ContactConstraintManager::sDrawSupportingFaces = false;
bool ContactConstraintManager::sDrawContactPointReduction = false;
bool ContactConstraintManager::sDrawContactManifolds = false;
#endif // JPH_DEBUG_RENDERER

//#define JPH_MANIFOLD_CACHE_DEBUG

////////////////////////////////////////////////////////////////////////////////////////////////////////
// ContactConstraintManager::WorldContactPoint
////////////////////////////////////////////////////////////////////////////////////////////////////////

void ContactConstraintManager::WorldContactPoint::CalculateNonPenetrationConstraintProperties(const Body &inBody1, float inInvMass1, float inInvInertiaScale1, const Body &inBody2, float inInvMass2, float inInvInertiaScale2, RVec3Arg inWorldSpacePosition1, RVec3Arg inWorldSpacePosition2, Vec3Arg inWorldSpaceNormal)
{
	// Calculate collision points relative to body
	RVec3 p = 0.5_r * (inWorldSpacePosition1 + inWorldSpacePosition2);
	Vec3 r1 = Vec3(p - inBody1.GetCenterOfMassPosition());
	Vec3 r2 = Vec3(p - inBody2.GetCenterOfMassPosition());

	mNonPenetrationConstraint.CalculateConstraintPropertiesWithMassOverride(inBody1, inInvMass1, inInvInertiaScale1, r1, inBody2, inInvMass2, inInvInertiaScale2, r2, inWorldSpaceNormal);
}

template <EMotionType Type1, EMotionType Type2>
JPH_INLINE void ContactConstraintManager::WorldContactPoint::TemplatedCalculateFrictionAndNonPenetrationConstraintProperties(float inDeltaTime, float inGravityDeltaTimeDotNormal, const Body &inBody1, const Body &inBody2, float inInvM1, float inInvM2, Mat44Arg inInvI1, Mat44Arg inInvI2, RVec3Arg inWorldSpacePosition1, RVec3Arg inWorldSpacePosition2, Vec3Arg inWorldSpaceNormal, Vec3Arg inWorldSpaceTangent1, Vec3Arg inWorldSpaceTangent2, const ContactSettings &inSettings, float inMinVelocityForRestitution)
{
	JPH_DET_LOG("TemplatedCalculateFrictionAndNonPenetrationConstraintProperties: p1: " << inWorldSpacePosition1 << " p2: " << inWorldSpacePosition2
		<< " normal: " << inWorldSpaceNormal << " tangent1: " << inWorldSpaceTangent1 << " tangent2: " << inWorldSpaceTangent2
		<< " restitution: " << inSettings.mCombinedRestitution << " friction: " << inSettings.mCombinedFriction << " minv: " << inMinVelocityForRestitution
		<< " surface_vel: " << inSettings.mRelativeLinearSurfaceVelocity << " surface_ang: " << inSettings.mRelativeAngularSurfaceVelocity);

	// Calculate collision points relative to body
	RVec3 p = 0.5_r * (inWorldSpacePosition1 + inWorldSpacePosition2);
	Vec3 r1 = Vec3(p - inBody1.GetCenterOfMassPosition());
	Vec3 r2 = Vec3(p - inBody2.GetCenterOfMassPosition());

	// The gravity is applied in the beginning of the time step. If we get here, there was a collision
	// at the beginning of the time step, so we've applied too much gravity. This means that our
	// calculated restitution can be too high, so when we apply restitution, we cancel the added
	// velocity due to gravity.
	float gravity_dt_dot_normal;

	// Calculate velocity of collision points
	Vec3 relative_velocity;
	if constexpr (Type1 != EMotionType::Static && Type2 != EMotionType::Static)
	{
		const MotionProperties *mp1 = inBody1.GetMotionPropertiesUnchecked();
		const MotionProperties *mp2 = inBody2.GetMotionPropertiesUnchecked();
		relative_velocity = mp2->GetPointVelocityCOM(r2) - mp1->GetPointVelocityCOM(r1);
		gravity_dt_dot_normal = inGravityDeltaTimeDotNormal * (mp2->GetGravityFactor() - mp1->GetGravityFactor());
	}
	else if constexpr (Type1 != EMotionType::Static)
	{
		const MotionProperties *mp1 = inBody1.GetMotionPropertiesUnchecked();
		relative_velocity = -mp1->GetPointVelocityCOM(r1);
		gravity_dt_dot_normal = inGravityDeltaTimeDotNormal * mp1->GetGravityFactor();
	}
	else if constexpr (Type2 != EMotionType::Static)
	{
		const MotionProperties *mp2 = inBody2.GetMotionPropertiesUnchecked();
		relative_velocity = mp2->GetPointVelocityCOM(r2);
		gravity_dt_dot_normal = inGravityDeltaTimeDotNormal * mp2->GetGravityFactor();
	}
	else
	{
		JPH_ASSERT(false); // Static vs static makes no sense
		relative_velocity = Vec3::sZero();
		gravity_dt_dot_normal = 0.0f;
	}
	float normal_velocity = relative_velocity.Dot(inWorldSpaceNormal);

	// How much the shapes are penetrating (> 0 if penetrating, < 0 if separated)
	float penetration = Vec3(inWorldSpacePosition1 - inWorldSpacePosition2).Dot(inWorldSpaceNormal) - inSettings.mAdditionalSeparation - mSourceSeparation;

	// If there is no penetration, this is a speculative contact and we will apply a bias to the contact constraint
	// so that the constraint becomes relative_velocity . contact normal > -penetration / delta_time
	// instead of relative_velocity . contact normal > 0
	// See: GDC 2013: "Physics for Game Programmers; Continuous Collision" - Erin Catto
	float speculative_contact_velocity_bias = max(0.0f, -penetration / inDeltaTime);

	// Determine if the velocity is big enough for restitution
	float normal_velocity_bias;
	if (inSettings.mCombinedRestitution > 0.0f && normal_velocity < -inMinVelocityForRestitution)
	{
		// We have a velocity that is big enough for restitution. This is where speculative contacts don't work
		// great as we have to decide now if we're going to apply the restitution or not. If the relative
		// velocity is big enough for a hit, we apply the restitution (in the end, due to other constraints,
		// the objects may actually not collide and we will have applied restitution incorrectly). Another
		// artifact that occurs because of this approximation is that the object will bounce from its current
		// position rather than from a position where it is touching the other object. This causes the object
		// to appear to move faster for 1 frame (the opposite of time stealing).
		if (normal_velocity < -speculative_contact_velocity_bias)
			normal_velocity_bias = inSettings.mCombinedRestitution * (normal_velocity - gravity_dt_dot_normal);
		else
			// In this case we have predicted that we don't hit the other object, but if we do (due to other constraints changing velocities)
			// the speculative contact will prevent penetration but will not apply restitution leading to another artifact.
			normal_velocity_bias = speculative_contact_velocity_bias;
	}
	else
	{
		// No restitution. We can safely apply our contact velocity bias.
		normal_velocity_bias = speculative_contact_velocity_bias;
	}

	if (inSettings.mUseSourceBlockPGS)
	{
		const bool bounce = inSettings.mCombinedRestitution > 0.0f && normal_velocity < -inMinVelocityForRestitution && normal_velocity < -speculative_contact_velocity_bias;
		mSourceUnbiasedVelocityBias = bounce? normal_velocity_bias : 0.8f * speculative_contact_velocity_bias;
		normal_velocity_bias = bounce? normal_velocity_bias : -0.8f * penetration / inDeltaTime;
	}
	const char *raw_axis_flag = std::getenv("RRR3D_EXPERIMENT_RAW_CONTACT_MASS");
	const bool raw_axis_mass = inSettings.mUseSourceBlockPGS && raw_axis_flag != nullptr && std::strcmp(raw_axis_flag, "1") == 0;
	const char *jacobian_flag = std::getenv("RRR3D_EXPERIMENT_CONTACT_JACOBIAN_MASS");
	const bool jacobian_mass = inSettings.mUseSourceBlockPGS && jacobian_flag != nullptr && std::strcmp(jacobian_flag, "1") == 0;
    const char *friction_flag = std::getenv("RRR3D_EXPERIMENT_NATIVE_SINGLE_AXIS_FRICTION");
    const bool native_friction = friction_flag && std::strcmp(friction_flag, "1") == 0
        && inSettings.mCombinedFriction == 0.0f && inSettings.mCombinedFriction2 > 0.0f
        && inSettings.mRelativeLinearSurfaceVelocity == Vec3::sZero()
        && inSettings.mRelativeAngularSurfaceVelocity == Vec3::sZero();
    bool native_axis_placeholder = false;
    if constexpr ((Type1 == EMotionType::Dynamic && Type2 == EMotionType::Static) || (Type1 == EMotionType::Static && Type2 == EMotionType::Dynamic)) {
        const char *pose_flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_NONUNIT_POSE_MATRIX");
        native_axis_placeholder=pose_flag && std::strcmp(pose_flag,"1")==0 && NativeContactArithmetic::Flags()==31
            && inSettings.mUseSourceBlockPGS && inSettings.mCombinedRestitution==0.0f
            && ((inSettings.mCombinedFriction==0.0f && inSettings.mCombinedFriction2<=0.0f) || native_friction);
    }
    // Only initialize Jolt's unused Jacobian placeholder with a unit axis.
    // Native Prepare below replaces its mass/bias and the force-first solver
    // uses the original normal and lever without normalization.
    const Vec3 generic_normal=native_axis_placeholder && !inWorldSpaceNormal.IsNormalized(1.0e-5f)?
        inWorldSpaceNormal.Normalized() : inWorldSpaceNormal;
	mNonPenetrationConstraint.TemplatedCalculateConstraintProperties<Type1, Type2>(inInvM1, inInvI1, r1, inInvM2, inInvI2, r2, generic_normal, normal_velocity_bias, raw_axis_mass, jacobian_mass);
    mNativeContact.mMask = 0;
    mNativeSingleAxisFriction = false;
    mPairStaticFriction1V71=inSettings.mNativeStaticFriction1V71;
    mPairStaticFriction2V71=inSettings.mNativeStaticFriction2V69;


    if constexpr ((Type1 == EMotionType::Dynamic && Type2 == EMotionType::Static) || (Type1 == EMotionType::Static && Type2 == EMotionType::Dynamic)) {
        const uint native_mask = NativeContactArithmetic::Flags();
        if (native_mask && inSettings.mUseSourceBlockPGS && inSettings.mCombinedRestitution == 0.0f && ((inSettings.mCombinedFriction == 0.0f && inSettings.mCombinedFriction2 <= 0.0f) || (native_friction && native_mask == 31))) {
            constexpr bool first = Type1 == EMotionType::Dynamic;
            const Mat44 native_inverse_inertia=inSettings.mNativeInverseInertiaActive && native_mask==31?
                (first?inSettings.mInvInertiaScale1:inSettings.mInvInertiaScale2)*inSettings.mNativeInverseInertia:(first?inInvI1:inInvI2);
            mNativeContact.Prepare(native_mask,first,mNativeCachedLeverActive?mNativeCachedLever:(first?r1:r2),native_inverse_inertia,first?inInvM1:inInvM2,
                inWorldSpaceNormal,mDiagnosticNativeError?mDiagnosticNativeConstraintError:(mDerivedNativeError?mDerivedNativeConstraintError:-penetration),inDeltaTime,mDiagnosticNativeError,mNonPenetrationConstraint);
            mNativeContact.mDerivedError=mDerivedNativeError&&!mDiagnosticNativeError;
            if (native_friction) {
                using A = NativeContactArithmetic;
                mNativeSingleAxisFriction = true;
                mNativeFrictionAxis = inSettings.mNativeSolverTangentActive?
                    Vec3(inSettings.mNativeSourceTangent2[0],inSettings.mNativeSourceTangent2[1],inSettings.mNativeSourceTangent2[2]):
                    A::Polar(first? -inWorldSpaceTangent2 : inWorldSpaceTangent2);
                mNativeFrictionAxis1 = inSettings.mNativeSolverTangentActive?
                    Vec3(inSettings.mNativeSourceTangent1[0],inSettings.mNativeSourceTangent1[1],inSettings.mNativeSourceTangent1[2]):
                    A::Polar(first? -inWorldSpaceTangent1 : inWorldSpaceTangent1);
                mNativeFrictionImpulse = Vec3::sZero();
                mNativeFrictionCoefficient = inSettings.mCombinedFriction2;
                mNativeStaticFrictionCoefficientV69 = inSettings.mNativeStaticFriction2V69>=0.0f?inSettings.mNativeStaticFriction2V69:mNativeFrictionCoefficient;
                const Vec3 response = mNativeContact.Tensor(A::Cross(mNativeContact.mR, mNativeFrictionAxis));
                mNativeFrictionMass = A::Div(1.0f, A::Add(mNativeContact.mInvMass,
                    A::DotXYZ(mNativeFrictionAxis, A::Cross(response, mNativeContact.mR))));
                const Vec3 response1=mNativeContact.Tensor(A::Cross(mNativeContact.mR,mNativeFrictionAxis1));
                mNativeFrictionMass1=A::Div(1.0f,A::Add(mNativeContact.mInvMass,
                    A::DotXYZ(mNativeFrictionAxis1,A::Cross(response1,mNativeContact.mR))));
            }

        }
    }


	// Calculate friction part
	if (inSettings.mCombinedFriction > 0.0f || inSettings.mCombinedFriction2 > 0.0f)
	{
		// Get surface velocity relative to tangents
		Vec3 ws_surface_velocity = inSettings.mRelativeLinearSurfaceVelocity + inSettings.mRelativeAngularSurfaceVelocity.Cross(r1);
		float surface_velocity1 = inWorldSpaceTangent1.Dot(ws_surface_velocity);
		float surface_velocity2 = inWorldSpaceTangent2.Dot(ws_surface_velocity);

        const Vec3 generic_tangent1=native_axis_placeholder && mNativeSingleAxisFriction && !inWorldSpaceTangent1.IsNormalized(1.0e-5f)? inWorldSpaceTangent1.Normalized() : inWorldSpaceTangent1;
        const Vec3 generic_tangent2=native_axis_placeholder && mNativeSingleAxisFriction && !inWorldSpaceTangent2.IsNormalized(1.0e-5f)? inWorldSpaceTangent2.Normalized() : inWorldSpaceTangent2;
		// Implement friction as 2 AxisConstraintParts
		mFrictionConstraint1.TemplatedCalculateConstraintProperties<Type1, Type2>(inInvM1, inInvI1, r1, inInvM2, inInvI2, r2, generic_tangent1, surface_velocity1);
		mFrictionConstraint2.TemplatedCalculateConstraintProperties<Type1, Type2>(inInvM1, inInvI1, r1, inInvM2, inInvI2, r2, generic_tangent2, surface_velocity2);
	}
	else
	{
		// Turn off friction constraint
		mFrictionConstraint1.Deactivate();
		mFrictionConstraint2.Deactivate();
	}
    delete mNativePairV73; mNativePairV73=nullptr;
    if constexpr(Type1==EMotionType::Dynamic && Type2==EMotionType::Dynamic) {
        const char* flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_PAIR_SOLVER_V73");
        if(inSettings.mUseSourceBlockPGS && inSettings.mCombinedRestitution==0.0f &&
           inSettings.mRelativeLinearSurfaceVelocity==Vec3::sZero() && inSettings.mRelativeAngularSurfaceVelocity==Vec3::sZero() &&
           flag && (std::strcmp(flag,"1")==0 || std::strcmp(flag,"2")==0)) {
            namespace P=motorrock_native_pair_v73;
            auto polar=[](Vec3 v){return P::Vector{{v.GetX(),v.GetZ(),v.GetY()}};};
            mNativePairV73=new NativePairStateV73;
            mNativePairV73->mode=std::strcmp(flag,"1")==0?1u:2u;
            mNativePairV73->iteration=0;
            mNativePairV73->row=P::Row{};
            auto &row=mNativePairV73->row;
            row.normal=polar(inWorldSpaceNormal);
            row.tangent0=polar(-inWorldSpaceTangent1);
            row.tangent1=polar(inWorldSpaceTangent2);
            row.mass={{mNonPenetrationConstraint.GetDiagnosticEffectiveMass(),mFrictionConstraint1.GetDiagnosticEffectiveMass(),mFrictionConstraint2.GetDiagnosticEffectiveMass()}};
            const float error=-penetration;
            row.bias={{P::mul(P::mul(P::mul(error,P::div(1.0f,inDeltaTime)),row.mass[0]),0.8f),0.0f,0.0f}};
            row.static_mu={{mPairStaticFriction1V71>=0?mPairStaticFriction1V71:inSettings.mCombinedFriction,mPairStaticFriction2V71>=0?mPairStaticFriction2V71:inSettings.mCombinedFriction2}};
            row.dynamic_mu={{inSettings.mCombinedFriction,inSettings.mCombinedFriction2}};
            // Native pairs are sorted opposite to Jolt's body-ID order.
            const Vec3 levers[]={r2,r1};const Mat44 inverse[]={inInvI2,inInvI1};const float masses[]={inInvM2,inInvM1};const unsigned permutation[]={0,2,1};
            for(unsigned b=0;b<2;++b){
                auto &body=mNativePairV73->bodies[b];body=P::Body{};body.lever=polar(levers[b]);body.inverse_mass=masses[b];
                for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)body.inverse_inertia[c*3+r]=inverse[b].GetColumn3(permutation[c])[permutation[r]];
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// ContactConstraintManager::ContactConstraint
////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef JPH_DEBUG_RENDERER
void ContactConstraintManager::ContactConstraint::Draw(DebugRenderer *inRenderer, ColorArg inManifoldColor) const
{
	if (mContactPoints.empty())
		return;

	// Get body transforms
	RMat44 transform_body1 = mBody1->GetCenterOfMassTransform();
	RMat44 transform_body2 = mBody2->GetCenterOfMassTransform();

	RVec3 prev_point = transform_body1 * Vec3::sLoadFloat3Unsafe(mContactPoints.back().mContactPoint->mPosition1);
	for (const WorldContactPoint &wcp : mContactPoints)
	{
		// Test if any lambda from the previous frame was transferred
		float radius = wcp.mNonPenetrationConstraint.GetTotalLambda() == 0.0f
					&& wcp.mFrictionConstraint1.GetTotalLambda() == 0.0f
					&& wcp.mFrictionConstraint2.GetTotalLambda() == 0.0f? 0.1f :  0.2f;

		RVec3 next_point = transform_body1 * Vec3::sLoadFloat3Unsafe(wcp.mContactPoint->mPosition1);
		inRenderer->DrawMarker(next_point, Color::sCyan, radius);
		inRenderer->DrawMarker(transform_body2 * Vec3::sLoadFloat3Unsafe(wcp.mContactPoint->mPosition2), Color::sPurple, radius);

		// Draw edge
		inRenderer->DrawArrow(prev_point, next_point, inManifoldColor, 0.05f);
		prev_point = next_point;
	}

	// Draw normal
	RVec3 wp = transform_body1 * Vec3::sLoadFloat3Unsafe(mContactPoints[0].mContactPoint->mPosition1);
	inRenderer->DrawArrow(wp, wp + GetWorldSpaceNormal(), Color::sRed, 0.05f);

	// Get tangents
	Vec3 t1, t2;
	GetTangents(t1, t2);

	// Draw tangents
	inRenderer->DrawLine(wp, wp + t1, Color::sGreen);
	inRenderer->DrawLine(wp, wp + t2, Color::sBlue);
}
#endif // JPH_DEBUG_RENDERER

////////////////////////////////////////////////////////////////////////////////////////////////////////
// ContactConstraintManager::CachedContactPoint
////////////////////////////////////////////////////////////////////////////////////////////////////////

void ContactConstraintManager::CachedContactPoint::SaveState(StateRecorder &inStream) const
{
	inStream.Write(mPosition1);
	inStream.Write(mPosition2);
	inStream.Write(mNonPenetrationLambda);
	inStream.Write(mFrictionLambda);
}

void ContactConstraintManager::CachedContactPoint::RestoreState(StateRecorder &inStream)
{
	inStream.Read(mPosition1);
	inStream.Read(mPosition2);
	inStream.Read(mNonPenetrationLambda);
	inStream.Read(mFrictionLambda);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// ContactConstraintManager::CachedManifold
////////////////////////////////////////////////////////////////////////////////////////////////////////

void ContactConstraintManager::CachedManifold::SaveState(StateRecorder &inStream) const
{
	inStream.Write(mContactNormal);
}

void ContactConstraintManager::CachedManifold::RestoreState(StateRecorder &inStream)
{
	inStream.Read(mContactNormal);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// ContactConstraintManager::CachedBodyPair
////////////////////////////////////////////////////////////////////////////////////////////////////////

void ContactConstraintManager::CachedBodyPair::SaveState(StateRecorder &inStream) const
{
	inStream.Write(mDeltaPosition);
	inStream.Write(mDeltaRotation);
}

void ContactConstraintManager::CachedBodyPair::RestoreState(StateRecorder &inStream)
{
	inStream.Read(mDeltaPosition);
	inStream.Read(mDeltaRotation);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// ContactConstraintManager::ManifoldCache
////////////////////////////////////////////////////////////////////////////////////////////////////////

void ContactConstraintManager::ManifoldCache::Init(uint inMaxBodyPairs, uint inMaxContactConstraints, uint inCachedManifoldsSize)
{
	uint max_body_pairs = min(inMaxBodyPairs, cMaxBodyPairsLimit);
	JPH_ASSERT(max_body_pairs == inMaxBodyPairs, "Cannot support this many body pairs!");
	JPH_ASSERT(inMaxContactConstraints <= cMaxContactConstraintsLimit); // Should have been enforced by caller

	mAllocator.Init(uint(min(uint64(max_body_pairs) * sizeof(BodyPairMap::KeyValue) + inCachedManifoldsSize, uint64(~uint(0)))));

	mCachedManifolds.Init(GetNextPowerOf2(inMaxContactConstraints));
	mCachedBodyPairs.Init(GetNextPowerOf2(max_body_pairs));
}

void ContactConstraintManager::ManifoldCache::Clear()
{
	JPH_PROFILE_FUNCTION();

	mCachedManifolds.Clear();
	mCachedBodyPairs.Clear();
	mAllocator.Clear();

#ifdef JPH_ENABLE_ASSERTS
	// Mark as incomplete
	mIsFinalized = false;
#endif
}

void ContactConstraintManager::ManifoldCache::Prepare(uint inExpectedNumBodyPairs, uint inExpectedNumManifolds)
{
	// Minimum amount of buckets to use in the hash map
	constexpr uint32 cMinBuckets = 1024;

	// Use the next higher power of 2 of amount of objects in the cache from last frame to determine the amount of buckets in this frame
	mCachedManifolds.SetNumBuckets(min(max(cMinBuckets, GetNextPowerOf2(inExpectedNumManifolds)), mCachedManifolds.GetMaxBuckets()));
	mCachedBodyPairs.SetNumBuckets(min(max(cMinBuckets, GetNextPowerOf2(inExpectedNumBodyPairs)), mCachedBodyPairs.GetMaxBuckets()));
}

const ContactConstraintManager::MKeyValue *ContactConstraintManager::ManifoldCache::Find(const SubShapeIDPair &inKey, uint64 inKeyHash) const
{
	JPH_ASSERT(mIsFinalized);
	return mCachedManifolds.Find(inKey, inKeyHash);
}

ContactConstraintManager::MKeyValue *ContactConstraintManager::ManifoldCache::Create(ContactAllocator &ioContactAllocator, const SubShapeIDPair &inKey, uint64 inKeyHash, int inNumContactPoints)
{
	JPH_ASSERT(!mIsFinalized);
	MKeyValue *kv = mCachedManifolds.Create(ioContactAllocator, inKey, inKeyHash, CachedManifold::sGetRequiredExtraSize(inNumContactPoints));
	if (kv == nullptr)
	{
		ioContactAllocator.mErrors |= EPhysicsUpdateError::ManifoldCacheFull;
		return nullptr;
	}
	kv->GetValue().mNumContactPoints = uint16(inNumContactPoints);
	++ioContactAllocator.mNumManifolds;
	return kv;
}

ContactConstraintManager::MKVAndCreated ContactConstraintManager::ManifoldCache::FindOrCreate(ContactAllocator &ioContactAllocator, const SubShapeIDPair &inKey, uint64 inKeyHash, int inNumContactPoints)
{
	MKeyValue *kv = const_cast<MKeyValue *>(mCachedManifolds.Find(inKey, inKeyHash));
	if (kv != nullptr)
		return { kv, false };

	return { Create(ioContactAllocator, inKey, inKeyHash, inNumContactPoints), true };
}

uint32 ContactConstraintManager::ManifoldCache::ToHandle(const MKeyValue *inKeyValue) const
{
	JPH_ASSERT(!mIsFinalized);
	return mCachedManifolds.ToHandle(inKeyValue);
}

const ContactConstraintManager::MKeyValue *ContactConstraintManager::ManifoldCache::FromHandle(uint32 inHandle) const
{
	JPH_ASSERT(mIsFinalized);
	return mCachedManifolds.FromHandle(inHandle);
}

const ContactConstraintManager::BPKeyValue *ContactConstraintManager::ManifoldCache::Find(const BodyPair &inKey, uint64 inKeyHash) const
{
	JPH_ASSERT(mIsFinalized);
	return mCachedBodyPairs.Find(inKey, inKeyHash);
}

ContactConstraintManager::BPKeyValue *ContactConstraintManager::ManifoldCache::Create(ContactAllocator &ioContactAllocator, const BodyPair &inKey, uint64 inKeyHash)
{
	JPH_ASSERT(!mIsFinalized);
	BPKeyValue *kv = mCachedBodyPairs.Create(ioContactAllocator, inKey, inKeyHash, 0);
	if (kv == nullptr)
	{
		ioContactAllocator.mErrors |= EPhysicsUpdateError::BodyPairCacheFull;
		return nullptr;
	}
	++ioContactAllocator.mNumBodyPairs;
	return kv;
}

void ContactConstraintManager::ManifoldCache::GetAllBodyPairsSorted(Array<const BPKeyValue *> &outAll) const
{
	JPH_ASSERT(mIsFinalized);
	mCachedBodyPairs.GetAllKeyValues(outAll);

	// Sort by key
	QuickSort(outAll.begin(), outAll.end(), [](const BPKeyValue *inLHS, const BPKeyValue *inRHS) {
		return inLHS->GetKey() < inRHS->GetKey();
	});
}

void ContactConstraintManager::ManifoldCache::GetAllManifoldsSorted(const CachedBodyPair &inBodyPair, Array<const MKeyValue *> &outAll) const
{
	JPH_ASSERT(mIsFinalized);

	// Iterate through the attached manifolds
	for (uint32 handle = inBodyPair.mFirstCachedManifold; handle != ManifoldMap::cInvalidHandle; handle = FromHandle(handle)->GetValue().mNextWithSameBodyPair)
	{
		const MKeyValue *kv = mCachedManifolds.FromHandle(handle);
		outAll.push_back(kv);
	}

	// Sort by key
	QuickSort(outAll.begin(), outAll.end(), [](const MKeyValue *inLHS, const MKeyValue *inRHS) {
		return inLHS->GetKey() < inRHS->GetKey();
	});
}

void ContactConstraintManager::ManifoldCache::GetAllCCDManifoldsSorted(Array<const MKeyValue *> &outAll) const
{
	mCachedManifolds.GetAllKeyValues(outAll);

	for (int i = (int)outAll.size() - 1; i >= 0; --i)
		if ((outAll[i]->GetValue().mFlags & (uint16)CachedManifold::EFlags::CCDContact) == 0)
		{
			outAll[i] = outAll.back();
			outAll.pop_back();
		}

	// Sort by key
	QuickSort(outAll.begin(), outAll.end(), [](const MKeyValue *inLHS, const MKeyValue *inRHS) {
		return inLHS->GetKey() < inRHS->GetKey();
	});
}

void ContactConstraintManager::ManifoldCache::ContactPointRemovedCallbacks(ContactListener *inListener)
{
	JPH_PROFILE_FUNCTION();

	for (MKeyValue &kv : mCachedManifolds)
		if ((kv.GetValue().mFlags & uint16(CachedManifold::EFlags::ContactPersisted)) == 0)
			inListener->OnContactRemoved(kv.GetKey());
}

#ifdef JPH_ENABLE_ASSERTS

void ContactConstraintManager::ManifoldCache::Finalize()
{
	mIsFinalized = true;

#ifdef JPH_MANIFOLD_CACHE_DEBUG
	Trace("ManifoldMap:");
	mCachedManifolds.TraceStats();
	Trace("BodyPairMap:");
	mCachedBodyPairs.TraceStats();
#endif // JPH_MANIFOLD_CACHE_DEBUG
}

#endif

void ContactConstraintManager::ManifoldCache::SaveState(StateRecorder &inStream, const StateRecorderFilter *inFilter) const
{
	JPH_ASSERT(mIsFinalized);

	// Get contents of cache
	Array<const BPKeyValue *> all_bp;
	GetAllBodyPairsSorted(all_bp);

	// Determine which ones to save
	Array<const BPKeyValue *> selected_bp;
	if (inFilter == nullptr)
		selected_bp = std::move(all_bp);
	else
	{
		selected_bp.reserve(all_bp.size());
		for (const BPKeyValue *bp_kv : all_bp)
			if (inFilter->ShouldSaveContact(bp_kv->GetKey().mBodyA, bp_kv->GetKey().mBodyB))
				selected_bp.push_back(bp_kv);
	}

	// Write body pairs
	uint32 num_body_pairs = uint32(selected_bp.size());
	inStream.Write(num_body_pairs);
	for (const BPKeyValue *bp_kv : selected_bp)
	{
		// Write body pair key
		inStream.Write(bp_kv->GetKey());

		// Write body pair
		const CachedBodyPair &bp = bp_kv->GetValue();
		bp.SaveState(inStream);

		// Get attached manifolds
		Array<const MKeyValue *> all_m;
		GetAllManifoldsSorted(bp, all_m);

		// Write num manifolds
		uint32 num_manifolds = uint32(all_m.size());
		inStream.Write(num_manifolds);

		// Write all manifolds
		for (const MKeyValue *m_kv : all_m)
		{
			// Write key
			inStream.Write(m_kv->GetKey());
			const CachedManifold &cm = m_kv->GetValue();
			JPH_ASSERT((cm.mFlags & (uint16)CachedManifold::EFlags::CCDContact) == 0);

			// Write amount of contacts
			inStream.Write(cm.mNumContactPoints);

			// Write manifold
			cm.SaveState(inStream);

			// Write contact points
			for (uint32 i = 0; i < cm.mNumContactPoints; ++i)
				cm.mContactPoints[i].SaveState(inStream);
		}
	}

	// Get CCD manifolds
	Array<const MKeyValue *> all_m;
	GetAllCCDManifoldsSorted(all_m);

	// Determine which ones to save
	Array<const MKeyValue *> selected_m;
	if (inFilter == nullptr)
		selected_m = std::move(all_m);
	else
	{
		selected_m.reserve(all_m.size());
		for (const MKeyValue *m_kv : all_m)
			if (inFilter->ShouldSaveContact(m_kv->GetKey().GetBody1ID(), m_kv->GetKey().GetBody2ID()))
				selected_m.push_back(m_kv);
	}

	// Write all CCD manifold keys
	uint32 num_manifolds = uint32(selected_m.size());
	inStream.Write(num_manifolds);
	for (const MKeyValue *m_kv : selected_m)
		inStream.Write(m_kv->GetKey());
}

bool ContactConstraintManager::ManifoldCache::RestoreState(const ManifoldCache &inReadCache, StateRecorder &inStream, const StateRecorderFilter *inFilter)
{
	JPH_ASSERT(!mIsFinalized);

	bool success = true;

	// Create a contact allocator for restoring the contact cache
	ContactAllocator contact_allocator(GetContactAllocator());

	// When validating, get all existing body pairs
	Array<const BPKeyValue *> all_bp;
	if (inStream.IsValidating())
		inReadCache.GetAllBodyPairsSorted(all_bp);

	// Read amount of body pairs
	uint32 num_body_pairs;
	if (inStream.IsValidating())
		num_body_pairs = uint32(all_bp.size());
	inStream.Read(num_body_pairs);

	// Read entire cache
	for (uint32 i = 0; i < num_body_pairs; ++i)
	{
		// Read key
		BodyPair body_pair_key;
		if (inStream.IsValidating() && i < all_bp.size())
			body_pair_key = all_bp[i]->GetKey();
		inStream.Read(body_pair_key);

		// Check if we want to restore this contact
		if (inFilter == nullptr || inFilter->ShouldRestoreContact(body_pair_key.mBodyA, body_pair_key.mBodyB))
		{
			// Create new entry for this body pair
			uint64 body_pair_hash = body_pair_key.GetHash();
			BPKeyValue *bp_kv = Create(contact_allocator, body_pair_key, body_pair_hash);
			if (bp_kv == nullptr)
			{
				// Out of cache space
				success = false;
				break;
			}
			CachedBodyPair &bp = bp_kv->GetValue();

			// Read body pair
			if (inStream.IsValidating() && i < all_bp.size())
				memcpy(&bp, &all_bp[i]->GetValue(), sizeof(CachedBodyPair));
			bp.RestoreState(inStream);

			// When validating, get all existing manifolds
			Array<const MKeyValue *> all_m;
			if (inStream.IsValidating())
				inReadCache.GetAllManifoldsSorted(all_bp[i]->GetValue(), all_m);

			// Read amount of manifolds
			uint32 num_manifolds = 0;
			if (inStream.IsValidating())
				num_manifolds = uint32(all_m.size());
			inStream.Read(num_manifolds);

			uint32 handle = ManifoldMap::cInvalidHandle;
			for (uint32 j = 0; j < num_manifolds; ++j)
			{
				// Read key
				SubShapeIDPair sub_shape_key;
				if (inStream.IsValidating() && j < all_m.size())
					sub_shape_key = all_m[j]->GetKey();
				inStream.Read(sub_shape_key);
				uint64 sub_shape_key_hash = sub_shape_key.GetHash();

				// Read amount of contact points
				uint16 num_contact_points = 0;
				if (inStream.IsValidating() && j < all_m.size())
					num_contact_points = all_m[j]->GetValue().mNumContactPoints;
				inStream.Read(num_contact_points);

				// Read manifold
				MKeyValue *m_kv = Create(contact_allocator, sub_shape_key, sub_shape_key_hash, num_contact_points);
				if (m_kv == nullptr)
				{
					// Out of cache space
					success = false;
					break;
				}
				CachedManifold &cm = m_kv->GetValue();
				if (inStream.IsValidating() && j < all_m.size())
				{
					memcpy(&cm, &all_m[j]->GetValue(), CachedManifold::sGetRequiredTotalSize(num_contact_points));
					cm.mNumContactPoints = uint16(num_contact_points); // Restore num contact points
				}
				cm.RestoreState(inStream);
				cm.mNextWithSameBodyPair = handle;
				handle = ToHandle(m_kv);

				// Read contact points
				for (uint32 k = 0; k < num_contact_points; ++k)
					cm.mContactPoints[k].RestoreState(inStream);
			}
			bp.mFirstCachedManifold = handle;
		}
		else
		{
			// Skip the contact
			CachedBodyPair bp;
			bp.RestoreState(inStream);
			uint32 num_manifolds = 0;
			inStream.Read(num_manifolds);
			for (uint32 j = 0; j < num_manifolds; ++j)
			{
				SubShapeIDPair sub_shape_key;
				inStream.Read(sub_shape_key);
				uint16 num_contact_points;
				inStream.Read(num_contact_points);
				CachedManifold cm;
				cm.RestoreState(inStream);
				for (uint32 k = 0; k < num_contact_points; ++k)
					cm.mContactPoints[0].RestoreState(inStream);
			}
		}
	}

	// When validating, get all existing CCD manifolds
	Array<const MKeyValue *> all_m;
	if (inStream.IsValidating())
		inReadCache.GetAllCCDManifoldsSorted(all_m);

	// Read amount of CCD manifolds
	uint32 num_manifolds;
	if (inStream.IsValidating())
		num_manifolds = uint32(all_m.size());
	inStream.Read(num_manifolds);

	for (uint32 j = 0; j < num_manifolds; ++j)
	{
		// Read key
		SubShapeIDPair sub_shape_key;
		if (inStream.IsValidating() && j < all_m.size())
			sub_shape_key = all_m[j]->GetKey();
		inStream.Read(sub_shape_key);

		// Check if we want to restore this contact
		if (inFilter == nullptr || inFilter->ShouldRestoreContact(sub_shape_key.GetBody1ID(), sub_shape_key.GetBody2ID()))
		{
			// Create CCD manifold
			uint64 sub_shape_key_hash = sub_shape_key.GetHash();
			MKeyValue *m_kv = Create(contact_allocator, sub_shape_key, sub_shape_key_hash, 0);
			if (m_kv == nullptr)
			{
				// Out of cache space
				success = false;
				break;
			}
			CachedManifold &cm = m_kv->GetValue();
			cm.mFlags |= (uint16)CachedManifold::EFlags::CCDContact;
		}
	}

#ifdef JPH_ENABLE_ASSERTS
	// We don't finalize until the last part is restored
	if (inStream.IsLastPart())
		mIsFinalized = true;
#endif

	return success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// ContactConstraintManager
////////////////////////////////////////////////////////////////////////////////////////////////////////

ContactConstraintManager::ContactConstraintManager(const PhysicsSettings &inPhysicsSettings) :
	mPhysicsSettings(inPhysicsSettings)
{
#ifdef JPH_ENABLE_ASSERTS
	// For the first frame mark this empty buffer as finalized
	mCache[mCacheWriteIdx ^ 1].Finalize();
#endif
}

ContactConstraintManager::~ContactConstraintManager()
{
	JPH_ASSERT(mConstraints == nullptr);
}

void ContactConstraintManager::Init(uint inMaxBodyPairs, uint inMaxContactConstraints)
{
	// Limit the number of constraints so that the allocation size fits in an unsigned integer
	mMaxConstraints = min(inMaxContactConstraints, cMaxContactConstraintsLimit);
	JPH_ASSERT(mMaxConstraints == inMaxContactConstraints, "Cannot support this many contact constraints!");

	// Calculate worst case cache usage
	constexpr uint cMaxManifoldSizePerConstraint = sizeof(CachedManifold) + (MaxSourceContactPoints - 1) * sizeof(CachedContactPoint);
	static_assert(cMaxManifoldSizePerConstraint < sizeof(ContactConstraint)); // If not true, then the next line can overflow
	uint cached_manifolds_size = mMaxConstraints * cMaxManifoldSizePerConstraint;

	// Init the caches
	mCache[0].Init(inMaxBodyPairs, mMaxConstraints, cached_manifolds_size);
	mCache[1].Init(inMaxBodyPairs, mMaxConstraints, cached_manifolds_size);
}

void ContactConstraintManager::PrepareConstraintBuffer(PhysicsUpdateContext *inContext)
{
	// Store context
	mUpdateContext = inContext;

	// Allocate temporary constraint buffer
	JPH_ASSERT(mConstraints == nullptr);
	mConstraints = (ContactConstraint *)inContext->mTempAllocator->Allocate(mMaxConstraints * sizeof(ContactConstraint));
}

template <EMotionType Type1, EMotionType Type2>
JPH_INLINE void ContactConstraintManager::TemplatedCalculateFrictionAndNonPenetrationConstraintProperties(ContactConstraint &ioConstraint, const ContactSettings &inSettings, float inDeltaTime, Vec3Arg inGravityDeltaTime, RMat44Arg inTransformBody1, RMat44Arg inTransformBody2, const Body &inBody1, const Body &inBody2)
{
	// Calculate scaled mass and inertia
	Mat44 inv_i1;
	if constexpr (Type1 == EMotionType::Dynamic)
	{
		const MotionProperties *mp1 = inBody1.GetMotionPropertiesUnchecked();
		inv_i1 = inSettings.mInvInertiaScale1 * mp1->GetInverseInertiaForRotation(inTransformBody1.GetRotation());
	}
	else
	{
		inv_i1 = Mat44::sZero();
	}

	Mat44 inv_i2;
	if constexpr (Type2 == EMotionType::Dynamic)
	{
		const MotionProperties *mp2 = inBody2.GetMotionPropertiesUnchecked();
		inv_i2 = inSettings.mInvInertiaScale2 * mp2->GetInverseInertiaForRotation(inTransformBody2.GetRotation());
	}
	else
	{
		inv_i2 = Mat44::sZero();
	}

	// Calculate tangents
	Vec3 t1, t2;
	ioConstraint.GetTangents(t1, t2);

	Vec3 ws_normal = ioConstraint.GetWorldSpaceNormal();

	// Calculate value for restitution correction
	float gravity_dt_dot_normal = inGravityDeltaTime.Dot(ws_normal);

	// Setup velocity constraint properties
	float min_velocity_for_restitution = mPhysicsSettings.mMinVelocityForRestitution;
	uint source_point_index = 0;
	for (WorldContactPoint &wcp : ioConstraint.mContactPoints)
	{
		wcp.mSourceSeparation = inSettings.mUseSourceSeparation? inSettings.mSourceSeparations[source_point_index] : 0.0f;
		wcp.mDiagnosticNativeError = inSettings.mUseDiagnosticNativeErrors;
		wcp.mDiagnosticNativeConstraintError = inSettings.mDiagnosticNativeErrors[source_point_index];
		wcp.mDerivedNativeError = inSettings.mUseDerivedNativeErrors;
		wcp.mDerivedNativeConstraintError = inSettings.mDerivedNativeErrors[source_point_index];
        wcp.mNativeCachedLeverActive = inSettings.mNativeCachedLeverActive;
        const auto& native_lever = inSettings.mNativeCachedLeverArms[source_point_index];
        wcp.mNativeCachedLever = Vec3(native_lever[0],native_lever[2],native_lever[1]);
		++source_point_index;
		RVec3 p1 = inTransformBody1 * Vec3::sLoadFloat3Unsafe(wcp.mContactPoint->mPosition1);
		RVec3 p2 = inTransformBody2 * Vec3::sLoadFloat3Unsafe(wcp.mContactPoint->mPosition2);
		wcp.TemplatedCalculateFrictionAndNonPenetrationConstraintProperties<Type1, Type2>(inDeltaTime, gravity_dt_dot_normal, inBody1, inBody2, ioConstraint.mInvMass1, ioConstraint.mInvMass2, inv_i1, inv_i2, p1, p2, ws_normal, t1, t2, inSettings, min_velocity_for_restitution);
	}
}

inline void ContactConstraintManager::CalculateFrictionAndNonPenetrationConstraintProperties(ContactConstraint &ioConstraint, const ContactSettings &inSettings, float inDeltaTime, Vec3Arg inGravityDeltaTime, RMat44Arg inTransformBody1, RMat44Arg inTransformBody2, const Body &inBody1, const Body &inBody2)
{
	// Dispatch to the correct templated form
	switch (inBody1.GetMotionType())
	{
	case EMotionType::Dynamic:
		switch (inBody2.GetMotionType())
		{
		case EMotionType::Dynamic:
			TemplatedCalculateFrictionAndNonPenetrationConstraintProperties<EMotionType::Dynamic, EMotionType::Dynamic>(ioConstraint, inSettings, inDeltaTime, inGravityDeltaTime, inTransformBody1, inTransformBody2, inBody1, inBody2);
			break;

		case EMotionType::Kinematic:
			TemplatedCalculateFrictionAndNonPenetrationConstraintProperties<EMotionType::Dynamic, EMotionType::Kinematic>(ioConstraint, inSettings, inDeltaTime, inGravityDeltaTime, inTransformBody1, inTransformBody2, inBody1, inBody2);
			break;

		case EMotionType::Static:
			TemplatedCalculateFrictionAndNonPenetrationConstraintProperties<EMotionType::Dynamic, EMotionType::Static>(ioConstraint, inSettings, inDeltaTime, inGravityDeltaTime, inTransformBody1, inTransformBody2, inBody1, inBody2);
			break;

		default:
			JPH_ASSERT(false);
			break;
		}
		break;

	case EMotionType::Kinematic:
		JPH_ASSERT(inBody2.IsDynamic());
		TemplatedCalculateFrictionAndNonPenetrationConstraintProperties<EMotionType::Kinematic, EMotionType::Dynamic>(ioConstraint, inSettings, inDeltaTime, inGravityDeltaTime, inTransformBody1, inTransformBody2, inBody1, inBody2);
		break;

	case EMotionType::Static:
		JPH_ASSERT(inBody2.IsDynamic());
		TemplatedCalculateFrictionAndNonPenetrationConstraintProperties<EMotionType::Static, EMotionType::Dynamic>(ioConstraint, inSettings, inDeltaTime, inGravityDeltaTime, inTransformBody1, inTransformBody2, inBody1, inBody2);
		break;

	default:
		JPH_ASSERT(false);
		break;
	}
}

void ContactConstraintManager::GetContactsFromCache(ContactAllocator &ioContactAllocator, Body &inBody1, Body &inBody2, bool &outPairHandled, bool &outConstraintCreated)
{
	// Start with nothing found and not handled
	outConstraintCreated = false;
	outPairHandled = false;

	// Swap bodies so that body 1 id < body 2 id
	Body *body1, *body2;
	if (inBody1.GetID() < inBody2.GetID())
	{
		body1 = &inBody1;
		body2 = &inBody2;
	}
	else
	{
		body1 = &inBody2;
		body2 = &inBody1;
	}

	if (mContactListener != nullptr && mContactListener->WantsContactManifoldRefresh(*body1, *body2))
		return;

	// Find the cached body pair
	BodyPair body_pair_key(body1->GetID(), body2->GetID());
	uint64 body_pair_hash = body_pair_key.GetHash();
	const ManifoldCache &read_cache = mCache[mCacheWriteIdx ^ 1];
	const BPKeyValue *kv = read_cache.Find(body_pair_key, body_pair_hash);
	if (kv == nullptr)
		return;
	const CachedBodyPair &input_cbp = kv->GetValue();

	// Get relative translation
	Quat inv_r1 = body1->GetRotation().Conjugated();
	Vec3 delta_position = inv_r1 * Vec3(body2->GetCenterOfMassPosition() - body1->GetCenterOfMassPosition());

	// Get old position delta
	Vec3 old_delta_position = Vec3::sLoadFloat3Unsafe(input_cbp.mDeltaPosition);

	// Check if bodies are still roughly in the same relative position
	if ((delta_position - old_delta_position).LengthSq() > mPhysicsSettings.mBodyPairCacheMaxDeltaPositionSq)
		return;

	// Determine relative orientation
	Quat delta_rotation = inv_r1 * body2->GetRotation();

	// Reconstruct old quaternion delta
	Quat old_delta_rotation = Quat::sLoadFloat3Unsafe(input_cbp.mDeltaRotation);

	// Check if bodies are still roughly in the same relative orientation
	// The delta between 2 quaternions p and q is: p q^* = [rotation_axis * sin(angle / 2), cos(angle / 2)]
	// From the W component we can extract the angle: cos(angle / 2) = px * qx + py * qy + pz * qz + pw * qw = p . q
	// Since we want to abort if the rotation is smaller than -angle or bigger than angle, we can write the comparison as |p . q| < cos(angle / 2)
	if (abs(delta_rotation.Dot(old_delta_rotation)) < mPhysicsSettings.mBodyPairCacheCosMaxDeltaRotationDiv2)
		return;

	// The cache is valid, return that we've handled this body pair
	outPairHandled = true;

	// Copy the cached body pair to this frame
	ManifoldCache &write_cache = mCache[mCacheWriteIdx];
	BPKeyValue *output_bp_kv = write_cache.Create(ioContactAllocator, body_pair_key, body_pair_hash);
	if (output_bp_kv == nullptr)
		return; // Out of cache space
	CachedBodyPair *output_cbp = &output_bp_kv->GetValue();
	memcpy(output_cbp, &input_cbp, sizeof(CachedBodyPair));

	// If there were no contacts, we have handled the contact
	if (input_cbp.mFirstCachedManifold == ManifoldMap::cInvalidHandle)
		return;

	// Get body transforms
	RMat44 transform_body1 = body1->GetCenterOfMassTransform();
	RMat44 transform_body2 = body2->GetCenterOfMassTransform();

	// Get time step
	float delta_time = mUpdateContext->mStepDeltaTime;

	// Calculate value for restitution correction
	Vec3 gravity_dt = mUpdateContext->mPhysicsSystem->GetGravity() * delta_time;

	// Copy manifolds
	uint32 output_handle = ManifoldMap::cInvalidHandle;
	uint32 input_handle = input_cbp.mFirstCachedManifold;
	do
	{
		JPH_PROFILE("Add Constraint From Cached Manifold");

		// Find the existing manifold
		const MKeyValue *input_kv = read_cache.FromHandle(input_handle);
		const SubShapeIDPair &input_key = input_kv->GetKey();
		const CachedManifold &input_cm = input_kv->GetValue();
		JPH_ASSERT(input_cm.mNumContactPoints > 0); // There should be contact points in this manifold!

		// Create room for manifold in write buffer and copy data
		uint64 input_hash = input_key.GetHash();
		MKeyValue *output_kv = write_cache.Create(ioContactAllocator, input_key, input_hash, input_cm.mNumContactPoints);
		if (output_kv == nullptr)
			break; // Out of cache space
		CachedManifold *output_cm = &output_kv->GetValue();
		memcpy(output_cm, &input_cm, CachedManifold::sGetRequiredTotalSize(input_cm.mNumContactPoints));

		// Link the object under the body pairs
		output_cm->mNextWithSameBodyPair = output_handle;
		output_handle = write_cache.ToHandle(output_kv);

		// Calculate default contact settings
		ContactSettings settings;
		settings.mCombinedFriction = mCombineFriction(*body1, input_key.GetSubShapeID1(), *body2, input_key.GetSubShapeID2());
		settings.mCombinedRestitution = mCombineRestitution(*body1, input_key.GetSubShapeID1(), *body2, input_key.GetSubShapeID2());
		settings.mIsSensor = body1->IsSensor() || body2->IsSensor();

		// Calculate world space contact normal
		Vec3 world_space_normal = transform_body2.Multiply3x3(Vec3::sLoadFloat3Unsafe(output_cm->mContactNormal)).Normalized();

		// Call contact listener to update settings
		if (mContactListener != nullptr)
		{
			// Convert constraint to manifold structure for callback
			ContactManifold manifold;
			manifold.mWorldSpaceNormal = world_space_normal;
			manifold.mSubShapeID1 = input_key.GetSubShapeID1();
			manifold.mSubShapeID2 = input_key.GetSubShapeID2();
			manifold.mBaseOffset = transform_body1.GetTranslation();
			manifold.mRelativeContactPointsOn1.resize(output_cm->mNumContactPoints);
			manifold.mRelativeContactPointsOn2.resize(output_cm->mNumContactPoints);
			Mat44 local_transform_body2 = transform_body2.PostTranslated(-manifold.mBaseOffset).ToMat44();
			float penetration_depth = -FLT_MAX;
			for (uint32 i = 0; i < output_cm->mNumContactPoints; ++i)
			{
				const CachedContactPoint &ccp = output_cm->mContactPoints[i];
				manifold.mRelativeContactPointsOn1[i] = transform_body1.Multiply3x3(Vec3::sLoadFloat3Unsafe(ccp.mPosition1));
				manifold.mRelativeContactPointsOn2[i] = local_transform_body2 * Vec3::sLoadFloat3Unsafe(ccp.mPosition2);
				penetration_depth = max(penetration_depth, (manifold.mRelativeContactPointsOn1[i] - manifold.mRelativeContactPointsOn2[i]).Dot(world_space_normal));
			}
			manifold.mPenetrationDepth = penetration_depth; // We don't have the penetration depth anymore, estimate it

			// Notify callback
			mContactListener->OnContactPersisted(*body1, *body2, manifold, settings);
		}

		JPH_ASSERT(settings.mIsSensor || !(body1->IsSensor() || body2->IsSensor()), "Sensors cannot be converted into regular bodies by a contact callback!");
		if (!settings.mIsSensor // If one of the bodies is a sensor, don't actually create the constraint
			&& ((body1->IsDynamic() && settings.mInvMassScale1 != 0.0f) // One of the bodies must have mass to be able to create a contact constraint
				|| (body2->IsDynamic() && settings.mInvMassScale2 != 0.0f)))
		{
			// Add contact constraint in world space for the solver
			uint32 constraint_idx = mNumConstraints++;
			if (constraint_idx >= mMaxConstraints)
			{
				ioContactAllocator.mErrors |= EPhysicsUpdateError::ContactConstraintsFull;
				break;
			}

			// A constraint will be created
			outConstraintCreated = true;

			ContactConstraint &constraint = mConstraints[constraint_idx];
			new (&constraint) ContactConstraint();
			constraint.mBody1 = body1;
			constraint.mBody2 = body2;
			constraint.mSortKey = input_hash;
			world_space_normal.StoreFloat3(&constraint.mWorldSpaceNormal);
			constraint.mCombinedFriction = settings.mCombinedFriction;
		constraint.mAdditionalSeparation = settings.mAdditionalSeparation;
		constraint.mPositionPenetrationSlop = settings.mPositionPenetrationSlop;
			constraint.mUseIndependentFriction = settings.mUseIndependentFriction;
			constraint.mUseBoxContactOrder = settings.mUseBoxContactOrder;
		constraint.mUseSourceBlockPGS = settings.mUseSourceBlockPGS;
			constraint.mCombinedFriction2 = settings.mCombinedFriction2 >= 0.0f? settings.mCombinedFriction2 : settings.mCombinedFriction;
			Vec3 friction_direction1 = settings.mFrictionDirection1 - world_space_normal * settings.mFrictionDirection1.Dot(world_space_normal);
			if (friction_direction1.LengthSq() < Square(1.0e-6f))
				friction_direction1 = world_space_normal.GetNormalizedPerpendicular();
			else
				friction_direction1 = friction_direction1.Normalized();
			friction_direction1.StoreFloat3(&constraint.mWorldSpaceTangent1);
			constraint.mInvMass1 = body1->GetMotionPropertiesUnchecked() != nullptr? settings.mInvMassScale1 * body1->GetMotionPropertiesUnchecked()->GetInverseMassUnchecked() : 0.0f;
			constraint.mInvInertiaScale1 = settings.mInvInertiaScale1;
			constraint.mInvMass2 = body2->GetMotionPropertiesUnchecked() != nullptr? settings.mInvMassScale2 * body2->GetMotionPropertiesUnchecked()->GetInverseMassUnchecked() : 0.0f;
			constraint.mInvInertiaScale2 = settings.mInvInertiaScale2;
			constraint.mContactPoints.resize(output_cm->mNumContactPoints);
			for (uint32 i = 0; i < output_cm->mNumContactPoints; ++i)
			{
				CachedContactPoint &ccp = output_cm->mContactPoints[i];
				WorldContactPoint &wcp = constraint.mContactPoints[i];
			wcp.mSourceSeparation = settings.mUseSourceSeparation? settings.mSourceSeparations[i] : 0.0f;
		wcp.mDiagnosticNativeError = settings.mUseDiagnosticNativeErrors;
		wcp.mDiagnosticNativeConstraintError = settings.mDiagnosticNativeErrors[i];
		wcp.mDerivedNativeError = settings.mUseDerivedNativeErrors;
		wcp.mDerivedNativeConstraintError = settings.mDerivedNativeErrors[i];
        wcp.mNativeCachedLeverActive = settings.mNativeCachedLeverActive;
        const auto& native_lever = settings.mNativeCachedLeverArms[i];
        wcp.mNativeCachedLever = Vec3(native_lever[0],native_lever[2],native_lever[1]);
				wcp.mNonPenetrationConstraint.SetTotalLambda(ccp.mNonPenetrationLambda);
				wcp.mFrictionConstraint1.SetTotalLambda(ccp.mFrictionLambda[0]);
				wcp.mFrictionConstraint2.SetTotalLambda(ccp.mFrictionLambda[1]);
				wcp.mContactPoint = &ccp;
			}

			JPH_DET_LOG("GetContactsFromCache: id1: " << constraint.mBody1->GetID() << " id2: " << constraint.mBody2->GetID() << " key: " << constraint.mSortKey);

			// Calculate friction and non-penetration constraint properties for all contact points
			CalculateFrictionAndNonPenetrationConstraintProperties(constraint, settings, delta_time, gravity_dt, transform_body1, transform_body2, *body1, *body2);

			// Notify island builder
			mUpdateContext->mIslandBuilder->LinkContact(constraint_idx, body1->GetIndexInActiveBodiesInternal(), body2->GetIndexInActiveBodiesInternal());

		#ifdef JPH_DEBUG_RENDERER
			// Draw the manifold
			if (sDrawContactManifolds)
				constraint.Draw(DebugRenderer::sInstance, Color::sYellow);
		#endif // JPH_DEBUG_RENDERER

		#ifdef JPH_TRACK_SIMULATION_STATS
			// Track new contact constraints
			if (!body1->IsStatic())
				body1->GetMotionPropertiesUnchecked()->GetSimulationStats().mNumContactConstraints.fetch_add(1, memory_order_relaxed);
			if (!body2->IsStatic())
				body2->GetMotionPropertiesUnchecked()->GetSimulationStats().mNumContactConstraints.fetch_add(1, memory_order_relaxed);
		#endif
		}

		// Mark contact as persisted so that we won't fire OnContactRemoved callbacks
		input_cm.mFlags |= (uint16)CachedManifold::EFlags::ContactPersisted;

		// Fetch the next manifold
		input_handle = input_cm.mNextWithSameBodyPair;
	}
	while (input_handle != ManifoldMap::cInvalidHandle);
	output_cbp->mFirstCachedManifold = output_handle;
}

ContactConstraintManager::BodyPairHandle ContactConstraintManager::AddBodyPair(ContactAllocator &ioContactAllocator, const Body &inBody1, const Body &inBody2)
{
	// Swap bodies so that body 1 id < body 2 id
	const Body *body1, *body2;
	if (inBody1.GetID() < inBody2.GetID())
	{
		body1 = &inBody1;
		body2 = &inBody2;
	}
	else
	{
		body1 = &inBody2;
		body2 = &inBody1;
	}

	// Add an entry
	BodyPair body_pair_key(body1->GetID(), body2->GetID());
	uint64 body_pair_hash = body_pair_key.GetHash();
	BPKeyValue *body_pair_kv = mCache[mCacheWriteIdx].Create(ioContactAllocator, body_pair_key, body_pair_hash);
	if (body_pair_kv == nullptr)
		return nullptr; // Out of cache space
	CachedBodyPair *cbp = &body_pair_kv->GetValue();
	cbp->mFirstCachedManifold = ManifoldMap::cInvalidHandle;

	// Get relative translation
	Quat inv_r1 = body1->GetRotation().Conjugated();
	Vec3 delta_position = inv_r1 * Vec3(body2->GetCenterOfMassPosition() - body1->GetCenterOfMassPosition());

	// Store it
	delta_position.StoreFloat3(&cbp->mDeltaPosition);

	// Determine relative orientation
	Quat delta_rotation = inv_r1 * body2->GetRotation();
    // This is Jolt's three-float cache key, not the physical body pose.
    // Its reconstructed W assumes a unit quaternion. Keep the original raw
    // integration pose on the body, but make this bounded cache key valid.
    if (!delta_rotation.IsNormalized() && delta_rotation.AllowsOriginalPoseDrift())
        delta_rotation = delta_rotation.Normalized();

	// Store it
	delta_rotation.StoreFloat3(&cbp->mDeltaRotation);

	return cbp;
}

template <EMotionType Type1, EMotionType Type2>
bool ContactConstraintManager::TemplatedAddContactConstraint(ContactAllocator &ioContactAllocator, BodyPairHandle inBodyPairHandle, Body &inBody1, Body &inBody2, const ContactManifold &inManifold)
{
	// Calculate hash
	SubShapeIDPair key { inBody1.GetID(), inManifold.mSubShapeID1, inBody2.GetID(), inManifold.mSubShapeID2 };
	uint64 key_hash = key.GetHash();

	// Determine number of contact points
	int num_contact_points = (int)inManifold.mRelativeContactPointsOn1.size();
	JPH_ASSERT(num_contact_points <= (inManifold.mUseSourceSeparation? MaxSourceContactPoints : MaxContactPoints));
	JPH_ASSERT(num_contact_points == (int)inManifold.mRelativeContactPointsOn2.size());

	// Reserve space for new contact cache entry
	// Note that for dynamic vs dynamic we always require the first body to have a lower body id to get a consistent key
	// under which to look up the contact
	ManifoldCache &write_cache = mCache[mCacheWriteIdx];
	MKeyValue *new_manifold_kv = write_cache.Create(ioContactAllocator, key, key_hash, num_contact_points);
	if (new_manifold_kv == nullptr)
		return false; // Out of cache space
	CachedManifold *new_manifold = &new_manifold_kv->GetValue();

	// Transform the world space normal to the space of body 2 (this is usually the static body)
	RMat44 inverse_transform_body2 = inBody2.GetInverseCenterOfMassTransform();
	inverse_transform_body2.Multiply3x3(inManifold.mWorldSpaceNormal).Normalized().StoreFloat3(&new_manifold->mContactNormal);

	// Settings object that gets passed to the callback
	ContactSettings settings;
	settings.mCombinedFriction = mCombineFriction(inBody1, inManifold.mSubShapeID1, inBody2, inManifold.mSubShapeID2);
	settings.mCombinedRestitution = mCombineRestitution(inBody1, inManifold.mSubShapeID1, inBody2, inManifold.mSubShapeID2);
	settings.mIsSensor = inBody1.IsSensor() || inBody2.IsSensor();

	// Get the contact points for the old cache entry
	const ManifoldCache &read_cache = mCache[mCacheWriteIdx ^ 1];
	const MKeyValue *old_manifold_kv = read_cache.Find(key, key_hash);
	const CachedContactPoint *ccp_start;
	const CachedContactPoint *ccp_end;
	if (old_manifold_kv != nullptr)
	{
		// Call point persisted listener
		if (mContactListener != nullptr)
			mContactListener->OnContactPersisted(inBody1, inBody2, inManifold, settings);

		// Fetch the contact points from the old manifold
		const CachedManifold *old_manifold = &old_manifold_kv->GetValue();
		ccp_start = old_manifold->mContactPoints;
		ccp_end = ccp_start + old_manifold->mNumContactPoints;

		// Mark contact as persisted so that we won't fire OnContactRemoved callbacks
		old_manifold->mFlags |= (uint16)CachedManifold::EFlags::ContactPersisted;
	}
	else
	{
		// Call point added listener
		if (mContactListener != nullptr)
			mContactListener->OnContactAdded(inBody1, inBody2, inManifold, settings);

		// No contact points available from old manifold
		ccp_start = nullptr;
		ccp_end = nullptr;
	}

	// Get inverse transform for body 1
	RMat44 inverse_transform_body1 = inBody1.GetInverseCenterOfMassTransform();

	bool contact_constraint_created = false;

	// If one of the bodies is a sensor, don't actually create the constraint
	JPH_ASSERT(settings.mIsSensor || !(inBody1.IsSensor() || inBody2.IsSensor()), "Sensors cannot be converted into regular bodies by a contact callback!");
	if (!settings.mIsSensor
		&& ((inBody1.IsDynamic() && settings.mInvMassScale1 != 0.0f) // One of the bodies must have mass to be able to create a contact constraint
			|| (inBody2.IsDynamic() && settings.mInvMassScale2 != 0.0f)))
	{
		// Add contact constraint
		uint32 constraint_idx = mNumConstraints++;
		if (constraint_idx >= mMaxConstraints)
		{
			ioContactAllocator.mErrors |= EPhysicsUpdateError::ContactConstraintsFull;

			// Manifold has been created already, we're not filling it in, so we need to reset the contact number of points.
			// Note that we don't hook it up to the body pair cache so that it won't be used as a cache during the next simulation.
			new_manifold->mNumContactPoints = 0;
			return false;
		}

		// We will create a contact constraint
		contact_constraint_created = true;

		ContactConstraint &constraint = mConstraints[constraint_idx];
		new (&constraint) ContactConstraint();
		constraint.mBody1 = &inBody1;
		constraint.mBody2 = &inBody2;
		constraint.mSortKey = key_hash;
		inManifold.mWorldSpaceNormal.StoreFloat3(&constraint.mWorldSpaceNormal);
		constraint.mCombinedFriction = settings.mCombinedFriction;
		constraint.mAdditionalSeparation = settings.mAdditionalSeparation;
		constraint.mPositionPenetrationSlop = settings.mPositionPenetrationSlop;
		constraint.mUseIndependentFriction = settings.mUseIndependentFriction;
		constraint.mUseBoxContactOrder = settings.mUseBoxContactOrder;
		constraint.mUseSourceBlockPGS = settings.mUseSourceBlockPGS;
		constraint.mCombinedFriction2 = settings.mCombinedFriction2 >= 0.0f? settings.mCombinedFriction2 : settings.mCombinedFriction;
		Vec3 friction_direction1 = settings.mFrictionDirection1 - inManifold.mWorldSpaceNormal * settings.mFrictionDirection1.Dot(inManifold.mWorldSpaceNormal);
		if (friction_direction1.LengthSq() < Square(1.0e-6f))
			friction_direction1 = inManifold.mWorldSpaceNormal.GetNormalizedPerpendicular();
		else
			friction_direction1 = friction_direction1.Normalized();
		friction_direction1.StoreFloat3(&constraint.mWorldSpaceTangent1);
		constraint.mInvMass1 = inBody1.GetMotionPropertiesUnchecked() != nullptr? settings.mInvMassScale1 * inBody1.GetMotionPropertiesUnchecked()->GetInverseMassUnchecked() : 0.0f;
		constraint.mInvInertiaScale1 = settings.mInvInertiaScale1;
		constraint.mInvMass2 = inBody2.GetMotionPropertiesUnchecked() != nullptr? settings.mInvMassScale2 * inBody2.GetMotionPropertiesUnchecked()->GetInverseMassUnchecked() : 0.0f;
		constraint.mInvInertiaScale2 = settings.mInvInertiaScale2;

		JPH_DET_LOG("TemplatedAddContactConstraint: id1: " << constraint.mBody1->GetID() << " id2: " << constraint.mBody2->GetID() << " key: " << constraint.mSortKey);

		// Notify island builder
		mUpdateContext->mIslandBuilder->LinkContact(constraint_idx, inBody1.GetIndexInActiveBodiesInternal(), inBody2.GetIndexInActiveBodiesInternal());

		// Get time step
		float delta_time = mUpdateContext->mStepDeltaTime;

		// Calculate value for restitution correction
		float gravity_dt_dot_normal = inManifold.mWorldSpaceNormal.Dot(mUpdateContext->mPhysicsSystem->GetGravity() * delta_time);

		// Calculate scaled mass and inertia
		float inv_m1;
		Mat44 inv_i1;
		if constexpr (Type1 == EMotionType::Dynamic)
		{
			const MotionProperties *mp1 = inBody1.GetMotionPropertiesUnchecked();
			inv_m1 = settings.mInvMassScale1 * mp1->GetInverseMass();
			inv_i1 = settings.mInvInertiaScale1 * mp1->GetInverseInertiaForRotation(inverse_transform_body1.Transposed3x3());
		}
		else
		{
			inv_m1 = 0.0f;
			inv_i1 = Mat44::sZero();
		}

		float inv_m2;
		Mat44 inv_i2;
		if constexpr (Type2 == EMotionType::Dynamic)
		{
			const MotionProperties *mp2 = inBody2.GetMotionPropertiesUnchecked();
			inv_m2 = settings.mInvMassScale2 * mp2->GetInverseMass();
			inv_i2 = settings.mInvInertiaScale2 * mp2->GetInverseInertiaForRotation(inverse_transform_body2.Transposed3x3());
		}
		else
		{
			inv_m2 = 0.0f;
			inv_i2 = Mat44::sZero();
		}

		// Calculate tangents
		Vec3 t1, t2;
		constraint.GetTangents(t1, t2);

		constraint.mContactPoints.resize(num_contact_points);
		for (int i = 0; i < num_contact_points; ++i)
		{
			// Convert to world space and set positions
			WorldContactPoint &wcp = constraint.mContactPoints[i];
			wcp.mSourceSeparation = settings.mUseSourceSeparation? settings.mSourceSeparations[i] : 0.0f;
		wcp.mDiagnosticNativeError = settings.mUseDiagnosticNativeErrors;
		wcp.mDiagnosticNativeConstraintError = settings.mDiagnosticNativeErrors[i];
		wcp.mDerivedNativeError = settings.mUseDerivedNativeErrors;
		wcp.mDerivedNativeConstraintError = settings.mDerivedNativeErrors[i];
        wcp.mNativeCachedLeverActive = settings.mNativeCachedLeverActive;
        const auto& native_lever = settings.mNativeCachedLeverArms[i];
        wcp.mNativeCachedLever = Vec3(native_lever[0],native_lever[2],native_lever[1]);
			RVec3 p1_ws = inManifold.mBaseOffset + inManifold.mRelativeContactPointsOn1[i];
			RVec3 p2_ws = inManifold.mBaseOffset + inManifold.mRelativeContactPointsOn2[i];

			// Convert to local space to the body
			Vec3 p1_ls = Vec3(inverse_transform_body1 * p1_ws);
			Vec3 p2_ls = Vec3(inverse_transform_body2 * p2_ws);

			// Check if we have a close contact point from last update
			bool lambda_set = false;
			for (const CachedContactPoint *ccp = ccp_start; ccp < ccp_end; ccp++)
				if (Vec3::sLoadFloat3Unsafe(ccp->mPosition1).IsClose(p1_ls, mPhysicsSettings.mContactPointPreserveLambdaMaxDistSq)
					&& Vec3::sLoadFloat3Unsafe(ccp->mPosition2).IsClose(p2_ls, mPhysicsSettings.mContactPointPreserveLambdaMaxDistSq))
				{
					// Get lambdas from previous frame
					wcp.mNonPenetrationConstraint.SetTotalLambda(ccp->mNonPenetrationLambda);
					wcp.mFrictionConstraint1.SetTotalLambda(ccp->mFrictionLambda[0]);
					wcp.mFrictionConstraint2.SetTotalLambda(ccp->mFrictionLambda[1]);
					lambda_set = true;
					break;
				}
			if (!lambda_set)
			{
				wcp.mNonPenetrationConstraint.SetTotalLambda(0.0f);
				wcp.mFrictionConstraint1.SetTotalLambda(0.0f);
				wcp.mFrictionConstraint2.SetTotalLambda(0.0f);
			}

			// Create new contact point
			CachedContactPoint &cp = new_manifold->mContactPoints[i];
			p1_ls.StoreFloat3(&cp.mPosition1);
			p2_ls.StoreFloat3(&cp.mPosition2);
			wcp.mContactPoint = &cp;

			// Setup velocity constraint
			wcp.TemplatedCalculateFrictionAndNonPenetrationConstraintProperties<Type1, Type2>(delta_time, gravity_dt_dot_normal, inBody1, inBody2, inv_m1, inv_m2, inv_i1, inv_i2, p1_ws, p2_ws, inManifold.mWorldSpaceNormal, t1, t2, settings, mPhysicsSettings.mMinVelocityForRestitution);
		}

	#ifdef JPH_DEBUG_RENDERER
		// Draw the manifold
		if (sDrawContactManifolds)
			constraint.Draw(DebugRenderer::sInstance, Color::sOrange);
	#endif // JPH_DEBUG_RENDERER

	#ifdef JPH_TRACK_SIMULATION_STATS
		// Track new contact constraints
		if constexpr (Type1 != EMotionType::Static)
			inBody1.GetMotionPropertiesUnchecked()->GetSimulationStats().mNumContactConstraints.fetch_add(1, memory_order_relaxed);
		if constexpr (Type2 != EMotionType::Static)
			inBody2.GetMotionPropertiesUnchecked()->GetSimulationStats().mNumContactConstraints.fetch_add(1, memory_order_relaxed);
	#endif
	}
	else
	{
		// Store the contact manifold in the cache
		for (int i = 0; i < num_contact_points; ++i)
		{
			// Convert to local space to the body
			Vec3 p1 = Vec3(inverse_transform_body1 * (inManifold.mBaseOffset + inManifold.mRelativeContactPointsOn1[i]));
			Vec3 p2 = Vec3(inverse_transform_body2 * (inManifold.mBaseOffset + inManifold.mRelativeContactPointsOn2[i]));

			// Create new contact point
			CachedContactPoint &cp = new_manifold->mContactPoints[i];
			p1.StoreFloat3(&cp.mPosition1);
			p2.StoreFloat3(&cp.mPosition2);

			// Reset contact impulses, we haven't applied any
			cp.mNonPenetrationLambda = 0.0f;
			cp.mFrictionLambda[0] = 0.0f;
			cp.mFrictionLambda[1] = 0.0f;
		}
	}

	// Store cached contact point in body pair cache
	CachedBodyPair *cbp = reinterpret_cast<CachedBodyPair *>(inBodyPairHandle);
	new_manifold->mNextWithSameBodyPair = cbp->mFirstCachedManifold;
	cbp->mFirstCachedManifold = write_cache.ToHandle(new_manifold_kv);

	// A contact constraint was added
	return contact_constraint_created;
}

bool ContactConstraintManager::AddContactConstraint(ContactAllocator &ioContactAllocator, BodyPairHandle inBodyPairHandle, Body &inBody1, Body &inBody2, const ContactManifold &inManifold)
{
	JPH_PROFILE_FUNCTION();

	JPH_DET_LOG("AddContactConstraint: id1: " << inBody1.GetID() << " id2: " << inBody2.GetID()
		<< " subshape1: " << inManifold.mSubShapeID1 << " subshape2: " << inManifold.mSubShapeID2
		<< " normal: " << inManifold.mWorldSpaceNormal << " pendepth: " << inManifold.mPenetrationDepth);

	JPH_ASSERT(inManifold.mWorldSpaceNormal.IsNormalized());

	// Swap bodies so that body 1 id < body 2 id
	const ContactManifold *manifold;
	Body *body1, *body2;
	ContactManifold temp;
	if (inBody2.GetID() < inBody1.GetID())
	{
		body1 = &inBody2;
		body2 = &inBody1;
		temp = inManifold.SwapShapes();
		manifold = &temp;
	}
	else
	{
		body1 = &inBody1;
		body2 = &inBody2;
		manifold = &inManifold;
	}

	ContactManifold source_manifold;
	if (mContactListener != nullptr && mContactListener->OverrideContactManifold(*body1, *body2, *manifold, source_manifold))
	{
		if (source_manifold.mRelativeContactPointsOn1.empty()) return false;
		if (source_manifold.mRelativeContactPointsOn1.size() <= MaxSourceContactPoints) manifold = &source_manifold;
	}

	// Dispatch to the correct templated form
	// Note: Non-dynamic vs non-dynamic can happen in this case due to one body being a sensor, so we need to have an extended switch case here
	switch (body1->GetMotionType())
	{
	case EMotionType::Dynamic:
		{
			switch (body2->GetMotionType())
			{
			case EMotionType::Dynamic:
				return TemplatedAddContactConstraint<EMotionType::Dynamic, EMotionType::Dynamic>(ioContactAllocator, inBodyPairHandle, *body1, *body2, *manifold);

			case EMotionType::Kinematic:
				return TemplatedAddContactConstraint<EMotionType::Dynamic, EMotionType::Kinematic>(ioContactAllocator, inBodyPairHandle, *body1, *body2, *manifold);

			case EMotionType::Static:
				return TemplatedAddContactConstraint<EMotionType::Dynamic, EMotionType::Static>(ioContactAllocator, inBodyPairHandle, *body1, *body2, *manifold);

			default:
				JPH_ASSERT(false);
				break;
			}
			break;
		}

	case EMotionType::Kinematic:
		switch (body2->GetMotionType())
		{
		case EMotionType::Dynamic:
			return TemplatedAddContactConstraint<EMotionType::Kinematic, EMotionType::Dynamic>(ioContactAllocator, inBodyPairHandle, *body1, *body2, *manifold);

		case EMotionType::Kinematic:
			return TemplatedAddContactConstraint<EMotionType::Kinematic, EMotionType::Kinematic>(ioContactAllocator, inBodyPairHandle, *body1, *body2, *manifold);

		case EMotionType::Static:
			return TemplatedAddContactConstraint<EMotionType::Kinematic, EMotionType::Static>(ioContactAllocator, inBodyPairHandle, *body1, *body2, *manifold);

		default:
			JPH_ASSERT(false);
			break;
		}
		break;

	case EMotionType::Static:
		switch (body2->GetMotionType())
		{
		case EMotionType::Dynamic:
			return TemplatedAddContactConstraint<EMotionType::Static, EMotionType::Dynamic>(ioContactAllocator, inBodyPairHandle, *body1, *body2, *manifold);

		case EMotionType::Kinematic:
			return TemplatedAddContactConstraint<EMotionType::Static, EMotionType::Kinematic>(ioContactAllocator, inBodyPairHandle, *body1, *body2, *manifold);

		case EMotionType::Static: // Static vs static not possible
		default:
			JPH_ASSERT(false);
			break;
		}
		break;

	default:
		JPH_ASSERT(false);
		break;
	}

	return false;
}

void ContactConstraintManager::OnCCDContactAdded(ContactAllocator &ioContactAllocator, const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &outSettings)
{
	JPH_ASSERT(inManifold.mWorldSpaceNormal.IsNormalized());

	// Calculate contact settings
	outSettings.mCombinedFriction = mCombineFriction(inBody1, inManifold.mSubShapeID1, inBody2, inManifold.mSubShapeID2);
	outSettings.mCombinedRestitution = mCombineRestitution(inBody1, inManifold.mSubShapeID1, inBody2, inManifold.mSubShapeID2);
	outSettings.mIsSensor = false; // For now, no sensors are supported during CCD

	// The remainder of this function only deals with calling contact callbacks, if there's no contact callback we also don't need to do this work
	if (mContactListener != nullptr)
	{
		// Swap bodies so that body 1 id < body 2 id
		const ContactManifold *manifold;
		const Body *body1, *body2;
		ContactManifold temp;
		if (inBody2.GetID() < inBody1.GetID())
		{
			body1 = &inBody2;
			body2 = &inBody1;
			temp = inManifold.SwapShapes();
			manifold = &temp;
		}
		else
		{
			body1 = &inBody1;
			body2 = &inBody2;
			manifold = &inManifold;
		}

		// Calculate hash
		SubShapeIDPair key { body1->GetID(), manifold->mSubShapeID1, body2->GetID(), manifold->mSubShapeID2 };
		uint64 key_hash = key.GetHash();

		// Check if we already created this contact this physics update
		ManifoldCache &write_cache = mCache[mCacheWriteIdx];
		MKVAndCreated new_manifold_kv = write_cache.FindOrCreate(ioContactAllocator, key, key_hash, 0);
		if (new_manifold_kv.second)
		{
			// This contact is new for this physics update, check if previous update we already had this contact.
			const ManifoldCache &read_cache = mCache[mCacheWriteIdx ^ 1];
			const MKeyValue *old_manifold_kv = read_cache.Find(key, key_hash);
			if (old_manifold_kv == nullptr)
			{
				// New contact
				mContactListener->OnContactAdded(*body1, *body2, *manifold, outSettings);
			}
			else
			{
				// Existing contact
				mContactListener->OnContactPersisted(*body1, *body2, *manifold, outSettings);

				// Mark contact as persisted so that we won't fire OnContactRemoved callbacks
				old_manifold_kv->GetValue().mFlags |= (uint16)CachedManifold::EFlags::ContactPersisted;
			}

			// Check if the cache is full
			if (new_manifold_kv.first != nullptr)
			{
				// We don't store any contact points in this manifold as it is not for caching impulses, we only need to know that the contact was created
				CachedManifold &new_manifold = new_manifold_kv.first->GetValue();
				new_manifold.mContactNormal = { 0, 0, 0 };
				new_manifold.mFlags |= (uint16)CachedManifold::EFlags::CCDContact;
			}
		}
		else
		{
			// Already found this contact this physics update.
			// Note that we can trigger OnContactPersisted multiple times per physics update, but otherwise we have no way of obtaining the settings
			mContactListener->OnContactPersisted(*body1, *body2, *manifold, outSettings);
		}

		// If we swapped body1 and body2 we need to swap the mass scales back
		if (manifold == &temp)
		{
			std::swap(outSettings.mInvMassScale1, outSettings.mInvMassScale2);
			std::swap(outSettings.mInvInertiaScale1, outSettings.mInvInertiaScale2);
			// Note we do not need to negate the relative surface velocity as it is not applied by the CCD collision constraint
		}
	}

	JPH_ASSERT(outSettings.mIsSensor || !(inBody1.IsSensor() || inBody2.IsSensor()), "Sensors cannot be converted into regular bodies by a contact callback!");
}

void ContactConstraintManager::SortContacts(uint32 *inConstraintIdxBegin, uint32 *inConstraintIdxEnd) const
{
	JPH_PROFILE_FUNCTION();

	QuickSort(inConstraintIdxBegin, inConstraintIdxEnd, [this](uint32 inLHS, uint32 inRHS) {
		const ContactConstraint &lhs = mConstraints[inLHS];
		const ContactConstraint &rhs = mConstraints[inRHS];

        // Source investigation: contact pairs follow shape creation order;
        // the default Jolt hash ordering can reverse floor and border rows.
        const char *source_order=std::getenv("RRR3D_EXPERIMENT_NATIVE_CONTACT_PAIR_ORDER_V60");
        if (source_order && std::strcmp(source_order,"1")==0) {
            const auto lhs1=lhs.mBody1->GetID(),rhs1=rhs.mBody1->GetID();
            const auto lhs2=lhs.mBody2->GetID(),rhs2=rhs.mBody2->GetID();
            if (lhs1!=rhs1) return lhs1<rhs1;
            if (lhs2!=rhs2) return lhs2<rhs2;
        }
		// Most of the time the sort key will be different so we sort on that
		if (lhs.mSortKey != rhs.mSortKey)
			return lhs.mSortKey < rhs.mSortKey;

		// If they're equal we use the IDs of body 1 to order
		if (lhs.mBody1 != rhs.mBody1)
			return lhs.mBody1->GetID() < rhs.mBody1->GetID();

		// If they're still equal we use the IDs of body 2 to order
		if (lhs.mBody2 != rhs.mBody2)
			return lhs.mBody2->GetID() < rhs.mBody2->GetID();

		JPH_ASSERT(inLHS == inRHS, "Hash collision, ordering will be inconsistent");
		return false;
	});
}

void ContactConstraintManager::FinalizeContactCacheAndCallContactPointRemovedCallbacks(uint inExpectedNumBodyPairs, uint inExpectedNumManifolds)
{
	JPH_PROFILE_FUNCTION();

#ifdef JPH_ENABLE_ASSERTS
	// Mark cache as finalized
	ManifoldCache &old_write_cache = mCache[mCacheWriteIdx];
	old_write_cache.Finalize();

	// Check that the count of body pairs and manifolds that we tracked outside of the cache (to avoid contention on an atomic) is correct
	JPH_ASSERT(old_write_cache.GetNumBodyPairs() == inExpectedNumBodyPairs);
	JPH_ASSERT(old_write_cache.GetNumManifolds() == inExpectedNumManifolds);
#endif

	// Buffers are now complete, make write buffer the read buffer
	mCacheWriteIdx ^= 1;

	// Get the old read cache / new write cache
	ManifoldCache &old_read_cache = mCache[mCacheWriteIdx];

	// Call the contact point removal callbacks
	if (mContactListener != nullptr)
		old_read_cache.ContactPointRemovedCallbacks(mContactListener);

	// We're done with the old read cache now
	old_read_cache.Clear();

	// Use the amount of contacts from the last iteration to determine the amount of buckets to use in the hash map for the next iteration
	old_read_cache.Prepare(inExpectedNumBodyPairs, inExpectedNumManifolds);
}

bool ContactConstraintManager::WereBodiesInContact(const BodyID &inBody1ID, const BodyID &inBody2ID) const
{
	// The body pair needs to be in the cache and it needs to have a manifold (otherwise it's just a record indicating that there are no collisions)
	const ManifoldCache &read_cache = mCache[mCacheWriteIdx ^ 1];
	BodyPair key;
	if (inBody1ID < inBody2ID)
		key = BodyPair(inBody1ID, inBody2ID);
	else
		key = BodyPair(inBody2ID, inBody1ID);
	uint64 key_hash = key.GetHash();
	const BPKeyValue *kv = read_cache.Find(key, key_hash);
	return kv != nullptr && kv->GetValue().mFirstCachedManifold != ManifoldMap::cInvalidHandle;
}

template <EMotionType Type1, EMotionType Type2>
JPH_INLINE void ContactConstraintManager::sWarmStartConstraint(ContactConstraint &ioConstraint, MotionProperties *ioMotionProperties1, MotionProperties *ioMotionProperties2, float inWarmStartImpulseRatio)
{
	if (ioConstraint.mUseSourceBlockPGS)
	{
		for (WorldContactPoint &wcp : ioConstraint.mContactPoints)
		{
			wcp.mNonPenetrationConstraint.SetTotalLambda(0.0f);
			wcp.mFrictionConstraint1.SetTotalLambda(0.0f);
			wcp.mFrictionConstraint2.SetTotalLambda(0.0f);
            if(wcp.mNativePairV73){wcp.mNativePairV73->row.normal_impulse=0;wcp.mNativePairV73->row.friction={};}
		}
		return;
	}

	// Calculate tangents
	Vec3 t1, t2;
	ioConstraint.GetTangents(t1, t2);

	Vec3 ws_normal = ioConstraint.GetWorldSpaceNormal();

	for (WorldContactPoint &wcp : ioConstraint.mContactPoints)
	{
		// Warm starting: Apply impulse from last frame
		if (wcp.mFrictionConstraint1.IsActive() || wcp.mFrictionConstraint2.IsActive())
		{
			wcp.mFrictionConstraint1.TemplatedWarmStart<Type1, Type2>(ioMotionProperties1, ioConstraint.mInvMass1, ioMotionProperties2, ioConstraint.mInvMass2, t1, inWarmStartImpulseRatio);
			wcp.mFrictionConstraint2.TemplatedWarmStart<Type1, Type2>(ioMotionProperties1, ioConstraint.mInvMass1, ioMotionProperties2, ioConstraint.mInvMass2, t2, inWarmStartImpulseRatio);
		}
		wcp.mNonPenetrationConstraint.TemplatedWarmStart<Type1, Type2>(ioMotionProperties1, ioConstraint.mInvMass1, ioMotionProperties2, ioConstraint.mInvMass2, ws_normal, inWarmStartImpulseRatio);
	}
}

template <class MotionPropertiesCallback>
void ContactConstraintManager::WarmStartVelocityConstraints(const uint32 *inConstraintIdxBegin, const uint32 *inConstraintIdxEnd, float inWarmStartImpulseRatio, MotionPropertiesCallback &ioCallback)
{
	JPH_PROFILE_FUNCTION();

	for (const uint32 *constraint_idx = inConstraintIdxBegin; constraint_idx < inConstraintIdxEnd; ++constraint_idx)
	{
		ContactConstraint &constraint = mConstraints[*constraint_idx];

		// Fetch bodies
		Body &body1 = *constraint.mBody1;
		EMotionType motion_type1 = body1.GetMotionType();
		MotionProperties *motion_properties1 = body1.GetMotionPropertiesUnchecked();

		Body &body2 = *constraint.mBody2;
		EMotionType motion_type2 = body2.GetMotionType();
		MotionProperties *motion_properties2 = body2.GetMotionPropertiesUnchecked();

		// Dispatch to the correct templated form
		// Note: Warm starting doesn't differentiate between kinematic/static bodies so we handle both as static bodies
		if (motion_type1 == EMotionType::Dynamic)
		{
			if (motion_type2 == EMotionType::Dynamic)
			{
				sWarmStartConstraint<EMotionType::Dynamic, EMotionType::Dynamic>(constraint, motion_properties1, motion_properties2, inWarmStartImpulseRatio);

				ioCallback(motion_properties2);
			}
			else
				sWarmStartConstraint<EMotionType::Dynamic, EMotionType::Static>(constraint, motion_properties1, motion_properties2, inWarmStartImpulseRatio);

			ioCallback(motion_properties1);
		}
		else
		{
			JPH_ASSERT(motion_type2 == EMotionType::Dynamic);

			sWarmStartConstraint<EMotionType::Static, EMotionType::Dynamic>(constraint, motion_properties1, motion_properties2, inWarmStartImpulseRatio);

			ioCallback(motion_properties2);
		}
	}
}

// Specialize for the two body callback types
template void ContactConstraintManager::WarmStartVelocityConstraints<CalculateSolverSteps>(const uint32 *inConstraintIdxBegin, const uint32 *inConstraintIdxEnd, float inWarmStartImpulseRatio, CalculateSolverSteps &ioCallback);
template void ContactConstraintManager::WarmStartVelocityConstraints<DummyCalculateSolverSteps>(const uint32 *inConstraintIdxBegin, const uint32 *inConstraintIdxEnd, float inWarmStartImpulseRatio, DummyCalculateSolverSteps &ioCallback);

template <EMotionType Type1, EMotionType Type2>
JPH_INLINE bool ContactConstraintManager::sSolveVelocityConstraint(ContactConstraint &ioConstraint, MotionProperties *ioMotionProperties1, MotionProperties *ioMotionProperties2, ContactListener *inListener)
{
	bool any_impulse_applied = false;

	// Calculate tangents
	Vec3 t1, t2;
	ioConstraint.GetTangents(t1, t2);

	if (ioConstraint.mUseSourceBlockPGS)
	{
		Vec3 normal = ioConstraint.GetWorldSpaceNormal();
		const char *split_flag = std::getenv("RRR3D_EXPERIMENT_CONTACT_SPLIT_BIAS");
		const bool split_bias = split_flag != nullptr && std::strcmp(split_flag, "1") == 0;
		const char *linear_flag = std::getenv("RRR3D_EXPERIMENT_CONTACT_PREMULTIPLIED_LINEAR");
		const bool premultiplied_linear = linear_flag != nullptr && std::strcmp(linear_flag, "1") == 0;
        uint native_index = 0;
        const char *trace_flag = std::getenv("RRR3D_CONTACT_ROWS");
        const bool trace_native = inListener && trace_flag && std::strcmp(trace_flag,"1") == 0;
		for (WorldContactPoint &wcp : ioConstraint.mContactPoints)
		{
            auto &native = wcp.mNativeContact;
            if constexpr(Type1==EMotionType::Dynamic && Type2==EMotionType::Dynamic) {
                if(wcp.mNativePairV73 && wcp.mNativePairV73->mode==1) {
                    namespace P=motorrock_native_pair_v73;
                    const auto polar=[](Vec3 v){return P::Vector{{v.GetX(),v.GetZ(),v.GetY()}};};
                    auto bodies=wcp.mNativePairV73->bodies;
                    MotionProperties* motions[]={ioMotionProperties2,ioMotionProperties1};
                    for(unsigned b=0;b<2;++b){bodies[b].velocity=polar(motions[b]->GetLinearVelocity());bodies[b].angular=polar(-motions[b]->GetAngularVelocity());}
                    auto &row=wcp.mNativePairV73->row;
                    const char* trace=std::getenv("RRR3D_NATIVE_PAIR_TRACE_FILE_V73");
                    if(trace && *trace){
                        static std::mutex traceMutex;static unsigned traceCount=0;std::scoped_lock guard(traceMutex);
                        if(traceCount++<256)if(FILE* file=std::fopen(trace,"a")){
                            std::fprintf(file,"{\"entry\":%u,\"index\":%u,\"iteration\":%u",traceCount,native_index,wcp.mNativePairV73->iteration);
                            const auto vector=[&](const char*name,const P::Vector&v){std::fprintf(file,",\"%s\":[%.9g,%.9g,%.9g]",name,v[0],v[1],v[2]);};
                            vector("normal",row.normal);vector("tangent0",row.tangent0);vector("tangent1",row.tangent1);vector("friction",row.friction);vector("mass",row.mass);vector("bias",row.bias);
                            std::fprintf(file,",\"normal_impulse\":%.9g,\"static_mu\":[%.9g,%.9g],\"dynamic_mu\":[%.9g,%.9g]",row.normal_impulse,row.static_mu[0],row.static_mu[1],row.dynamic_mu[0],row.dynamic_mu[1]);
                            for(unsigned b=0;b<2;++b){char name[32];std::snprintf(name,sizeof(name),"body%u_velocity",b);vector(name,bodies[b].velocity);std::snprintf(name,sizeof(name),"body%u_angular",b);vector(name,bodies[b].angular);std::snprintf(name,sizeof(name),"body%u_lever",b);vector(name,bodies[b].lever);std::fprintf(file,",\"body%u_inverse_mass\":%.9g,\"body%u_inverse_inertia\":[",b,bodies[b].inverse_mass,b);for(unsigned i=0;i<9;++i)std::fprintf(file,"%s%.9g",i?",":"",bodies[b].inverse_inertia[i]);std::fprintf(file,"]");}
                            std::fprintf(file,"}\n");std::fclose(file);
                        }
                    }
                    const auto force=P::solve(row,bodies);
                    for(unsigned b=0;b<2;++b){
                        P::Vector applied,linear;for(unsigned k=0;k<3;++k){applied[k]=P::mul(b==0?1.0f:-1.0f,force[k]);linear[k]=P::mul(applied[k],bodies[b].inverse_mass);}
                        const auto angular=P::tensor(bodies[b].inverse_inertia,P::cross(bodies[b].lever,applied));
                        motions[b]->AddLinearVelocityStep(Vec3(linear[0],linear[2],linear[1]));
                        motions[b]->AddAngularVelocityStep(-Vec3(angular[0],angular[2],angular[1]));
                    }
                    wcp.mNonPenetrationConstraint.SetSourceContactLambda(row.normal_impulse);
                    wcp.mFrictionConstraint1.SetSourceContactLambda(-P::dot(row.tangent0,row.friction));
                    wcp.mFrictionConstraint2.SetSourceContactLambda(P::dot(row.tangent1,row.friction));
                    ++wcp.mNativePairV73->iteration;++native_index;
                    any_impulse_applied|=force!=P::Vector{};
                    continue;
                }
            }
            if (native.mMask && wcp.mNativeSingleAxisFriction) {
                using A = NativeContactArithmetic;
                if (trace_native) inListener->OnNativeContactIteration(*ioConstraint.mBody1,*ioConstraint.mBody2,native_index,native,wcp.mNonPenetrationConstraint.GetDiagnosticEffectiveMass(),wcp.mNonPenetrationConstraint.GetTotalLambda());
                ++native.mIteration; ++native_index;
                MotionProperties *motion = native.mDynamicFirst? ioMotionProperties1 : ioMotionProperties2;
                const Vec3 spin = A::Cross(A::Axial(motion->GetAngularVelocity()), native.mR);
                const Vec3 velocity = A::Polar(motion->GetLinearVelocity());
                const Vec3 point(A::Add(velocity.GetX(),spin.GetX()), A::Add(velocity.GetY(),spin.GetY()), A::Add(velocity.GetZ(),spin.GetZ()));
                const float old = wcp.mNonPenetrationConstraint.GetTotalLambda();
                float delta = A::Sub(A::Mul(wcp.mNonPenetrationConstraint.GetDiagnosticEffectiveMass(),-A::DotXZY(native.mNormal,point)),native.mScaledBias);
                float total = A::Add(old,delta);
                if (total < 0.0f) { total=0.0f; delta=-old; }
                else if (total > FLT_MAX) { total=FLT_MAX; delta=A::Sub(total,old); }
                Vec3 friction_delta = Vec3::sZero();
                if (total > 0.0f) {
                    const float previous = A::DotXZY(wcp.mNativeFrictionAxis,wcp.mNativeFrictionImpulse);
                    float desired = A::Mul(wcp.mNativeFrictionMass,-A::DotXZY(wcp.mNativeFrictionAxis,point));
                    const float target = A::Add(previous,desired);
                    const float limit = A::Mul(wcp.mNativeFrictionCoefficient,total);
                    const float static_limit = A::Mul(wcp.mNativeStaticFrictionCoefficientV69,total);
                    if (std::abs(target) > static_limit)
                        desired = A::Sub(A::Mul(A::Div(limit,std::abs(target)),target),previous);
                    const float previous1=A::DotXZY(wcp.mNativeFrictionAxis1,wcp.mNativeFrictionImpulse);
                    float desired1=A::Mul(wcp.mNativeFrictionMass1,-A::DotXZY(wcp.mNativeFrictionAxis1,point));
                    const float target1=A::Add(previous1,desired1);
                    if(std::abs(target1)>0.0f)
                        desired1=A::Sub(A::Mul(A::Div(0.0f,std::abs(target1)),target1),previous1);
                    friction_delta = Vec3(
                        A::Add(A::Mul(wcp.mNativeFrictionAxis1.GetX(),desired1),A::Mul(wcp.mNativeFrictionAxis.GetX(),desired)),
                        A::Add(A::Mul(wcp.mNativeFrictionAxis1.GetY(),desired1),A::Mul(wcp.mNativeFrictionAxis.GetY(),desired)),
                        A::Add(A::Mul(wcp.mNativeFrictionAxis1.GetZ(),desired1),A::Mul(wcp.mNativeFrictionAxis.GetZ(),desired)));
                    wcp.mNativeFrictionImpulse = Vec3(A::Add(wcp.mNativeFrictionImpulse.GetX(),friction_delta.GetX()), A::Add(wcp.mNativeFrictionImpulse.GetY(),friction_delta.GetY()), A::Add(wcp.mNativeFrictionImpulse.GetZ(),friction_delta.GetZ()));
                }
                const Vec3 force(A::Add(friction_delta.GetX(),A::Mul(native.mNormal.GetX(),delta)), A::Add(friction_delta.GetY(),A::Mul(native.mNormal.GetY(),delta)), A::Add(friction_delta.GetZ(),A::Mul(native.mNormal.GetZ(),delta)));
                const char* forceTrace=std::getenv("RRR3D_CONTACT_FORCE_TRACE_FILE_V62");
                if(forceTrace && *forceTrace) {
                    static std::mutex traceMutex;static unsigned traceCount=0;
                    std::scoped_lock traceLock(traceMutex);
                    if(traceCount++<256)if(FILE* file=std::fopen(forceTrace,"a")) {
                        std::fprintf(file,"{\"entry\":%u,\"index\":%u,\"iteration\":%u,\"r\":[%.9g,%.9g,%.9g],\"point\":[%.9g,%.9g,%.9g],\"axis2\":[%.9g,%.9g,%.9g],\"masses\":[%.9g,%.9g],\"coefficient\":%.9g,\"delta\":%.9g,\"friction\":[%.9g,%.9g,%.9g],\"force\":[%.9g,%.9g,%.9g]}\n",traceCount,native_index-1,native.mIteration,native.mR.GetX(),native.mR.GetY(),native.mR.GetZ(),point.GetX(),point.GetY(),point.GetZ(),wcp.mNativeFrictionAxis.GetX(),wcp.mNativeFrictionAxis.GetY(),wcp.mNativeFrictionAxis.GetZ(),wcp.mNativeFrictionMass1,wcp.mNativeFrictionMass,wcp.mNativeFrictionCoefficient,delta,friction_delta.GetX(),friction_delta.GetY(),friction_delta.GetZ(),force.GetX(),force.GetY(),force.GetZ());
                        std::fclose(file);
                    }
                }
                const Vec3 linear(A::Mul(force.GetX(),native.mInvMass), A::Mul(force.GetY(),native.mInvMass), A::Mul(force.GetZ(),native.mInvMass));
                motion->AddLinearVelocityStep(A::Polar(linear));
                motion->AddAngularVelocityStep(A::Axial(native.Tensor(A::Cross(native.mR,force))));
                wcp.mNonPenetrationConstraint.SetSourceContactLambda(total);
                any_impulse_applied |= force != Vec3::sZero();
                continue;
            }
            if (native.mMask) {
                if (trace_native) inListener->OnNativeContactIteration(*ioConstraint.mBody1,*ioConstraint.mBody2,native_index,native,wcp.mNonPenetrationConstraint.GetDiagnosticEffectiveMass(),wcp.mNonPenetrationConstraint.GetTotalLambda());
                ++native.mIteration; ++native_index;
                MotionProperties *motion = native.mDynamicFirst?ioMotionProperties1:ioMotionProperties2;
                const float old = wcp.mNonPenetrationConstraint.GetTotalLambda();
                float delta = native.Delta(motion,normal,wcp.mNonPenetrationConstraint,split_bias);
                float total = NativeContactArithmetic::Add(old,delta);
                if (total < 0.0f) { total = 0.0f; delta = -old; }
                else if (total > FLT_MAX) { total = FLT_MAX; delta = NativeContactArithmetic::Sub(total,old); }
                else if (!(native.mMask & NativeContactArithmetic::OriginalDelta)) delta = NativeContactArithmetic::Sub(total,old);
                if (native.mMask & NativeContactArithmetic::ForceFirst) any_impulse_applied |= native.Apply(motion,delta);
                else {
                    any_impulse_applied |= wcp.mNonPenetrationConstraint.ApplySourceContactDelta<Type1,Type2>(ioMotionProperties1,ioConstraint.mInvMass1,ioMotionProperties2,ioConstraint.mInvMass2,normal,delta,premultiplied_linear);
                }
                wcp.mNonPenetrationConstraint.SetSourceContactLambda(total);
                continue; // Native prototype is restricted to friction-free normal rows.
            }
            const uint trace_index = native_index++;
            const char *iteration_flag = std::getenv("RRR3D_CONTACT_ITERATION_ROWS");
            const bool trace_iteration = trace_native && iteration_flag && std::strcmp(iteration_flag, "1") == 0;
            const auto trace_axis = [&](const AxisConstraintPart &part, const char *phase, float lambda) {
                if (!trace_iteration) return;
                inListener->OnSourceNormalRow(*ioConstraint.mBody1, *ioConstraint.mBody2, trace_index, phase,
                    part.GetDiagnosticEffectiveMass(), part.GetDiagnosticVelocityBias(),
                    wcp.mSourceUnbiasedVelocityBias, wcp.mSourceSeparation, lambda, 0.0f,
                    Type1 == EMotionType::Static? Vec3::sZero() : part.GetDiagnosticR1CrossAxis(),
                    Type2 == EMotionType::Static? Vec3::sZero() : part.GetDiagnosticR2CrossAxis(),
                    Type1 == EMotionType::Dynamic? part.GetDiagnosticInvI1R1CrossAxis() : Vec3::sZero(),
                    Type2 == EMotionType::Dynamic? part.GetDiagnosticInvI2R2CrossAxis() : Vec3::sZero());
            };
            trace_axis(wcp.mNonPenetrationConstraint, "iteration-before", wcp.mNonPenetrationConstraint.GetTotalLambda());
			float normal_lambda = max(0.0f, wcp.mNonPenetrationConstraint.TemplatedSolveVelocityConstraintGetTotalLambda<Type1, Type2>(ioMotionProperties1, ioMotionProperties2, normal, split_bias));
			float lambda1 = 0.0f, lambda2 = 0.0f;
			if (wcp.mFrictionConstraint1.IsActive() || wcp.mFrictionConstraint2.IsActive())
			{
				const float limit1 = ioConstraint.mCombinedFriction * normal_lambda;
				const float limit2 = ioConstraint.mCombinedFriction2 * normal_lambda;
				lambda1 = Clamp(wcp.mFrictionConstraint1.TemplatedSolveVelocityConstraintGetTotalLambda<Type1, Type2>(ioMotionProperties1, ioMotionProperties2, t1), -limit1, limit1);
                if (wcp.mPairStaticFriction1V71 >= 0.0f) {
                    const float target1=wcp.mFrictionConstraint1.TemplatedSolveVelocityConstraintGetTotalLambda<Type1,Type2>(ioMotionProperties1,ioMotionProperties2,t1);
                    lambda1=std::abs(target1)>wcp.mPairStaticFriction1V71*normal_lambda?Clamp(target1,-limit1,limit1):target1;
                }
                const float target2=wcp.mFrictionConstraint2.TemplatedSolveVelocityConstraintGetTotalLambda<Type1, Type2>(ioMotionProperties1, ioMotionProperties2, t2);
                lambda2=wcp.mPairStaticFriction1V71>=0.0f && std::abs(target2)<=wcp.mPairStaticFriction2V71*normal_lambda?target2:Clamp(target2,-limit2,limit2);
				// Nx retains previously applied friction when this row has no normal force.
				const char *zero_flag = std::getenv("RRR3D_EXPERIMENT_FRICTION_ZERO");
				if (zero_flag != nullptr && std::strcmp(zero_flag, "1") == 0 && normal_lambda == 0.0f) {
					lambda1 = wcp.mFrictionConstraint1.GetTotalLambda();
					lambda2 = wcp.mFrictionConstraint2.GetTotalLambda();
				}
			}
            trace_axis(wcp.mNonPenetrationConstraint, "iteration-normal-calculated", normal_lambda);
            trace_axis(wcp.mFrictionConstraint1, "iteration-friction1-calculated", lambda1);
            trace_axis(wcp.mFrictionConstraint2, "iteration-friction2-calculated", lambda2);
            if constexpr(Type1==EMotionType::Dynamic && Type2==EMotionType::Dynamic) {
                if(wcp.mNativePairV73 && wcp.mNativePairV73->mode==2) {
                    namespace P=motorrock_native_pair_v73;
                    const auto polar=[](Vec3 v){return P::Vector{{v.GetX(),v.GetZ(),v.GetY()}};};
                    const auto n=polar(normal),a=polar(t1),b=polar(t2);
                    const float dn=P::sub(normal_lambda,wcp.mNonPenetrationConstraint.GetTotalLambda());
                    const float da=P::sub(lambda1,wcp.mFrictionConstraint1.GetTotalLambda()),db=P::sub(lambda2,wcp.mFrictionConstraint2.GetTotalLambda());
                    P::Vector force;for(unsigned k=0;k<3;++k)force[k]=P::add(P::add(P::mul(a[k],da),P::mul(b[k],db)),P::mul(n[k],dn));
                    MotionProperties* motions[]={ioMotionProperties2,ioMotionProperties1};
                    for(unsigned i=0;i<2;++i){const auto &body=wcp.mNativePairV73->bodies[i];P::Vector applied,linear;for(unsigned k=0;k<3;++k){applied[k]=P::mul(i==0?1.0f:-1.0f,force[k]);linear[k]=P::mul(applied[k],body.inverse_mass);}const auto angular=P::tensor(body.inverse_inertia,P::cross(body.lever,applied));motions[i]->AddLinearVelocityStep(Vec3(linear[0],linear[2],linear[1]));motions[i]->AddAngularVelocityStep(-Vec3(angular[0],angular[2],angular[1]));}
                    wcp.mNonPenetrationConstraint.SetSourceContactLambda(normal_lambda);wcp.mFrictionConstraint1.SetSourceContactLambda(lambda1);wcp.mFrictionConstraint2.SetSourceContactLambda(lambda2);
                    any_impulse_applied|=force!=P::Vector{};
                    continue;
                }
            }
			any_impulse_applied |= wcp.mNonPenetrationConstraint.TemplatedSolveVelocityConstraintApplyLambda<Type1, Type2>(ioMotionProperties1, ioConstraint.mInvMass1, ioMotionProperties2, ioConstraint.mInvMass2, normal, normal_lambda, premultiplied_linear);
            trace_axis(wcp.mNonPenetrationConstraint, "iteration-normal-applied", normal_lambda);
			if (wcp.mFrictionConstraint1.IsActive() || wcp.mFrictionConstraint2.IsActive())
			{
				any_impulse_applied |= wcp.mFrictionConstraint1.TemplatedSolveVelocityConstraintApplyLambda<Type1, Type2>(ioMotionProperties1, ioConstraint.mInvMass1, ioMotionProperties2, ioConstraint.mInvMass2, t1, lambda1);
                trace_axis(wcp.mFrictionConstraint1, "iteration-friction1-applied", lambda1);
				any_impulse_applied |= wcp.mFrictionConstraint2.TemplatedSolveVelocityConstraintApplyLambda<Type1, Type2>(ioMotionProperties1, ioConstraint.mInvMass1, ioMotionProperties2, ioConstraint.mInvMass2, t2, lambda2);
                trace_axis(wcp.mFrictionConstraint2, "iteration-friction2-applied", lambda2);
			}
		}
		return any_impulse_applied;
	}

	// Nx box face winding. Keep mesh contact reduction and legacy pairs intact.
	if (ioConstraint.mUseBoxContactOrder && ioConstraint.mContactPoints.size() == 4)
	{
		Quat rotation = ioConstraint.mBody1->GetRotation();
		auto position = [&](const WorldContactPoint &p) { return rotation * Vec3::sLoadFloat3Unsafe(p.mContactPoint->mPosition1); };
		Vec3 center = Vec3::sZero();
		for (const WorldContactPoint &p : ioConstraint.mContactPoints) center += position(p);
		center *= 0.25f;
		std::sort(ioConstraint.mContactPoints.begin(), ioConstraint.mContactPoints.end(), [&](const WorldContactPoint &a, const WorldContactPoint &b) {
			Vec3 pa = position(a) - center, pb = position(b) - center;
			return std::atan2(pa.Dot(t2), pa.Dot(t1)) > std::atan2(pb.Dot(t2), pb.Dot(t1));
		});
		float lowest = FLT_MAX;
		for (const WorldContactPoint &p : ioConstraint.mContactPoints) lowest = min(lowest, position(p).Dot(t2));
		// Group vertices on the same box edge at float contact precision.
		float tie = 1.0e-5f * max(1.0f, std::abs(lowest));
		auto anchor = ioConstraint.mContactPoints.begin();
		float primary = -FLT_MAX;
		for (auto p = ioConstraint.mContactPoints.begin(); p != ioConstraint.mContactPoints.end(); ++p)
			if (position(*p).Dot(t2) <= lowest + tie && position(*p).Dot(t1) > primary)
			{
				primary = position(*p).Dot(t1);
				anchor = p;
			}
		std::rotate(ioConstraint.mContactPoints.begin(), anchor, ioConstraint.mContactPoints.end());
	}

	// First apply all friction constraints (non-penetration is more important than friction)
	for (WorldContactPoint &wcp : ioConstraint.mContactPoints)
	{
		// Check if friction is enabled
		if (wcp.mFrictionConstraint1.IsActive() || wcp.mFrictionConstraint2.IsActive())
		{
			// Calculate impulse to stop motion in tangential direction
			float lambda1 = wcp.mFrictionConstraint1.TemplatedSolveVelocityConstraintGetTotalLambda<Type1, Type2>(ioMotionProperties1, ioMotionProperties2, t1);
			float lambda2 = wcp.mFrictionConstraint2.TemplatedSolveVelocityConstraintGetTotalLambda<Type1, Type2>(ioMotionProperties1, ioMotionProperties2, t2);

			// Calculate max impulses that can be applied. Note that we're using the non-penetration impulse from the previous iteration here.
			// We do this because non-penetration is more important so is solved last (the last things that are solved in an iterative solver
			// contribute the most).
			float normal_lambda = wcp.mNonPenetrationConstraint.GetTotalLambda();
			float max_lambda_f1 = ioConstraint.mCombinedFriction * normal_lambda;
			float max_lambda_f2 = ioConstraint.mCombinedFriction2 * normal_lambda;

			if (ioConstraint.mUseIndependentFriction)
			{
				// Nx diagonal frozen-body probe: each axis decelerates at mu*g,
				// rather than sharing the ellipse budget (mu*g/sqrt(2)).
				lambda1 = Clamp(lambda1, -max_lambda_f1, max_lambda_f1);
				lambda2 = Clamp(lambda2, -max_lambda_f2, max_lambda_f2);
			}
			else
			{
				// Clamp to the anisotropic Coulomb ellipse. Equal coefficients
				// reduce to Jolt's original circular cone.
				if (max_lambda_f1 <= 0.0f)
					lambda1 = 0.0f;
				if (max_lambda_f2 <= 0.0f)
					lambda2 = 0.0f;
				float normalized_lambda_sq =
					(max_lambda_f1 > 0.0f? Square(lambda1 / max_lambda_f1) : 0.0f)
					+ (max_lambda_f2 > 0.0f? Square(lambda2 / max_lambda_f2) : 0.0f);
				if (normalized_lambda_sq > 1.0f)
				{
					float scale = 1.0f / sqrt(normalized_lambda_sq);
					lambda1 *= scale;
					lambda2 *= scale;
				}
			}

			// Apply the friction impulse
			if (wcp.mFrictionConstraint1.TemplatedSolveVelocityConstraintApplyLambda<Type1, Type2>(ioMotionProperties1, ioConstraint.mInvMass1, ioMotionProperties2, ioConstraint.mInvMass2, t1, lambda1))
				any_impulse_applied = true;
			if (wcp.mFrictionConstraint2.TemplatedSolveVelocityConstraintApplyLambda<Type1, Type2>(ioMotionProperties1, ioConstraint.mInvMass1, ioMotionProperties2, ioConstraint.mInvMass2, t2, lambda2))
				any_impulse_applied = true;
		}
	}

	Vec3 ws_normal = ioConstraint.GetWorldSpaceNormal();

	// Then apply all non-penetration constraints
	for (WorldContactPoint &wcp : ioConstraint.mContactPoints)
	{
		// Solve non penetration velocities
		if (wcp.mNonPenetrationConstraint.TemplatedSolveVelocityConstraint<Type1, Type2>(ioMotionProperties1, ioConstraint.mInvMass1, ioMotionProperties2, ioConstraint.mInvMass2, ws_normal, 0.0f, FLT_MAX))
			any_impulse_applied = true;
	}

	return any_impulse_applied;
}

bool ContactConstraintManager::SolveVelocityConstraints(const uint32 *inConstraintIdxBegin, const uint32 *inConstraintIdxEnd)
{
	JPH_PROFILE_FUNCTION();

	bool any_impulse_applied = false;

	for (const uint32 *constraint_idx = inConstraintIdxBegin; constraint_idx < inConstraintIdxEnd; ++constraint_idx)
	{
		ContactConstraint &constraint = mConstraints[*constraint_idx];

		// Fetch bodies
		Body &body1 = *constraint.mBody1;
		EMotionType motion_type1 = body1.GetMotionType();
		MotionProperties *motion_properties1 = body1.GetMotionPropertiesUnchecked();

		Body &body2 = *constraint.mBody2;
		EMotionType motion_type2 = body2.GetMotionType();
		MotionProperties *motion_properties2 = body2.GetMotionPropertiesUnchecked();

		// Dispatch to the correct templated form
		switch (motion_type1)
		{
		case EMotionType::Dynamic:
			switch (motion_type2)
			{
			case EMotionType::Dynamic:
				any_impulse_applied |= sSolveVelocityConstraint<EMotionType::Dynamic, EMotionType::Dynamic>(constraint, motion_properties1, motion_properties2, mContactListener);
				break;

			case EMotionType::Kinematic:
				any_impulse_applied |= sSolveVelocityConstraint<EMotionType::Dynamic, EMotionType::Kinematic>(constraint, motion_properties1, motion_properties2, mContactListener);
				break;

			case EMotionType::Static:
				any_impulse_applied |= sSolveVelocityConstraint<EMotionType::Dynamic, EMotionType::Static>(constraint, motion_properties1, motion_properties2, mContactListener);
				break;

			default:
				JPH_ASSERT(false);
				break;
			}
			break;

		case EMotionType::Kinematic:
			JPH_ASSERT(motion_type2 == EMotionType::Dynamic);
			any_impulse_applied |= sSolveVelocityConstraint<EMotionType::Kinematic, EMotionType::Dynamic>(constraint, motion_properties1, motion_properties2, mContactListener);
			break;

		case EMotionType::Static:
			JPH_ASSERT(motion_type2 == EMotionType::Dynamic);
			any_impulse_applied |= sSolveVelocityConstraint<EMotionType::Static, EMotionType::Dynamic>(constraint, motion_properties1, motion_properties2, mContactListener);
			break;

		default:
			JPH_ASSERT(false);
			break;
		}
	}

	return any_impulse_applied;
}

void ContactConstraintManager::StoreAppliedImpulses(const uint32 *inConstraintIdxBegin, const uint32 *inConstraintIdxEnd, Constraint *const *inActiveConstraints, const uint32 *inJointBegin, const uint32 *inJointEnd, float inDeltaTime)
{
	const char *row_trace_flag = std::getenv("RRR3D_CONTACT_ROWS");
	const bool trace_rows = mContactListener != nullptr && row_trace_flag != nullptr && std::strcmp(row_trace_flag, "1") == 0;
	const auto trace_normal_rows = [&](const ContactConstraint &constraint, const char *phase) {
		if (!trace_rows) return;
		uint point_index = 0;
		for (const WorldContactPoint &wcp : constraint.mContactPoints) {
			const auto &row = wcp.mNonPenetrationConstraint;
			mContactListener->OnSourceNormalRow(*constraint.mBody1, *constraint.mBody2, point_index++, phase,
				row.GetDiagnosticEffectiveMass(), row.GetDiagnosticVelocityBias(),
				wcp.mSourceUnbiasedVelocityBias, wcp.mSourceSeparation, row.GetTotalLambda(), inDeltaTime,
				constraint.mBody1->IsStatic()? Vec3::sZero() : row.GetDiagnosticR1CrossAxis(),
				constraint.mBody2->IsStatic()? Vec3::sZero() : row.GetDiagnosticR2CrossAxis(),
				constraint.mBody1->IsDynamic()? row.GetDiagnosticInvI1R1CrossAxis() : Vec3::sZero(),
				constraint.mBody2->IsDynamic()? row.GetDiagnosticInvI2R2CrossAxis() : Vec3::sZero());
		}
	};
	struct SourceMotion { Body *body; Vec3 velocity; Vec3 angular; };
	Array<SourceMotion> source_motion;
	for (const uint32 *idx = inConstraintIdxBegin; idx < inConstraintIdxEnd; ++idx)
	{
		ContactConstraint &constraint = mConstraints[*idx];
		if (!constraint.mUseSourceBlockPGS) continue;
		for (Body *body : { constraint.mBody1, constraint.mBody2 })
			if (body->IsDynamic() && std::none_of(source_motion.begin(), source_motion.end(), [&](const SourceMotion &s) { return s.body == body; }))
				source_motion.push_back({ body, body->GetLinearVelocity(), body->GetAngularVelocity() });
		trace_normal_rows(constraint, "biased-final");
		for (WorldContactPoint &wcp : constraint.mContactPoints) {
			wcp.mNonPenetrationConstraint.SetHardVelocityBias(wcp.mSourceUnbiasedVelocityBias);
			if (wcp.mNativeContact.mMask) wcp.mNativeContact.MakeUnbiased();
            if(wcp.mNativePairV73)wcp.mNativePairV73->row.bias[0]=motorrock_native_pair_v73::add(0.0f,std::max(0.0f,wcp.mNativePairV73->row.bias[0]));
		}
	}
	Array<VehicleConstraint *> source_vehicles;
	if (inActiveConstraints != nullptr)
		for (const uint32 *index = inJointBegin; index < inJointEnd; ++index)
			if (inActiveConstraints[*index]->GetSubType() == EConstraintSubType::Vehicle) {
				auto *vehicle = static_cast<VehicleConstraint *>(inActiveConstraints[*index]);
				Array<Body *> bodies;
				if (vehicle->PrepareSourceFinalVelocityConstraints(bodies)) {
					source_vehicles.push_back(vehicle);
					for (Body *body : bodies)
						if (body->IsDynamic() && std::none_of(source_motion.begin(), source_motion.end(), [&](const SourceMotion &motion) { return motion.body == body; }))
							source_motion.push_back({body, body->GetLinearVelocity(), body->GetAngularVelocity()});
				}
			}
	for (VehicleConstraint *vehicle : source_vehicles) vehicle->SolveVelocityConstraint(inDeltaTime);
	for (const uint32 *idx = inConstraintIdxBegin; idx < inConstraintIdxEnd; ++idx)
		if (mConstraints[*idx].mUseSourceBlockPGS) SolveVelocityConstraints(idx, idx + 1);
	for (const uint32 *idx = inConstraintIdxBegin; idx < inConstraintIdxEnd; ++idx)
		if (mConstraints[*idx].mUseSourceBlockPGS) trace_normal_rows(mConstraints[*idx], "unbiased-final");
	for (const SourceMotion &s : source_motion)
	{
		if (mContactListener != nullptr) mContactListener->OnSourceMotionSolved(*s.body, s.body->GetLinearVelocity(), s.body->GetAngularVelocity());
		s.body->GetMotionProperties()->SetLinearVelocity(s.velocity);
		// PhysX caps before solving; constraint impulses can exceed the cap until the next step.
		auto *motion = s.body->GetMotionProperties();
		const float limit = motion->GetMaxAngularVelocity();
		motion->SetMaxAngularVelocity(std::max(limit, s.angular.Length() + 0.001f));
		motion->SetAngularVelocity(s.angular);
		motion->SetMaxAngularVelocity(limit);
	}

	// Copy back total applied impulse to cache for the next frame
	for (const uint32 *constraint_idx = inConstraintIdxBegin; constraint_idx < inConstraintIdxEnd; ++constraint_idx)
	{
		const ContactConstraint &constraint = mConstraints[*constraint_idx];

		Vec3 normal_impulse = Vec3::sZero(), friction_impulse = Vec3::sZero();
		for (const WorldContactPoint &wcp : constraint.mContactPoints)
		{
			wcp.mContactPoint->mNonPenetrationLambda = wcp.mNonPenetrationConstraint.GetTotalLambda();
			wcp.mContactPoint->mFrictionLambda[0] = wcp.mFrictionConstraint1.GetTotalLambda();
			wcp.mContactPoint->mFrictionLambda[1] = wcp.mFrictionConstraint2.GetTotalLambda();
			normal_impulse -= constraint.GetWorldSpaceNormal() * wcp.mNonPenetrationConstraint.GetTotalLambda();
            if (wcp.mNativeSingleAxisFriction) {
                // The original row accumulates a world vector; projecting it
                // through Jolt's normalized tangent frame would change it.
                const Vec3 applied = NativeContactArithmetic::Polar(wcp.mNativeFrictionImpulse)
                    * (wcp.mNativeContact.mDynamicFirst? 1.0f : -1.0f);
                friction_impulse += applied;
                Vec3 tangent1, tangent2;
                constraint.GetTangents(tangent1,tangent2);
                // Source block PGS resets cached scalars before warm start.
                wcp.mContactPoint->mFrictionLambda[0]=-tangent1.Dot(applied);
                wcp.mContactPoint->mFrictionLambda[1]=-tangent2.Dot(applied);
                continue;
            }
			// Frictionless constraints have no initialized tangent frame (NaN in debug).
			if (wcp.mFrictionConstraint1.GetTotalLambda() != 0.0f || wcp.mFrictionConstraint2.GetTotalLambda() != 0.0f)
			{
				Vec3 tangent1, tangent2;
				constraint.GetTangents(tangent1, tangent2);
				friction_impulse -= tangent1 * wcp.mFrictionConstraint1.GetTotalLambda() + tangent2 * wcp.mFrictionConstraint2.GetTotalLambda();
			}
		}
		if (mContactListener != nullptr)
			mContactListener->OnContactSolved(*constraint.mBody1, *constraint.mBody2, normal_impulse, friction_impulse);
	}
}

bool ContactConstraintManager::SolvePositionConstraints(const uint32 *inConstraintIdxBegin, const uint32 *inConstraintIdxEnd)
{
	JPH_PROFILE_FUNCTION();

	bool any_impulse_applied = false;

	for (const uint32 *constraint_idx = inConstraintIdxBegin; constraint_idx < inConstraintIdxEnd; ++constraint_idx)
	{
		ContactConstraint &constraint = mConstraints[*constraint_idx];

		if (constraint.mUseSourceBlockPGS) continue;

		// Fetch bodies
		Body &body1 = *constraint.mBody1;
		Body &body2 = *constraint.mBody2;

		// Get transforms
		RMat44 transform1 = body1.GetCenterOfMassTransform();
		RMat44 transform2 = body2.GetCenterOfMassTransform();

		Vec3 ws_normal = constraint.GetWorldSpaceNormal();

		for (WorldContactPoint &wcp : constraint.mContactPoints)
		{
			// Calculate new contact point positions in world space (the bodies may have moved)
			RVec3 p1 = transform1 * Vec3::sLoadFloat3Unsafe(wcp.mContactPoint->mPosition1);
			RVec3 p2 = transform2 * Vec3::sLoadFloat3Unsafe(wcp.mContactPoint->mPosition2);

			// Calculate separation along the normal (negative if interpenetrating)
			// Allow a little penetration by default (PhysicsSettings::mPenetrationSlop) to avoid jittering between contact/no-contact which wipes out the contact cache and warm start impulses
			// Clamp penetration to a max PhysicsSettings::mMaxPenetrationDistance so that we don't apply a huge impulse if we're penetrating a lot
			float separation = max(Vec3(p2 - p1).Dot(ws_normal) + constraint.mAdditionalSeparation + wcp.mSourceSeparation + (constraint.mPositionPenetrationSlop >= 0.0f? constraint.mPositionPenetrationSlop : mPhysicsSettings.mPenetrationSlop), -mPhysicsSettings.mMaxPenetrationDistance);

			// Only enforce constraint when separation < 0 (otherwise we're apart)
			if (separation < 0.0f)
			{
				// Update constraint properties (bodies may have moved)
				wcp.CalculateNonPenetrationConstraintProperties(body1, constraint.mInvMass1, constraint.mInvInertiaScale1, body2, constraint.mInvMass2, constraint.mInvInertiaScale2, p1, p2, ws_normal);

				// Solve position errors
				if (wcp.mNonPenetrationConstraint.SolvePositionConstraintWithMassOverride(body1, constraint.mInvMass1, body2, constraint.mInvMass2, ws_normal, separation, mPhysicsSettings.mBaumgarte))
					any_impulse_applied = true;
			}
		}
	}

	return any_impulse_applied;
}

void ContactConstraintManager::RecycleConstraintBuffer()
{
    // Pair experiment state owns no ordinary contact-cache data.
    for(uint32 i=0;i<std::min(mNumConstraints.load(),mMaxConstraints);++i)
        for(auto &wcp:mConstraints[i].mContactPoints){delete wcp.mNativePairV73;wcp.mNativePairV73=nullptr;}
	// Reset constraint array
	mNumConstraints = 0;
}

void ContactConstraintManager::FinishConstraintBuffer()
{
    for(uint32 i=0;i<std::min(mNumConstraints.load(),mMaxConstraints);++i)
        for(auto &wcp:mConstraints[i].mContactPoints){delete wcp.mNativePairV73;wcp.mNativePairV73=nullptr;}
	// Free constraints buffer
	mUpdateContext->mTempAllocator->Free(mConstraints, mMaxConstraints * sizeof(ContactConstraint));
	mConstraints = nullptr;
	mNumConstraints = 0;

	// Reset update context
	mUpdateContext = nullptr;
}

void ContactConstraintManager::SaveState(StateRecorder &inStream, const StateRecorderFilter *inFilter) const
{
	mCache[mCacheWriteIdx ^ 1].SaveState(inStream, inFilter);
}

bool ContactConstraintManager::RestoreState(StateRecorder &inStream, const StateRecorderFilter *inFilter)
{
	bool success = mCache[mCacheWriteIdx].RestoreState(mCache[mCacheWriteIdx ^ 1], inStream, inFilter);

	// If this is the last part, the cache is finalized
	if (inStream.IsLastPart())
	{
		mCacheWriteIdx ^= 1;
		mCache[mCacheWriteIdx].Clear();
	}

	return success;
}

JPH_NAMESPACE_END
