// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#include <Jolt/Jolt.h>
#include <cstdio>
#include <cstdlib>

#include <Jolt/Physics/Vehicle/VehicleConstraint.h>
#include <Jolt/Physics/Vehicle/VehicleController.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/ObjectStream/TypeDeclarations.h>
#include <Jolt/Core/StreamIn.h>
#include <Jolt/Core/StreamOut.h>
#include <Jolt/Core/Factory.h>

JPH_NAMESPACE_BEGIN

JPH_IMPLEMENT_SERIALIZABLE_VIRTUAL(VehicleConstraintSettings)
{
	JPH_ADD_BASE_CLASS(VehicleConstraintSettings, ConstraintSettings)

	JPH_ADD_ATTRIBUTE(VehicleConstraintSettings, mUp)
	JPH_ADD_ATTRIBUTE(VehicleConstraintSettings, mForward)
	JPH_ADD_ATTRIBUTE(VehicleConstraintSettings, mMaxPitchRollAngle)
	JPH_ADD_ATTRIBUTE(VehicleConstraintSettings, mWheels)
	JPH_ADD_ATTRIBUTE(VehicleConstraintSettings, mAntiRollBars)
	JPH_ADD_ATTRIBUTE(VehicleConstraintSettings, mController)
}

void VehicleConstraintSettings::SaveBinaryState(StreamOut &inStream) const
{
	ConstraintSettings::SaveBinaryState(inStream);

	inStream.Write(mUp);
	inStream.Write(mForward);
	inStream.Write(mMaxPitchRollAngle);

	uint32 num_anti_rollbars = (uint32)mAntiRollBars.size();
	inStream.Write(num_anti_rollbars);
	for (const VehicleAntiRollBar &r : mAntiRollBars)
		r.SaveBinaryState(inStream);

	uint32 num_wheels = (uint32)mWheels.size();
	inStream.Write(num_wheels);
	for (const WheelSettings *w : mWheels)
		w->SaveBinaryState(inStream);

	inStream.Write(mController->GetRTTI()->GetHash());
	mController->SaveBinaryState(inStream);
}

void VehicleConstraintSettings::RestoreBinaryState(StreamIn &inStream)
{
	ConstraintSettings::RestoreBinaryState(inStream);

	inStream.Read(mUp);
	inStream.Read(mForward);
	inStream.Read(mMaxPitchRollAngle);

	uint32 num_anti_rollbars = 0;
	inStream.Read(num_anti_rollbars);
	mAntiRollBars.resize(num_anti_rollbars);
	for (VehicleAntiRollBar &r : mAntiRollBars)
		r.RestoreBinaryState(inStream);

	uint32 num_wheels = 0;
	inStream.Read(num_wheels);
	mWheels.resize(num_wheels);
	for (WheelSettings *w : mWheels)
		w->RestoreBinaryState(inStream);

	uint32 hash = 0;
	inStream.Read(hash);
	const RTTI *rtti = Factory::sInstance->Find(hash);
	mController = reinterpret_cast<VehicleControllerSettings *>(rtti->CreateObject());
	mController->RestoreBinaryState(inStream);
}

VehicleConstraint::VehicleConstraint(Body &inVehicleBody, const VehicleConstraintSettings &inSettings) :
	Constraint(inSettings),
	mBody(&inVehicleBody),
	mForward(inSettings.mForward),
	mUp(inSettings.mUp),
	mWorldUp(inSettings.mUp),
	mAntiRollBars(inSettings.mAntiRollBars)
{
	// Check sanity of incoming settings
	JPH_ASSERT(inSettings.mUp.IsNormalized());
	JPH_ASSERT(inSettings.mForward.IsNormalized());
	JPH_ASSERT(!inSettings.mWheels.empty());

	// Store max pitch/roll angle
	SetMaxPitchRollAngle(inSettings.mMaxPitchRollAngle);

	// Construct our controller class
	mController = inSettings.mController->ConstructController(*this);

	// Create wheels
	mWheels.resize(inSettings.mWheels.size());
	for (uint i = 0; i < mWheels.size(); ++i)
		mWheels[i] = mController->ConstructWheel(*inSettings.mWheels[i]);

	// Use the body ID as a seed for the step counter so that not all vehicles will update at the same time
	mCurrentStep = uint32(Hash64(inVehicleBody.GetID().GetIndex()));
}

VehicleConstraint::~VehicleConstraint()
{
	// Destroy controller
	delete mController;

	// Destroy our wheels
	for (Wheel *w : mWheels)
		delete w;
}

void VehicleConstraint::GetWheelLocalBasis(const Wheel *inWheel, Vec3 &outForward, Vec3 &outUp, Vec3 &outRight) const
{
	const WheelSettings *settings = inWheel->mSettings;

	Quat steer_rotation = Quat::sRotation(settings->mSteeringAxis, inWheel->mSteerAngle);
	outUp = steer_rotation * settings->mWheelUp;
	outForward = steer_rotation * settings->mWheelForward;
	outRight = outForward.Cross(outUp).Normalized();
	outForward = outUp.Cross(outRight).Normalized();
}

Mat44 VehicleConstraint::GetWheelLocalTransform(uint inWheelIndex, Vec3Arg inWheelRight, Vec3Arg inWheelUp) const
{
	JPH_ASSERT(inWheelIndex < mWheels.size());

	const Wheel *wheel = mWheels[inWheelIndex];
	const WheelSettings *settings = wheel->mSettings;

	// Use the two vectors provided to calculate a matrix that takes us from wheel model space to X = right, Y = up, Z = forward (the space where we will rotate the wheel)
	Mat44 wheel_to_rotational = Mat44(Vec4(inWheelRight, 0), Vec4(inWheelUp, 0), Vec4(inWheelUp.Cross(inWheelRight), 0), Vec4(0, 0, 0, 1)).Transposed();

	// Calculate the matrix that takes us from the rotational space to vehicle local space
	Vec3 local_forward, local_up, local_right;
	GetWheelLocalBasis(wheel, local_forward, local_up, local_right);
	Vec3 local_wheel_pos = settings->mPosition + settings->mSuspensionDirection * wheel->GetSuspensionLength();
	Mat44 rotational_to_local(Vec4(local_right, 0), Vec4(local_up, 0), Vec4(local_forward, 0), Vec4(local_wheel_pos, 1));

	// Calculate transform of rotated wheel
	return rotational_to_local * Mat44::sRotationX(wheel->mAngle) * wheel_to_rotational;
}

RMat44 VehicleConstraint::GetWheelWorldTransform(uint inWheelIndex, Vec3Arg inWheelRight, Vec3Arg inWheelUp) const
{
	return mBody->GetWorldTransform() * GetWheelLocalTransform(inWheelIndex, inWheelRight, inWheelUp);
}

void VehicleConstraint::OnStep(const PhysicsStepListenerContext &inContext)
{
	JPH_PROFILE_FUNCTION();

	// Callback to higher-level systems. We do it before PreCollide, in case steering changes.
	if (mPreStepCallback != nullptr)
		mPreStepCallback(*this, inContext);

	if (mIsGravityOverridden)
	{
		// If gravity is overridden, we replace the normal gravity calculations
		if (mBody->IsActive())
		{
			MotionProperties *mp = mBody->GetMotionProperties();
			mp->SetGravityFactor(0.0f);
			mBody->AddForce(mGravityOverride / mp->GetInverseMass());
		}

		// And we calculate the world up using the custom gravity
		mWorldUp = (-mGravityOverride).NormalizedOr(mWorldUp);
	}
	else
	{
		// Calculate new world up vector by inverting gravity
		mWorldUp = (-inContext.mPhysicsSystem->GetGravity()).NormalizedOr(mWorldUp);
	}

	// Callback on our controller
	mController->PreCollide(inContext.mDeltaTime, *inContext.mPhysicsSystem);

	// Calculate if this constraint is active by checking if our main vehicle body is active or any of the bodies we touch are active
	mIsActive = mBody->IsActive();

	// Test how often we need to update the wheels
	uint num_steps_between_collisions = mIsActive? mNumStepsBetweenCollisionTestActive : mNumStepsBetweenCollisionTestInactive;

	RMat44 body_transform = mBody->GetWorldTransform();

	const char *lost_mode = std::getenv("RRR3D_EXPERIMENT_SPRING_LOST");
	const bool source_lost = UsesSourceSpringFinal() && lost_mode != nullptr && std::strcmp(lost_mode, "1") == 0;
	// Test collision for wheels
	for (uint wheel_index = 0; wheel_index < mWheels.size(); ++wheel_index)
	{
		Wheel *w = mWheels[wheel_index];
		const WheelSettings *settings = w->mSettings;
		const bool previous_contact = source_lost && !w->mSourceContactLost && !w->mContactBodyID.IsInvalid();
		const BodyID previous_body = w->mContactBodyID;
		const SubShapeID previous_shape = w->mContactSubShapeID;
		const float previous_length = w->mSuspensionLength;
		const RVec3 previous_position = previous_contact? w->mContactPosition : RVec3::sZero();
		const Vec3 previous_normal = previous_contact? w->mContactNormal : Vec3::sZero();
		w->mSourceContactLost = false;

		// Calculate suspension origin and direction
		RVec3 ws_origin = body_transform * settings->mPosition;
		Vec3 ws_direction = body_transform.Multiply3x3(settings->mSuspensionDirection);

		// Test if we need to update this wheel
		if (num_steps_between_collisions == 0
			|| (mCurrentStep + wheel_index) % num_steps_between_collisions != 0)
		{
			// Simplified wheel contact test
			if (!w->mContactBodyID.IsInvalid())
			{
				// Test if the body is still valid
				w->mContactBody = inContext.mPhysicsSystem->GetBodyLockInterfaceNoLock().TryGetBody(w->mContactBodyID);
				if (w->mContactBody == nullptr)
				{
					// It's not, forget the contact
					w->mContactBodyID = BodyID();
					w->mContactSubShapeID = SubShapeID();
					w->mSuspensionLength = settings->mSuspensionMaxLength;
				}
				else
				{
					// Extrapolate the wheel contact properties
					mVehicleCollisionTester->PredictContactProperties(*inContext.mPhysicsSystem, *this, wheel_index, ws_origin, ws_direction, mBody->GetID(), w->mContactBody, w->mContactSubShapeID, w->mContactPosition, w->mContactNormal, w->mSuspensionLength);
				}
			}
		}
		else
		{
			// Full wheel contact test, start by resetting the contact data
			w->mContactBodyID = BodyID();
			w->mContactBody = nullptr;
			w->mContactSubShapeID = SubShapeID();
			w->mSuspensionLength = settings->mSuspensionMaxLength;

			// Test collision to find the floor
			if (mVehicleCollisionTester->Collide(*inContext.mPhysicsSystem, *this, wheel_index, ws_origin, ws_direction, mBody->GetID(), w->mContactBody, w->mContactSubShapeID, w->mContactPosition, w->mContactNormal, w->mSuspensionLength))
			{
				// Store ID (pointer is not valid outside of the simulation step)
				w->mContactBodyID = w->mContactBody->GetID();
			}
		}

		if (w->mContactBody == nullptr && previous_contact) {
			Body *previous = inContext.mPhysicsSystem->GetBodyLockInterfaceNoLock().TryGetBody(previous_body);
			if (previous != nullptr) {
				w->mContactBody = previous;
				w->mContactBodyID = previous_body;
				w->mContactSubShapeID = previous_shape;
				w->mSuspensionLength = previous_length;
				w->mContactPosition = previous_position;
				w->mContactNormal = previous_normal;
				w->mSourceContactLost = true;
			}
		}

		if (w->mContactBody != nullptr)
		{
			// Store contact velocity, cache this as the contact body may be removed
			w->mContactPointVelocity = w->mContactBody->GetPointVelocity(w->mContactPosition);

			// Determine plane constant for axle contact plane
			w->mAxlePlaneConstant = RVec3(w->mContactNormal).Dot(ws_origin + w->mSuspensionLength * ws_direction);

			// Check if body is active, if so the entire vehicle should be active
			mIsActive |= w->mContactBody->IsActive();

			// Determine world space forward using steering angle and body rotation
			Vec3 forward, up, right;
			GetWheelLocalBasis(w, forward, up, right);
			forward = body_transform.Multiply3x3(forward);
			right = body_transform.Multiply3x3(right);

			// The longitudinal axis is in the up/forward plane
			w->mContactLongitudinal = w->mContactNormal.Cross(right);

			// Make sure that the longitudinal axis is aligned with the forward axis
			if (w->mContactLongitudinal.Dot(forward) < 0.0f)
				w->mContactLongitudinal = -w->mContactLongitudinal;

			// Normalize it
			w->mContactLongitudinal = w->mContactLongitudinal.NormalizedOr(w->mContactNormal.GetNormalizedPerpendicular());

			// The lateral axis is perpendicular to contact normal and longitudinal axis
			w->mContactLateral = w->mContactLongitudinal.Cross(w->mContactNormal).Normalized();
		}
	}

	// Callback to higher-level systems. We do it immediately after wheel collision.
	if (mPostCollideCallback != nullptr)
		mPostCollideCallback(*this, inContext);

	// Calculate anti-rollbar impulses
	for (const VehicleAntiRollBar &r : mAntiRollBars)
	{
		JPH_ASSERT(r.mStiffness >= 0.0f);

		Wheel *lw = mWheels[r.mLeftWheel];
		Wheel *rw = mWheels[r.mRightWheel];

		if (lw->mContactBody != nullptr && rw->mContactBody != nullptr)
		{
			// Calculate the impulse to apply based on the difference in suspension length
			float difference = rw->mSuspensionLength - lw->mSuspensionLength;
			float impulse = difference * r.mStiffness * inContext.mDeltaTime;
			lw->mAntiRollBarImpulse = -impulse;
			rw->mAntiRollBarImpulse = impulse;
		}
		else
		{
			// When one of the wheels is not on the ground we don't apply any impulses
			lw->mAntiRollBarImpulse = rw->mAntiRollBarImpulse = 0.0f;
		}
	}

	// Callback on our controller
	mController->PostCollide(inContext.mDeltaTime, *inContext.mPhysicsSystem);

	// Callback to higher-level systems. We do it before the sleep section, in case velocities change.
	if (mPostStepCallback != nullptr)
		mPostStepCallback(*this, inContext);

	// If the wheels are rotating, we don't want to go to sleep yet
	if (mBody->GetAllowSleeping())
	{
		bool allow_sleep = mController->AllowSleep();
		if (allow_sleep)
			for (const Wheel *w : mWheels)
				if (abs(w->mAngularVelocity) > DegreesToRadians(10.0f))
				{
					allow_sleep = false;
					break;
				}
		if (!allow_sleep)
			mBody->ResetSleepTimer();
	}

	// Increment step counter
	++mCurrentStep;
}

void VehicleConstraint::BuildIslands(uint32 inConstraintIndex, IslandBuilder &ioBuilder, BodyManager &inBodyManager)
{
	// Find dynamic bodies that our wheels are touching
	BodyID *body_ids = (BodyID *)JPH_STACK_ALLOC((mWheels.size() + 1) * sizeof(BodyID));
	int num_bodies = 0;
	bool needs_to_activate = false;
	for (const Wheel *w : mWheels)
		if (w->mContactBody != nullptr)
		{
			// Avoid adding duplicates
			bool duplicate = false;
			BodyID id = w->mContactBody->GetID();
			for (int i = 0; i < num_bodies; ++i)
				if (body_ids[i] == id)
				{
					duplicate = true;
					break;
				}
			if (duplicate)
				continue;

			if (w->mContactBody->IsDynamic())
			{
				body_ids[num_bodies++] = id;
				needs_to_activate |= !w->mContactBody->IsActive();
			}
		}

	// Activate bodies, note that if we get here we have already told the system that we're active so that means our main body needs to be active too
	if (!mBody->IsActive())
	{
		// Our main body is not active, activate it too
		body_ids[num_bodies] = mBody->GetID();
		inBodyManager.ActivateBodies(body_ids, num_bodies + 1);
	}
	else if (needs_to_activate)
	{
		// Only activate bodies the wheels are touching
		inBodyManager.ActivateBodies(body_ids, num_bodies);
	}

	// Link the bodies into the same island
	uint32 min_active_index = Body::cInactiveIndex;
	for (int i = 0; i < num_bodies; ++i)
	{
		const Body &body = inBodyManager.GetBody(body_ids[i]);
		min_active_index = min(min_active_index, body.GetIndexInActiveBodiesInternal());
		ioBuilder.LinkBodies(mBody->GetIndexInActiveBodiesInternal(), body.GetIndexInActiveBodiesInternal());
	}

	// Link the constraint in the island
	ioBuilder.LinkConstraint(inConstraintIndex, mBody->GetIndexInActiveBodiesInternal(), min_active_index);
}

uint VehicleConstraint::BuildIslandSplits(LargeIslandSplitter &ioSplitter) const
{
	return ioSplitter.AssignToNonParallelSplit(mBody);
}

void VehicleConstraint::CalculateSuspensionForcePoint(const Wheel &inWheel, Vec3 &outR1PlusU, Vec3 &outR2) const
{
	// Determine point to apply force to
	RVec3 force_point;
	if (inWheel.mSettings->mEnableSuspensionForcePoint)
		force_point = mBody->GetWorldTransform() * inWheel.mSettings->mSuspensionForcePoint;
	else
		force_point = inWheel.mContactPosition;

	// Calculate r1 + u and r2
	outR1PlusU = Vec3(force_point - mBody->GetCenterOfMassPosition());
	outR2 = Vec3(force_point - inWheel.mContactBody->GetCenterOfMassPosition());
}

void VehicleConstraint::CalculatePitchRollConstraintProperties(RMat44Arg inBodyTransform)
{
	// Check if a limit was specified
	if (mCosMaxPitchRollAngle > -1.0f)
	{
		// Calculate cos of angle between world up vector and vehicle up vector
		Vec3 vehicle_up = inBodyTransform.Multiply3x3(mUp);
		mCosPitchRollAngle = mWorldUp.Dot(vehicle_up);
		if (mCosPitchRollAngle < mCosMaxPitchRollAngle)
		{
			// Calculate rotation axis to rotate vehicle towards up
			Vec3 rotation_axis = mWorldUp.Cross(vehicle_up);
			float len = rotation_axis.Length();
			if (len > 0.0f)
				mPitchRollRotationAxis = rotation_axis / len;

			mPitchRollPart.CalculateConstraintProperties(*mBody, Body::sFixedToWorld, mPitchRollRotationAxis);
		}
		else
			mPitchRollPart.Deactivate();
	}
	else
		mPitchRollPart.Deactivate();
}

void VehicleConstraint::SetupVelocityConstraint(float inDeltaTime)
{
	const bool source_stop = UsesSourceSuspensionStop();
	mSourceVelocitySteps = 0;
	RMat44 body_transform = mBody->GetWorldTransform();

	for (Wheel *w : mWheels)
		if (w->mContactBody != nullptr)
		{
			const WheelSettings *settings = w->mSettings;

			Vec3 neg_contact_normal = -w->mContactNormal;

			Vec3 r1_plus_u, r2;
			CalculateSuspensionForcePoint(*w, r1_plus_u, r2);
            const char *frame_flag_v57=std::getenv("RRR3D_EXPERIMENT_NATIVE_WHEEL_ROW_FRAME_V57");
            const bool frame_v57=frame_flag_v57&&std::strcmp(frame_flag_v57,"1")==0&&UsesSourceSpringFinal()&&w->mContactBody->IsStatic()&&w->mSteerAngle==0&&!settings->mEnableSuspensionForcePoint;
            Vec3 prepared_normal_v57=frame_v57?AxisConstraintPart::NativeWheelFrameAxisV57(*mBody,neg_contact_normal):neg_contact_normal;
            const char *local_lost_flag_v58=std::getenv("RRR3D_EXPERIMENT_NATIVE_LOST_LOCAL_FRAME_V58");
            const bool local_lost_v58=frame_v57&&local_lost_flag_v58&&std::strcmp(local_lost_flag_v58,"1")==0;
            if(local_lost_v58 && !w->mSourceContactLost) {
                namespace frame=motorrock_native_wheel_frame_v57;
                auto q=mBody->GetRotation();frame::Quaternion rotation{{-q.GetX(),-q.GetZ(),-q.GetY(),q.GetW()}};
                auto p=w->mContactPosition,c=mBody->GetCenterOfMassPosition();
                auto local=frame::local_point(rotation,{{float(p.GetX()),float(p.GetZ()),float(p.GetY())}},{{float(c.GetX()),float(c.GetZ()),float(c.GetY())}});
                auto axis=frame::inverse(rotation,{{-neg_contact_normal.GetX(),-neg_contact_normal.GetZ(),-neg_contact_normal.GetY()}},true);
                w->mSourceLocalContactPointV58=Vec3(local[0],local[1],local[2]);
                w->mSourceLocalContactAxisV58=Vec3(axis[0],axis[1],axis[2]);
                w->mSourceLocalContactValidV58=true;
            }
            const bool cached_frame_v58=local_lost_v58&&w->mSourceContactLost&&w->mSourceLocalContactValidV58;
            if(cached_frame_v58)prepared_normal_v57=AxisConstraintPart::NativeWheelLocalAxisV58(*mBody,w->mSourceLocalContactAxisV58);
            if(!w->mSourceContactLost) {
                w->mSourceTireCacheV59.valid=false;
                w->mSourceTireCacheV59.anchor_prepared=false;
                if(w->CanCacheSourceTiresV59(*this)) {
                    namespace frame=motorrock_native_wheel_frame_v57;
                    auto q=mBody->GetRotation();frame::Quaternion rotation{{-q.GetX(),-q.GetZ(),-q.GetY(),q.GetW()}};
                    auto lon=w->mContactLongitudinal,lat=w->mContactLateral;
                    auto a=frame::inverse(rotation,{{lon.GetX(),lon.GetZ(),lon.GetY()}},true);
                    auto b=frame::inverse(rotation,{{lat.GetX(),lat.GetZ(),lat.GetY()}},true);
                    w->mSourceTireCacheV59.longitudinal_axis=Vec3(a[0],a[1],a[2]);
                    w->mSourceTireCacheV59.lateral_axis=Vec3(b[0],b[1],b[2]);
                }
            }


			// Suspension spring
			if (settings->mSuspensionMaxLength > settings->mSuspensionMinLength
				&& (settings->mSuspensionSpring.mMode == ESpringMode::FrequencyAndDamping
					|| settings->mSuspensionSpring.mStiffness > 0.0f || settings->mSuspensionSpring.mDamping > 0.0f))
			{
				float stiffness, damping;
				if (settings->mSuspensionSpring.mMode == ESpringMode::FrequencyAndDamping)
				{
					// Calculate effective mass based on vehicle configuration (the stiffness of the spring should not be affected by the dynamics of the vehicle): K = 1 / (J M^-1 J^T)
					// Note that if no suspension force point is supplied we don't know where the force is applied so we assume it is applied at average suspension length
					Vec3 force_point = settings->mEnableSuspensionForcePoint? settings->mSuspensionForcePoint : settings->mPosition + 0.5f * (settings->mSuspensionMinLength + settings->mSuspensionMaxLength) * settings->mSuspensionDirection;
					Vec3 force_point_x_neg_up = force_point.Cross(-mUp);
					const MotionProperties *mp = mBody->GetMotionProperties();
					float effective_mass = 1.0f / (mp->GetInverseMass() + force_point_x_neg_up.Dot(mp->GetLocalSpaceInverseInertia().Multiply3x3(force_point_x_neg_up)));

					// Convert frequency and damping to stiffness and damping
					float omega = 2.0f * JPH_PI * settings->mSuspensionSpring.mFrequency;
					stiffness = effective_mass * Square(omega);
					damping = 2.0f * effective_mass * settings->mSuspensionSpring.mDamping * omega;
				}
				else
				{
					// In this case we can simply copy the properties
					stiffness = settings->mSuspensionSpring.mStiffness;
					damping = settings->mSuspensionSpring.mDamping;
				}

				// Calculate the damping and frequency of the suspension spring given the angle between the suspension direction and the contact normal
				// If the angle between the suspension direction and the inverse of the contact normal is alpha then the force on the spring relates to the force along the contact normal as:
				//
				// Fspring = Fnormal * cos(alpha)
				//
				// The spring force is:
				//
				// Fspring = -k * x
				//
				// where k is the spring constant and x is the displacement of the spring. So we have:
				//
				// Fnormal * cos(alpha) = -k * x <=> Fnormal = -k / cos(alpha) * x
				//
				// So we can see this as a spring with spring constant:
				//
				// k' = k / cos(alpha)
				//
				// In the same way the velocity relates like:
				//
				// Vspring = Vnormal * cos(alpha)
				//
				// Which results in the modified damping constant c:
				//
				// c' = c / cos(alpha)
				//
				// Note that we clamp 1 / cos(alpha) to the range [0.1, 1] in order not to increase the stiffness / damping by too much.
				Vec3 ws_direction = body_transform.Multiply3x3(settings->mSuspensionDirection);
				const char *raw_mode = std::getenv("RRR3D_EXPERIMENT_SUSPENSION_RAW");
				const bool raw_spring = UsesSourceSpringFinal() && raw_mode != nullptr && std::strcmp(raw_mode, "1") == 0;
				float cos_angle = raw_spring? 1.0f : max(0.1f, ws_direction.Dot(neg_contact_normal));
				stiffness /= cos_angle;
				damping /= cos_angle;

				// Get the value of the constraint equation
				float c = (w->mSuspensionLength - settings->mSuspensionMaxLength - settings->mSuspensionPreloadLength) * settings->mSuspensionSpringBiasFactor;

				w->mSuspensionPart.CalculateConstraintPropertiesWithStiffnessAndDamping(inDeltaTime, *mBody, r1_plus_u, *w->mContactBody, r2, neg_contact_normal, w->mAntiRollBarImpulse, c, stiffness, damping);
				w->mNativeSpringV53.enabled = false;
				const char *split_v53 = std::getenv("RRR3D_EXPERIMENT_NATIVE_SPRING_SPLIT_V53");
				if (UsesSourceSpringFinal() && split_v53 != nullptr && std::strcmp(split_v53, "1") == 0) w->mSuspensionPart.PrepareNativeSplitSpringV53(w->mNativeSpringV53);
                if(cached_frame_v58)w->mSuspensionPart.PrepareNativeWheelLocalFrameV58(*mBody,w->mSourceLocalContactPointV58,w->mSourceLocalContactAxisV58);
                else if(frame_v57)w->mSuspensionPart.PrepareNativeWheelFrameV57(*mBody,w->mContactPosition,neg_contact_normal);
                const char *inertia_v56=std::getenv("RRR3D_EXPERIMENT_NATIVE_WHEEL_ROW_INERTIA_V56");
                if(inertia_v56&&std::strcmp(inertia_v56,"1")==0)w->mSuspensionPart.PrepareNativeWheelRowInertiaV56(*mBody,*w->mContactBody,prepared_normal_v57,false);
                const char *prep_v53 = std::getenv("RRR3D_EXPERIMENT_NATIVE_SPRING_PREP_V53");
                if (w->mNativeSpringV53.enabled && prep_v53 != nullptr && std::strcmp(prep_v53, "1") == 0) {
                    using namespace motorrock_native_spring_v53;
                    float raw_error = sub(add(add(sub(w->mSuspensionLength, settings->mSuspensionMaxLength), 0.025f), 0.025f), settings->mSourceSpringTargetV53);
                    w->mSuspensionPart.PrepareNativeSpringCoefficientsV53(*mBody, *w->mContactBody, prepared_normal_v57, inDeltaTime, raw_error, stiffness, damping, settings->mSuspensionSpringBiasFactor, w->mNativeSpringV53);
                }

			}
			else
				w->mSuspensionPart.Deactivate();

			// Check if we reached the 'max up' position and if so add a hard velocity constraint that stops any further movement in the normal direction
			if (w->mSuspensionLength < settings->mSuspensionMinLength + (source_stop? 0.05f : 0.0f)) {
				w->mSuspensionMaxUpPart.CalculateConstraintProperties(*mBody, r1_plus_u, *w->mContactBody, r2, neg_contact_normal);
				if (source_stop) w->mSuspensionMaxUpPart.SetHardVelocityBias(0.7f * (w->mSuspensionLength - settings->mSuspensionMinLength) / inDeltaTime);
			} else
				w->mSuspensionMaxUpPart.Deactivate();

			// Friction and propulsion
            if(w->HasSourceLostTiresV59(*this)) {
                auto lon=AxisConstraintPart::NativeWheelLocalAxisV58(*mBody,w->mSourceTireCacheV59.longitudinal_axis);
                auto lat=AxisConstraintPart::NativeWheelLocalAxisV58(*mBody,w->mSourceTireCacheV59.lateral_axis);
                // Initialize hard-row storage with the collision axes. The
                // original non-unit prepared axes/Jacobians and mass replace
                // this generic preparation below, as in the active v57 path.
                w->mLongitudinalPart.CalculateConstraintProperties(*mBody,r1_plus_u,*w->mContactBody,r2,-w->mContactLongitudinal);
                w->mLateralPart.CalculateConstraintProperties(*mBody,r1_plus_u,*w->mContactBody,r2,-w->mContactLateral);
                w->mLongitudinalPart.PrepareNativeWheelLocalFrameV58(*mBody,w->mSourceLocalContactPointV58,w->mSourceTireCacheV59.longitudinal_axis);
                w->mLateralPart.PrepareNativeWheelLocalFrameV58(*mBody,w->mSourceLocalContactPointV58,w->mSourceTireCacheV59.lateral_axis);
                w->mLongitudinalPart.PrepareNativeWheelRowInertiaV56(*mBody,*w->mContactBody,lon,true);
                w->mLateralPart.PrepareNativeWheelRowInertiaV56(*mBody,*w->mContactBody,lat,true);
                w->mLongitudinalPart.SetNativeWheelAnchorV57(w->mSourceTireCacheV59.rolling,w->mSourceTireCacheV59.longitudinal_geometry);
                w->mLateralPart.SetNativeWheelAnchorV57(0,w->mSourceTireCacheV59.lateral_geometry);
            } else if (w->mSourceContactLost) {
				w->mLongitudinalPart.Deactivate();
				w->mLateralPart.Deactivate();
			} else {
			w->mLongitudinalPart.CalculateConstraintProperties(*mBody, r1_plus_u, *w->mContactBody, r2, -w->mContactLongitudinal);
			w->mLateralPart.CalculateConstraintProperties(*mBody, r1_plus_u, *w->mContactBody, r2, -w->mContactLateral);
            if(frame_v57){
             w->mLongitudinalPart.PrepareNativeWheelFrameV57(*mBody,w->mContactPosition,-w->mContactLongitudinal);
             w->mLateralPart.PrepareNativeWheelFrameV57(*mBody,w->mContactPosition,-w->mContactLateral);
            }

            const char *inertia_v56=std::getenv("RRR3D_EXPERIMENT_NATIVE_WHEEL_ROW_INERTIA_V56");
            if(inertia_v56&&std::strcmp(inertia_v56,"1")==0){
             w->mLongitudinalPart.PrepareNativeWheelRowInertiaV56(*mBody,*w->mContactBody,frame_v57?AxisConstraintPart::NativeWheelFrameAxisV57(*mBody,-w->mContactLongitudinal):-w->mContactLongitudinal,true);
             w->mLateralPart.PrepareNativeWheelRowInertiaV56(*mBody,*w->mContactBody,frame_v57?AxisConstraintPart::NativeWheelFrameAxisV57(*mBody,-w->mContactLateral):-w->mContactLateral,true);
            }
			}
		}
		else
		{
			// No contact -> disable everything
            w->mSourceTireCacheV59.valid=false;
			w->mSuspensionPart.Deactivate();
			w->mSuspensionMaxUpPart.Deactivate();
			w->mLongitudinalPart.Deactivate();
			w->mLateralPart.Deactivate();
		}

	CalculatePitchRollConstraintProperties(body_transform);
}

void VehicleConstraint::ResetWarmStart()
{
	for (Wheel *w : mWheels)
	{
        w->mSourceTireCacheV59.valid=false;
		w->mSuspensionPart.Deactivate();
		w->mSuspensionMaxUpPart.Deactivate();
		w->mLongitudinalPart.Deactivate();
		w->mLateralPart.Deactivate();
	}

	mPitchRollPart.Deactivate();
}

void VehicleConstraint::WarmStartVelocityConstraint(float inWarmStartImpulseRatio)
{
	const float stop_ratio = UsesSourceSuspensionStop()? 0.0f : inWarmStartImpulseRatio;
	const char *cold_mode = std::getenv("RRR3D_EXPERIMENT_SPRING_COLD");
	const float spring_ratio = UsesSourceSpringFinal() && cold_mode != nullptr && std::strcmp(cold_mode, "1") == 0? 0.0f : inWarmStartImpulseRatio;
	for (Wheel *w : mWheels)
		if (w->mContactBody != nullptr)
		{
			Vec3 neg_contact_normal = -w->mContactNormal;

			w->mSuspensionPart.WarmStart(*mBody, *w->mContactBody, neg_contact_normal, spring_ratio);
			w->mSuspensionMaxUpPart.WarmStart(*mBody, *w->mContactBody, neg_contact_normal, stop_ratio);
			w->mLongitudinalPart.WarmStart(*mBody, *w->mContactBody, -w->mContactLongitudinal, 0.0f); // Don't warm start the longitudinal part (the engine/brake force, we don't want to preserve anything from the last frame)
            const char *tire_cold_v57=std::getenv("RRR3D_EXPERIMENT_NATIVE_TIRE_COLD_V57");
            const bool native_tire_cold=UsesSourceSpringFinal()&&tire_cold_v57&&std::strcmp(tire_cold_v57,"1")==0&&w->mContactBody->IsStatic()&&w->mSteerAngle==0;
            w->mLateralPart.WarmStart(*mBody, *w->mContactBody, -w->mContactLateral, native_tire_cold?0.0f:inWarmStartImpulseRatio);
		}

	mPitchRollPart.WarmStart(*mBody, Body::sFixedToWorld, inWarmStartImpulseRatio);
}


// Bounded, opt-in read-only wheel solver telemetry. The default path does no I/O.
static void TraceSourceWheelV62(const Body& body, uint step, uint iteration, int wheel,
                               const char* phase, float lambda=0, float mass=0)
{
    const char* path=std::getenv("RRR3D_WHEEL_SOLVER_TRACE_FILE_V62");
    const char* target=std::getenv("RRR3D_WHEEL_SOLVER_TRACE_STEP_V62");
    if(!path || !*path || !target || step!=std::strtoul(target,nullptr,10))return;
    const Vec3 v=body.GetLinearVelocity(),a=body.GetAngularVelocity();
    FILE* file=std::fopen(path,"a");
    if(!file)return;
    std::fprintf(file,"{\"step\":%u,\"iteration\":%u,\"wheel\":%d,\"phase\":\"%s\",\"lambda\":%.9g,\"mass\":%.9g,\"velocity\":[%.9g,%.9g,%.9g],\"angular\":[%.9g,%.9g,%.9g]}\n",step,iteration,wheel,phase,lambda,mass,v.GetX(),v.GetZ(),v.GetY(),-a.GetX(),-a.GetZ(),-a.GetY());
    std::fclose(file);
}

bool VehicleConstraint::SolveVelocityConstraint(float inDeltaTime)
{
	const bool source = UsesSourceSpringFinal();
	const bool source_stop = UsesSourceSuspensionStop();
	if (source) ++mSourceVelocitySteps;
	bool impulse = false;

    const char* batchedStopsFlag=std::getenv("RRR3D_EXPERIMENT_NATIVE_SUSPENSION_STOP_ORDER_V62");
    const bool batchedStops=source && source_stop && batchedStopsFlag && std::strcmp(batchedStopsFlag,"1")==0;
    // Original Nx submits all active hard suspension stops before the soft
    // spring rows. Interleaving a stop with its spring changes the next stop.
    if(batchedStops)for(uint index=0;index<mWheels.size();++index) {
        Wheel* wheel=mWheels[mWheels.size()-1-index];
        if(wheel->mContactBody && wheel->mSuspensionMaxUpPart.IsActive())
            impulse |= wheel->mSuspensionMaxUpPart.SolveVelocityConstraint(*mBody,*wheel->mContactBody,-wheel->mContactNormal,0.0f,FLT_MAX);
    }
	// Solve suspension
	for (uint index = 0; index < mWheels.size(); ++index)
	{
		Wheel *w = mWheels[source? mWheels.size() - 1 - index : index];
		if (w->mContactBody != nullptr)
		{
			Vec3 neg_contact_normal = -w->mContactNormal;

			if (source_stop && !batchedStops && w->mSuspensionMaxUpPart.IsActive())
				impulse |= w->mSuspensionMaxUpPart.SolveVelocityConstraint(*mBody, *w->mContactBody, neg_contact_normal, 0.0f, FLT_MAX);

            const char *local_lost_flag_v58=std::getenv("RRR3D_EXPERIMENT_NATIVE_LOST_LOCAL_FRAME_V58");
            const bool cached_frame_v58=local_lost_flag_v58&&std::strcmp(local_lost_flag_v58,"1")==0
                && AxisConstraintPart::UsesNativeWheelFrameV57()&&UsesSourceSpringFinal()&&w->mContactBody->IsStatic()
                && w->mSteerAngle==0&&!w->mSettings->mEnableSuspensionForcePoint&&w->mSourceContactLost&&w->mSourceLocalContactValidV58;
            TraceSourceWheelV62(*mBody,GetSourceSimulationCounterV59(),mSourceVelocitySteps,int(mWheels.size()-1-index),"spring-before",w->mSuspensionPart.GetTotalLambda(),w->mSuspensionPart.GetDiagnosticEffectiveMass());
			if (w->mSuspensionPart.IsActive())
                impulse |= w->mNativeSpringV53.enabled? w->mSuspensionPart.SolveNativeSplitSpringV53(*mBody, *w->mContactBody,
                    cached_frame_v58?AxisConstraintPart::NativeWheelLocalAxisV58(*mBody,w->mSourceLocalContactAxisV58):
                    (AxisConstraintPart::UsesNativeWheelFrameV57()&&UsesSourceSpringFinal()&&w->mContactBody->IsStatic()&&w->mSteerAngle==0&&!w->mSettings->mEnableSuspensionForcePoint?AxisConstraintPart::NativeWheelFrameAxisV57(*mBody,neg_contact_normal):neg_contact_normal),
                    w->mNativeSpringV53, 0.0f, FLT_MAX) : w->mSuspensionPart.SolveVelocityConstraint(*mBody, *w->mContactBody, neg_contact_normal, 0.0f, FLT_MAX);

            TraceSourceWheelV62(*mBody,GetSourceSimulationCounterV59(),mSourceVelocitySteps,int(mWheels.size()-1-index),"spring-after",w->mSuspensionPart.GetTotalLambda(),w->mSuspensionPart.GetDiagnosticEffectiveMass());
			if (!source_stop && w->mSuspensionMaxUpPart.IsActive())
				impulse |= w->mSuspensionMaxUpPart.SolveVelocityConstraint(*mBody, *w->mContactBody, neg_contact_normal, 0.0f, FLT_MAX);

		}

		}

	// Solve the horizontal movement of the vehicle
    TraceSourceWheelV62(*mBody,GetSourceSimulationCounterV59(),mSourceVelocitySteps,-1,"tires-before");
	impulse |= mController->SolveLongitudinalAndLateralConstraints(inDeltaTime);
    TraceSourceWheelV62(*mBody,GetSourceSimulationCounterV59(),mSourceVelocitySteps,-1,"tires-after");

	// Apply the pitch / roll constraint to avoid the vehicle from toppling over
	if (mPitchRollPart.IsActive())
		impulse |= mPitchRollPart.SolveVelocityConstraint(*mBody, Body::sFixedToWorld, mPitchRollRotationAxis, 0, FLT_MAX);

	return impulse || source;
}

bool VehicleConstraint::SolvePositionConstraint(float inDeltaTime, float inBaumgarte)
{
	bool impulse = false;

	RMat44 body_transform = mBody->GetWorldTransform();

	for (Wheel *w : mWheels)
		if (w->mContactBody != nullptr)
		{
			const WheelSettings *settings = w->mSettings;

			// Check if we reached the 'max up' position now that the body has possibly moved
			// We do this by calculating the axle position at minimum suspension length and making sure it does not go through the
			// plane defined by the contact normal and the axle position when the contact happened
			// TODO: This assumes that only the vehicle moved and not the ground as we kept the axle contact plane in world space
			Vec3 ws_direction = body_transform.Multiply3x3(settings->mSuspensionDirection);
			RVec3 ws_position = body_transform * settings->mPosition;
			RVec3 min_suspension_pos = ws_position + settings->mSuspensionMinLength * ws_direction;
			float max_up_error = float(RVec3(w->mContactNormal).Dot(min_suspension_pos) - w->mAxlePlaneConstant);
			if (max_up_error < 0.0f)
			{
				Vec3 neg_contact_normal = -w->mContactNormal;

				// Recalculate constraint properties since the body may have moved
				Vec3 r1_plus_u, r2;
				CalculateSuspensionForcePoint(*w, r1_plus_u, r2);
				w->mSuspensionMaxUpPart.CalculateConstraintProperties(*mBody, r1_plus_u, *w->mContactBody, r2, neg_contact_normal);

				if (!UsesSourceSuspensionStop())
				impulse |= w->mSuspensionMaxUpPart.SolvePositionConstraint(*mBody, *w->mContactBody, neg_contact_normal, max_up_error, inBaumgarte);
			}
		}

	// Apply the pitch / roll constraint to avoid the vehicle from toppling over
	CalculatePitchRollConstraintProperties(body_transform);
	if (mPitchRollPart.IsActive())
		impulse |= mPitchRollPart.SolvePositionConstraint(*mBody, Body::sFixedToWorld, mCosPitchRollAngle - mCosMaxPitchRollAngle, inBaumgarte);

	return impulse;
}

#ifdef JPH_DEBUG_RENDERER

void VehicleConstraint::DrawConstraint(DebugRenderer *inRenderer) const
{
	mController->Draw(inRenderer);
}

void VehicleConstraint::DrawConstraintLimits(DebugRenderer *inRenderer) const
{
}

#endif // JPH_DEBUG_RENDERER

void VehicleConstraint::SaveState(StateRecorder &inStream) const
{
	Constraint::SaveState(inStream);

	mController->SaveState(inStream);

	for (const Wheel *w : mWheels)
	{
		inStream.Write(w->mAngularVelocity);
		inStream.Write(w->mAngle);
		inStream.Write(w->mContactBodyID); // Used by MotorcycleController::PreCollide
		inStream.Write(w->mContactPosition); // Used by VehicleCollisionTester::PredictContactProperties
		inStream.Write(w->mContactNormal); // Used by MotorcycleController::PreCollide
		inStream.Write(w->mContactLateral); // Used by MotorcycleController::PreCollide
		inStream.Write(w->mSuspensionLength); // Used by VehicleCollisionTester::PredictContactProperties

		w->mSuspensionPart.SaveState(inStream);
		w->mSuspensionMaxUpPart.SaveState(inStream);
		w->mLongitudinalPart.SaveState(inStream);
		w->mLateralPart.SaveState(inStream);
	}

	inStream.Write(mPitchRollRotationAxis); // When rotation is too small we use last frame so we need to store it
	mPitchRollPart.SaveState(inStream);
	inStream.Write(mCurrentStep);
}

void VehicleConstraint::RestoreState(StateRecorder &inStream)
{
	Constraint::RestoreState(inStream);

	mController->RestoreState(inStream);

	for (Wheel *w : mWheels)
	{
		inStream.Read(w->mAngularVelocity);
		inStream.Read(w->mAngle);
		inStream.Read(w->mContactBodyID);
		inStream.Read(w->mContactPosition);
		inStream.Read(w->mContactNormal);
		inStream.Read(w->mContactLateral);
		inStream.Read(w->mSuspensionLength);
		w->mContactBody = nullptr; // No longer valid

		w->mSuspensionPart.RestoreState(inStream);
		w->mSuspensionMaxUpPart.RestoreState(inStream);
		w->mLongitudinalPart.RestoreState(inStream);
		w->mLateralPart.RestoreState(inStream);
	}

	inStream.Read(mPitchRollRotationAxis);
	mPitchRollPart.RestoreState(inStream);
	inStream.Read(mCurrentStep);
}

Ref<ConstraintSettings> VehicleConstraint::GetConstraintSettings() const
{
	VehicleConstraintSettings *settings = new VehicleConstraintSettings;
	ToConstraintSettings(*settings);
	settings->mUp = mUp;
	settings->mForward = mForward;
	settings->mMaxPitchRollAngle = ACos(mCosMaxPitchRollAngle);
	settings->mWheels.resize(mWheels.size());
	for (Wheels::size_type w = 0; w < mWheels.size(); ++w)
		settings->mWheels[w] = const_cast<WheelSettings *>(mWheels[w]->mSettings.GetPtr());
	settings->mAntiRollBars = mAntiRollBars;
	settings->mController = mController->GetSettings();
	return settings;
}

JPH_NAMESPACE_END

JPH_NAMESPACE_BEGIN
bool VehicleConstraint::UsesSourceSpringFinal() const
{
 const char *mode = std::getenv("RRR3D_EXPERIMENT_SPRING_FINAL");
 return mode != nullptr && std::strcmp(mode, "1") == 0 &&
  !mWheels.empty() && mWheels[0]->mSettings->mSuspensionSpringBiasFactor == 0.7f;
}
uint VehicleConstraint::GetSourceSimulationCounterV59() const {
 return mCurrentStep-uint32(Hash64(mBody->GetID().GetIndex()));
}
bool VehicleConstraint::PrepareSourceFinalVelocityConstraints(Array<Body *> &outBodies)
{
 if (!UsesSourceSpringFinal()) return false;
 bool contact = false; for (const Wheel *w : mWheels) contact |= w->mContactBody != nullptr;
 if (!contact) return false;
 outBodies.push_back(mBody);
 for (Wheel *w : mWheels)
  if (w->mContactBody != nullptr) {
   if (w->mContactBody->IsDynamic()) outBodies.push_back(w->mContactBody);
   if (w->mSuspensionPart.IsActive()) { if (w->mNativeSpringV53.enabled) w->mSuspensionPart.PrepareNativeFinalSpringV53(w->mNativeSpringV53); else w->mSuspensionPart.PrepareSourceFinalSpringBias(mSourceVelocitySteps); }
   if (UsesSourceSuspensionStop() && w->mSuspensionMaxUpPart.IsActive()) w->mSuspensionMaxUpPart.SetHardVelocityBias(0.0f);
   const char *anchor_v57=std::getenv("RRR3D_EXPERIMENT_NATIVE_WHEEL_ANCHOR_BIAS_V57");
   if(anchor_v57&&std::strcmp(anchor_v57,"1")==0)w->ClearSourceAnchorGeometryV57();
  }
 return true;
}
JPH_NAMESPACE_END

JPH_NAMESPACE_BEGIN
bool VehicleConstraint::UsesSourceSuspensionStop() const {
 const char *mode = std::getenv("RRR3D_EXPERIMENT_SUSPENSION_STOP");
 return UsesSourceSpringFinal() && mode != nullptr && std::strcmp(mode, "1") == 0;
}
JPH_NAMESPACE_END
