// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#pragma once

#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Constraints/ConstraintPart/SpringPart.h>
#include <Jolt/Physics/Constraints/SpringSettings.h>
#include <Jolt/Physics/Constraints/ConstraintPart/NativeSpringSplitV53.h>
#include <Jolt/Physics/Constraints/ConstraintPart/NativeWheelRowV56.h>
#include <Jolt/Physics/Constraints/ConstraintPart/NativeWheelFrameV57.h>
#include <Jolt/Physics/StateRecorder.h>
#include <Jolt/Physics/DeterminismLog.h>

JPH_NAMESPACE_BEGIN

/// Constraint that constrains motion along 1 axis
///
/// @see "Constraints Derivation for Rigid Body Simulation in 3D" - Daniel Chappuis, section 2.1.1
/// (we're not using the approximation of eq 27 but instead add the U term as in eq 55)
///
/// Constraint equation (eq 25):
///
/// \f[C = (p_2 - p_1) \cdot n\f]
///
/// Jacobian (eq 28):
///
/// \f[J = \begin{bmatrix} -n^T & (-(r_1 + u) \times n)^T & n^T & (r_2 \times n)^T \end{bmatrix}\f]
///
/// Used terms (here and below, everything in world space):\n
/// n = constraint axis (normalized).\n
/// p1, p2 = constraint points.\n
/// r1 = p1 - x1.\n
/// r2 = p2 - x2.\n
/// u = x2 + r2 - x1 - r1 = p2 - p1.\n
/// x1, x2 = center of mass for the bodies.\n
/// v = [v1, w1, v2, w2].\n
/// v1, v2 = linear velocity of body 1 and 2.\n
/// w1, w2 = angular velocity of body 1 and 2.\n
/// M = mass matrix, a diagonal matrix of the mass and inertia with diagonal [m1, I1, m2, I2].\n
/// \f$K^{-1} = \left( J M^{-1} J^T \right)^{-1}\f$ = effective mass.\n
/// b = velocity bias.\n
/// \f$\beta\f$ = baumgarte constant.
class AxisConstraintPart
{
	/// Internal helper function to update velocities of bodies after Lagrange multiplier is calculated
	template <EMotionType Type1, EMotionType Type2>
	JPH_INLINE bool				ApplyVelocityStep(MotionProperties *ioMotionProperties1, float inInvMass1, MotionProperties *ioMotionProperties2, float inInvMass2, Vec3Arg inWorldSpaceAxis, float inLambda, bool inPremultipliedLinear = false) const
	{
		// Apply impulse if delta is not zero
		if (inLambda != 0.0f)
		{
			// Calculate velocity change due to constraint
			//
			// Impulse:
			// P = J^T lambda
			//
			// Euler velocity integration:
			// v' = v + M^-1 P
			if constexpr (Type1 == EMotionType::Dynamic)
			{
				ioMotionProperties1->SubLinearVelocityStep((inPremultipliedLinear? (inWorldSpaceAxis * inInvMass1) * inLambda : (inLambda * inInvMass1) * inWorldSpaceAxis));
				ioMotionProperties1->SubAngularVelocityStep(inLambda * Vec3::sLoadFloat3Unsafe(mInvI1_R1PlusUxAxis));
			}
			if constexpr (Type2 == EMotionType::Dynamic)
			{
				ioMotionProperties2->AddLinearVelocityStep((inPremultipliedLinear? (inWorldSpaceAxis * inInvMass2) * inLambda : (inLambda * inInvMass2) * inWorldSpaceAxis));
				ioMotionProperties2->AddAngularVelocityStep(inLambda * Vec3::sLoadFloat3Unsafe(mInvI2_R2xAxis));
			}
			return true;
		}

		return false;
	}

	/// Internal helper function to calculate the inverse effective mass
	template <EMotionType Type1, EMotionType Type2>
	JPH_INLINE float			TemplatedCalculateInverseEffectiveMass(float inInvMass1, Mat44Arg inInvI1, Vec3Arg inR1PlusU, float inInvMass2, Mat44Arg inInvI2, Vec3Arg inR2, Vec3Arg inWorldSpaceAxis, bool inRawAxisMass = false, bool inJacobianMass = false)
	{
		JPH_ASSERT(inWorldSpaceAxis.IsNormalized(1.0e-5f));

		// Calculate properties used below
		Vec3 r1_plus_u_x_axis;
		if constexpr (Type1 != EMotionType::Static)
		{
			r1_plus_u_x_axis = inR1PlusU.Cross(inWorldSpaceAxis);
			r1_plus_u_x_axis.StoreFloat3(&mR1PlusUxAxis);
		}
		else
		{
		#ifdef JPH_DEBUG
			Vec3::sNaN().StoreFloat3(&mR1PlusUxAxis);
		#endif
		}

		Vec3 r2_x_axis;
		if constexpr (Type2 != EMotionType::Static)
		{
			r2_x_axis = inR2.Cross(inWorldSpaceAxis);
			r2_x_axis.StoreFloat3(&mR2xAxis);
		}
		else
		{
		#ifdef JPH_DEBUG
			Vec3::sNaN().StoreFloat3(&mR2xAxis);
		#endif
		}

		// Calculate inverse effective mass: K = J M^-1 J^T
		float inv_effective_mass;

		if constexpr (Type1 == EMotionType::Dynamic)
		{
			Vec3 invi1_r1_plus_u_x_axis = inInvI1.Multiply3x3(r1_plus_u_x_axis);
			invi1_r1_plus_u_x_axis.StoreFloat3(&mInvI1_R1PlusUxAxis);
			inv_effective_mass = (inJacobianMass? inWorldSpaceAxis.Dot(inWorldSpaceAxis * inInvMass1) : (inRawAxisMass? inInvMass1 * inWorldSpaceAxis.LengthSq() : inInvMass1)) + invi1_r1_plus_u_x_axis.Dot(r1_plus_u_x_axis);
		}
		else
		{
			(void)r1_plus_u_x_axis; // Fix compiler warning: Not using this (it's not calculated either)
			JPH_IF_DEBUG(Vec3::sNaN().StoreFloat3(&mInvI1_R1PlusUxAxis);)
			inv_effective_mass = 0.0f;
		}

		if constexpr (Type2 == EMotionType::Dynamic)
		{
			Vec3 invi2_r2_x_axis = inInvI2.Multiply3x3(r2_x_axis);
			invi2_r2_x_axis.StoreFloat3(&mInvI2_R2xAxis);
			inv_effective_mass += (inJacobianMass? inWorldSpaceAxis.Dot(inWorldSpaceAxis * inInvMass2) : (inRawAxisMass? inInvMass2 * inWorldSpaceAxis.LengthSq() : inInvMass2)) + invi2_r2_x_axis.Dot(r2_x_axis);
		}
		else
		{
			(void)r2_x_axis; // Fix compiler warning: Not using this (it's not calculated either)
			JPH_IF_DEBUG(Vec3::sNaN().StoreFloat3(&mInvI2_R2xAxis);)
		}

		return inv_effective_mass;
	}

	/// Internal helper function to calculate the inverse effective mass
	JPH_INLINE float			CalculateInverseEffectiveMass(const Body &inBody1, Vec3Arg inR1PlusU, const Body &inBody2, Vec3Arg inR2, Vec3Arg inWorldSpaceAxis)
	{
		// Dispatch to the correct templated form
		switch (inBody1.GetMotionType())
		{
		case EMotionType::Dynamic:
			{
				const MotionProperties *mp1 = inBody1.GetMotionPropertiesUnchecked();
				float inv_m1 = mp1->GetInverseMass();
				Mat44 inv_i1 = inBody1.GetInverseInertia();
				switch (inBody2.GetMotionType())
				{
				case EMotionType::Dynamic:
					return TemplatedCalculateInverseEffectiveMass<EMotionType::Dynamic, EMotionType::Dynamic>(inv_m1, inv_i1, inR1PlusU, inBody2.GetMotionPropertiesUnchecked()->GetInverseMass(), inBody2.GetInverseInertia(), inR2, inWorldSpaceAxis);

				case EMotionType::Kinematic:
					return TemplatedCalculateInverseEffectiveMass<EMotionType::Dynamic, EMotionType::Kinematic>(inv_m1, inv_i1, inR1PlusU, 0 /* Will not be used */, Mat44() /* Will not be used */, inR2, inWorldSpaceAxis);

				case EMotionType::Static:
					return TemplatedCalculateInverseEffectiveMass<EMotionType::Dynamic, EMotionType::Static>(inv_m1, inv_i1, inR1PlusU, 0 /* Will not be used */, Mat44() /* Will not be used */, inR2, inWorldSpaceAxis);

				default:
					break;
				}
				break;
			}

		case EMotionType::Kinematic:
			JPH_ASSERT(inBody2.IsDynamic());
			return TemplatedCalculateInverseEffectiveMass<EMotionType::Kinematic, EMotionType::Dynamic>(0 /* Will not be used */, Mat44() /* Will not be used */, inR1PlusU, inBody2.GetMotionPropertiesUnchecked()->GetInverseMass(), inBody2.GetInverseInertia(), inR2, inWorldSpaceAxis);

		case EMotionType::Static:
			JPH_ASSERT(inBody2.IsDynamic());
			return TemplatedCalculateInverseEffectiveMass<EMotionType::Static, EMotionType::Dynamic>(0 /* Will not be used */, Mat44() /* Will not be used */, inR1PlusU, inBody2.GetMotionPropertiesUnchecked()->GetInverseMass(), inBody2.GetInverseInertia(), inR2, inWorldSpaceAxis);

		default:
			break;
		}

		JPH_ASSERT(false);
		return 0.0f;
	}

	/// Internal helper function to calculate the inverse effective mass, version that supports mass scaling
	JPH_INLINE float			CalculateInverseEffectiveMassWithMassOverride(const Body &inBody1, float inInvMass1, float inInvInertiaScale1, Vec3Arg inR1PlusU, const Body &inBody2, float inInvMass2, float inInvInertiaScale2, Vec3Arg inR2, Vec3Arg inWorldSpaceAxis)
	{
		// Dispatch to the correct templated form
		switch (inBody1.GetMotionType())
		{
		case EMotionType::Dynamic:
			{
				Mat44 inv_i1 = inInvInertiaScale1 * inBody1.GetInverseInertia();
				switch (inBody2.GetMotionType())
				{
				case EMotionType::Dynamic:
					return TemplatedCalculateInverseEffectiveMass<EMotionType::Dynamic, EMotionType::Dynamic>(inInvMass1, inv_i1, inR1PlusU, inInvMass2, inInvInertiaScale2 * inBody2.GetInverseInertia(), inR2, inWorldSpaceAxis);

				case EMotionType::Kinematic:
					return TemplatedCalculateInverseEffectiveMass<EMotionType::Dynamic, EMotionType::Kinematic>(inInvMass1, inv_i1, inR1PlusU, 0 /* Will not be used */, Mat44() /* Will not be used */, inR2, inWorldSpaceAxis);

				case EMotionType::Static:
					return TemplatedCalculateInverseEffectiveMass<EMotionType::Dynamic, EMotionType::Static>(inInvMass1, inv_i1, inR1PlusU, 0 /* Will not be used */, Mat44() /* Will not be used */, inR2, inWorldSpaceAxis);

				default:
					break;
				}
				break;
			}

		case EMotionType::Kinematic:
			JPH_ASSERT(inBody2.IsDynamic());
			return TemplatedCalculateInverseEffectiveMass<EMotionType::Kinematic, EMotionType::Dynamic>(0 /* Will not be used */, Mat44() /* Will not be used */, inR1PlusU, inInvMass2, inInvInertiaScale2 * inBody2.GetInverseInertia(), inR2, inWorldSpaceAxis);

		case EMotionType::Static:
			JPH_ASSERT(inBody2.IsDynamic());
			return TemplatedCalculateInverseEffectiveMass<EMotionType::Static, EMotionType::Dynamic>(0 /* Will not be used */, Mat44() /* Will not be used */, inR1PlusU, inInvMass2, inInvInertiaScale2 * inBody2.GetInverseInertia(), inR2, inWorldSpaceAxis);

		default:
			break;
		}

		JPH_ASSERT(false);
		return 0.0f;
	}

public:
	void PrepareSourceFinalSpringBias(uint inVelocitySteps) { mSpringPart.PrepareSourceFinalBias(mEffectiveMass, inVelocitySteps); }
	void SetHardVelocityBias(float inBias) { mSpringPart.CalculateSpringPropertiesWithBias(inBias); }

	/// Templated form of CalculateConstraintProperties with the motion types baked in
	template <EMotionType Type1, EMotionType Type2>
	JPH_INLINE void				TemplatedCalculateConstraintProperties(float inInvMass1, Mat44Arg inInvI1, Vec3Arg inR1PlusU, float inInvMass2, Mat44Arg inInvI2, Vec3Arg inR2, Vec3Arg inWorldSpaceAxis, float inBias = 0.0f, bool inRawAxisMass = false, bool inJacobianMass = false)
	{
		float inv_effective_mass = TemplatedCalculateInverseEffectiveMass<Type1, Type2>(inInvMass1, inInvI1, inR1PlusU, inInvMass2, inInvI2, inR2, inWorldSpaceAxis, inRawAxisMass, inJacobianMass);

		if (inv_effective_mass == 0.0f)
			Deactivate();
		else
		{
			mEffectiveMass = 1.0f / inv_effective_mass;
			mSpringPart.CalculateSpringPropertiesWithBias(inBias);
		}

		JPH_DET_LOG("TemplatedCalculateConstraintProperties: invM1: " << inInvMass1 << " invI1: " << inInvI1 << " r1PlusU: " << inR1PlusU << " invM2: " << inInvMass2 << " invI2: " << inInvI2 << " r2: " << inR2 << " bias: " << inBias << " r1PlusUxAxis: " << mR1PlusUxAxis << " r2xAxis: " << mR2xAxis << " invI1_R1PlusUxAxis: " << mInvI1_R1PlusUxAxis << " invI2_R2xAxis: " << mInvI2_R2xAxis << " effectiveMass: " << mEffectiveMass << " totalLambda: " << mTotalLambda);
	}

	/// Calculate properties used during the functions below
	/// @param inBody1 The first body that this constraint is attached to
	/// @param inBody2 The second body that this constraint is attached to
	/// @param inR1PlusU See equations above (r1 + u)
	/// @param inR2 See equations above (r2)
	/// @param inWorldSpaceAxis Axis along which the constraint acts (normalized, pointing from body 1 to 2)
	/// @param inBias Bias term (b) for the constraint impulse: lambda = J v + b
	inline void					CalculateConstraintProperties(const Body &inBody1, Vec3Arg inR1PlusU, const Body &inBody2, Vec3Arg inR2, Vec3Arg inWorldSpaceAxis, float inBias = 0.0f)
	{
		float inv_effective_mass = CalculateInverseEffectiveMass(inBody1, inR1PlusU, inBody2, inR2, inWorldSpaceAxis);

		if (inv_effective_mass == 0.0f)
			Deactivate();
		else
		{
			mEffectiveMass = 1.0f / inv_effective_mass;
			mSpringPart.CalculateSpringPropertiesWithBias(inBias);
		}
	}

	/// Calculate properties used during the functions below, version that supports mass scaling
	/// @param inBody1 The first body that this constraint is attached to
	/// @param inBody2 The second body that this constraint is attached to
	/// @param inInvMass1 The inverse mass of body 1 (only used when body 1 is dynamic)
	/// @param inInvMass2 The inverse mass of body 2 (only used when body 2 is dynamic)
	/// @param inInvInertiaScale1 Scale factor for the inverse inertia of body 1
	/// @param inInvInertiaScale2 Scale factor for the inverse inertia of body 2
	/// @param inR1PlusU See equations above (r1 + u)
	/// @param inR2 See equations above (r2)
	/// @param inWorldSpaceAxis Axis along which the constraint acts (normalized, pointing from body 1 to 2)
	/// @param inBias Bias term (b) for the constraint impulse: lambda = J v + b
	inline void					CalculateConstraintPropertiesWithMassOverride(const Body &inBody1, float inInvMass1, float inInvInertiaScale1, Vec3Arg inR1PlusU, const Body &inBody2, float inInvMass2, float inInvInertiaScale2, Vec3Arg inR2, Vec3Arg inWorldSpaceAxis, float inBias = 0.0f)
	{
		float inv_effective_mass = CalculateInverseEffectiveMassWithMassOverride(inBody1, inInvMass1, inInvInertiaScale1, inR1PlusU, inBody2, inInvMass2, inInvInertiaScale2, inR2, inWorldSpaceAxis);

		if (inv_effective_mass == 0.0f)
			Deactivate();
		else
		{
			mEffectiveMass = 1.0f / inv_effective_mass;
			mSpringPart.CalculateSpringPropertiesWithBias(inBias);
		}
	}

	/// Calculate properties used during the functions below
	/// @param inDeltaTime Time step
	/// @param inBody1 The first body that this constraint is attached to
	/// @param inBody2 The second body that this constraint is attached to
	/// @param inR1PlusU See equations above (r1 + u)
	/// @param inR2 See equations above (r2)
	/// @param inWorldSpaceAxis Axis along which the constraint acts (normalized, pointing from body 1 to 2)
	/// @param inBias Bias term (b) for the constraint impulse: lambda = J v + b
	///	@param inC Value of the constraint equation (C).
	///	@param inFrequency Oscillation frequency (Hz).
	///	@param inDamping Damping factor (0 = no damping, 1 = critical damping).
	inline void					CalculateConstraintPropertiesWithFrequencyAndDamping(float inDeltaTime, const Body &inBody1, Vec3Arg inR1PlusU, const Body &inBody2, Vec3Arg inR2, Vec3Arg inWorldSpaceAxis, float inBias, float inC, float inFrequency, float inDamping)
	{
		float inv_effective_mass = CalculateInverseEffectiveMass(inBody1, inR1PlusU, inBody2, inR2, inWorldSpaceAxis);

		if (inv_effective_mass == 0.0f)
			Deactivate();
		else
			mSpringPart.CalculateSpringPropertiesWithFrequencyAndDamping(inDeltaTime, inv_effective_mass, inBias, inC, inFrequency, inDamping, mEffectiveMass);
	}

	/// Calculate properties used during the functions below
	/// @param inDeltaTime Time step
	/// @param inBody1 The first body that this constraint is attached to
	/// @param inBody2 The second body that this constraint is attached to
	/// @param inR1PlusU See equations above (r1 + u)
	/// @param inR2 See equations above (r2)
	/// @param inWorldSpaceAxis Axis along which the constraint acts (normalized, pointing from body 1 to 2)
	/// @param inBias Bias term (b) for the constraint impulse: lambda = J v + b
	///	@param inC Value of the constraint equation (C).
	///	@param inStiffness Spring stiffness k.
	///	@param inDamping Spring damping coefficient c.
	inline void					CalculateConstraintPropertiesWithStiffnessAndDamping(float inDeltaTime, const Body &inBody1, Vec3Arg inR1PlusU, const Body &inBody2, Vec3Arg inR2, Vec3Arg inWorldSpaceAxis, float inBias, float inC, float inStiffness, float inDamping)
	{
		float inv_effective_mass = CalculateInverseEffectiveMass(inBody1, inR1PlusU, inBody2, inR2, inWorldSpaceAxis);

		if (inv_effective_mass == 0.0f)
			Deactivate();
		else
			mSpringPart.CalculateSpringPropertiesWithStiffnessAndDamping(inDeltaTime, inv_effective_mass, inBias, inC, inStiffness, inDamping, mEffectiveMass);
	}

	/// Selects one of the above functions based on the spring settings
	inline void					CalculateConstraintPropertiesWithSettings(float inDeltaTime, const Body &inBody1, Vec3Arg inR1PlusU, const Body &inBody2, Vec3Arg inR2, Vec3Arg inWorldSpaceAxis, float inBias, float inC, const SpringSettings &inSpringSettings)
	{
		float inv_effective_mass = CalculateInverseEffectiveMass(inBody1, inR1PlusU, inBody2, inR2, inWorldSpaceAxis);

		if (inv_effective_mass == 0.0f)
			Deactivate();
		else if (inSpringSettings.mMode == ESpringMode::FrequencyAndDamping)
			mSpringPart.CalculateSpringPropertiesWithFrequencyAndDamping(inDeltaTime, inv_effective_mass, inBias, inC, inSpringSettings.mFrequency, inSpringSettings.mDamping, mEffectiveMass);
		else
			mSpringPart.CalculateSpringPropertiesWithStiffnessAndDamping(inDeltaTime, inv_effective_mass, inBias, inC, inSpringSettings.mStiffness, inSpringSettings.mDamping, mEffectiveMass);
	}

	/// Deactivate this constraint
	inline void					Deactivate()
	{
		mEffectiveMass = 0.0f;
		mTotalLambda = 0.0f;
	}

	/// Check if constraint is active
	inline bool					IsActive() const
	{
		return mEffectiveMass != 0.0f;
	}

	/// Templated form of WarmStart with the motion types baked in
	template <EMotionType Type1, EMotionType Type2>
	inline void					TemplatedWarmStart(MotionProperties *ioMotionProperties1, float inInvMass1, MotionProperties *ioMotionProperties2, float inInvMass2, Vec3Arg inWorldSpaceAxis, float inWarmStartImpulseRatio)
	{
		mTotalLambda *= inWarmStartImpulseRatio;

		ApplyVelocityStep<Type1, Type2>(ioMotionProperties1, inInvMass1, ioMotionProperties2, inInvMass2, inWorldSpaceAxis, mTotalLambda);
	}

	/// Must be called from the WarmStartVelocityConstraint call to apply the previous frame's impulses
	/// @param ioBody1 The first body that this constraint is attached to
	/// @param ioBody2 The second body that this constraint is attached to
	/// @param inWorldSpaceAxis Axis along which the constraint acts (normalized)
	/// @param inWarmStartImpulseRatio Ratio of new step to old time step (dt_new / dt_old) for scaling the lagrange multiplier of the previous frame
	inline void					WarmStart(Body &ioBody1, Body &ioBody2, Vec3Arg inWorldSpaceAxis, float inWarmStartImpulseRatio)
	{
		EMotionType motion_type1 = ioBody1.GetMotionType();
		MotionProperties *motion_properties1 = ioBody1.GetMotionPropertiesUnchecked();

		EMotionType motion_type2 = ioBody2.GetMotionType();
		MotionProperties *motion_properties2 = ioBody2.GetMotionPropertiesUnchecked();

		// Dispatch to the correct templated form
		// Note: Warm starting doesn't differentiate between kinematic/static bodies so we handle both as static bodies
		if (motion_type1 == EMotionType::Dynamic)
		{
			if (motion_type2 == EMotionType::Dynamic)
				TemplatedWarmStart<EMotionType::Dynamic, EMotionType::Dynamic>(motion_properties1, motion_properties1->GetInverseMass(), motion_properties2, motion_properties2->GetInverseMass(), inWorldSpaceAxis, inWarmStartImpulseRatio);
			else
				TemplatedWarmStart<EMotionType::Dynamic, EMotionType::Static>(motion_properties1, motion_properties1->GetInverseMass(), motion_properties2, 0.0f /* Unused */, inWorldSpaceAxis, inWarmStartImpulseRatio);
		}
		else
		{
			JPH_ASSERT(motion_type2 == EMotionType::Dynamic);
			TemplatedWarmStart<EMotionType::Static, EMotionType::Dynamic>(motion_properties1, 0.0f /* Unused */, motion_properties2, motion_properties2->GetInverseMass(), inWorldSpaceAxis, inWarmStartImpulseRatio);
		}
	}

	/// Templated form of SolveVelocityConstraint with the motion types baked in, part 1: get the total lambda
	template <EMotionType Type1, EMotionType Type2>
	JPH_INLINE float			TemplatedSolveVelocityConstraintGetTotalLambda(const MotionProperties *ioMotionProperties1, const MotionProperties *ioMotionProperties2, Vec3Arg inWorldSpaceAxis, bool inSplitBias = false) const
	{
		// Calculate jacobian multiplied by linear velocity
		float jv;
		if constexpr (Type1 != EMotionType::Static && Type2 != EMotionType::Static)
			jv = inWorldSpaceAxis.Dot(ioMotionProperties1->GetLinearVelocity() - ioMotionProperties2->GetLinearVelocity());
		else if constexpr (Type1 != EMotionType::Static)
			jv = inWorldSpaceAxis.Dot(ioMotionProperties1->GetLinearVelocity());
		else if constexpr (Type2 != EMotionType::Static)
			jv = inWorldSpaceAxis.Dot(-ioMotionProperties2->GetLinearVelocity());
		else
			JPH_ASSERT(false); // Static vs static is nonsensical!

		// Calculate jacobian multiplied by angular velocity
		if constexpr (Type1 != EMotionType::Static)
			jv += Vec3::sLoadFloat3Unsafe(mR1PlusUxAxis).Dot(ioMotionProperties1->GetAngularVelocity());
		if constexpr (Type2 != EMotionType::Static)
			jv -= Vec3::sLoadFloat3Unsafe(mR2xAxis).Dot(ioMotionProperties2->GetAngularVelocity());

		// Lagrange multiplier is:
		//
		// lambda = -K^-1 (J v + b)
		float lambda;
		if (inSplitBias) {
			const volatile float scaled_velocity = mEffectiveMass * jv;
			const volatile float scaled_bias = mEffectiveMass * mSpringPart.GetBias(mTotalLambda);
			lambda = scaled_velocity - scaled_bias;
		} else lambda = mEffectiveMass * (jv - mSpringPart.GetBias(mTotalLambda));

		// Return the total accumulated lambda
		return mTotalLambda + lambda;
	}

	/// Templated form of SolveVelocityConstraint with the motion types baked in, part 2: apply new lambda
	template <EMotionType Type1, EMotionType Type2>
	JPH_INLINE bool				TemplatedSolveVelocityConstraintApplyLambda(MotionProperties *ioMotionProperties1, float inInvMass1, MotionProperties *ioMotionProperties2, float inInvMass2, Vec3Arg inWorldSpaceAxis, float inTotalLambda, bool inPremultipliedLinear = false)
	{
		float delta_lambda = inTotalLambda - mTotalLambda; // Calculate change in lambda
		mTotalLambda = inTotalLambda; // Store accumulated impulse

		return ApplyVelocityStep<Type1, Type2>(ioMotionProperties1, inInvMass1, ioMotionProperties2, inInvMass2, inWorldSpaceAxis, delta_lambda, inPremultipliedLinear);
	}

	/// Templated form of SolveVelocityConstraint with the motion types baked in
	template <EMotionType Type1, EMotionType Type2>
	inline bool					TemplatedSolveVelocityConstraint(MotionProperties *ioMotionProperties1, float inInvMass1, MotionProperties *ioMotionProperties2, float inInvMass2, Vec3Arg inWorldSpaceAxis, float inMinLambda, float inMaxLambda)
	{
		float total_lambda = TemplatedSolveVelocityConstraintGetTotalLambda<Type1, Type2>(ioMotionProperties1, ioMotionProperties2, inWorldSpaceAxis);

		// Clamp impulse to specified range
		total_lambda = Clamp(total_lambda, inMinLambda, inMaxLambda);

		return TemplatedSolveVelocityConstraintApplyLambda<Type1, Type2>(ioMotionProperties1, inInvMass1, ioMotionProperties2, inInvMass2, inWorldSpaceAxis, total_lambda);
	}

	/// Iteratively update the velocity constraint. Makes sure d/dt C(...) = 0, where C is the constraint equation.
	/// @param ioBody1 The first body that this constraint is attached to
	/// @param ioBody2 The second body that this constraint is attached to
	/// @param inWorldSpaceAxis Axis along which the constraint acts (normalized)
	/// @param inMinLambda Minimum value of constraint impulse to apply (N s)
	/// @param inMaxLambda Maximum value of constraint impulse to apply (N s)
	inline bool					SolveVelocityConstraint(Body &ioBody1, Body &ioBody2, Vec3Arg inWorldSpaceAxis, float inMinLambda, float inMaxLambda)
	{
		EMotionType motion_type1 = ioBody1.GetMotionType();
		MotionProperties *motion_properties1 = ioBody1.GetMotionPropertiesUnchecked();

		EMotionType motion_type2 = ioBody2.GetMotionType();
		MotionProperties *motion_properties2 = ioBody2.GetMotionPropertiesUnchecked();

		// Dispatch to the correct templated form
		switch (motion_type1)
		{
		case EMotionType::Dynamic:
			switch (motion_type2)
			{
			case EMotionType::Dynamic:
				return TemplatedSolveVelocityConstraint<EMotionType::Dynamic, EMotionType::Dynamic>(motion_properties1, motion_properties1->GetInverseMass(), motion_properties2, motion_properties2->GetInverseMass(), inWorldSpaceAxis, inMinLambda, inMaxLambda);

			case EMotionType::Kinematic:
				return TemplatedSolveVelocityConstraint<EMotionType::Dynamic, EMotionType::Kinematic>(motion_properties1, motion_properties1->GetInverseMass(), motion_properties2, 0.0f /* Unused */, inWorldSpaceAxis, inMinLambda, inMaxLambda);

			case EMotionType::Static:
				return TemplatedSolveVelocityConstraint<EMotionType::Dynamic, EMotionType::Static>(motion_properties1, motion_properties1->GetInverseMass(), motion_properties2, 0.0f /* Unused */, inWorldSpaceAxis, inMinLambda, inMaxLambda);

			default:
				JPH_ASSERT(false);
				break;
			}
			break;

		case EMotionType::Kinematic:
			JPH_ASSERT(motion_type2 == EMotionType::Dynamic);
			return TemplatedSolveVelocityConstraint<EMotionType::Kinematic, EMotionType::Dynamic>(motion_properties1, 0.0f /* Unused */, motion_properties2, motion_properties2->GetInverseMass(), inWorldSpaceAxis, inMinLambda, inMaxLambda);

		case EMotionType::Static:
			JPH_ASSERT(motion_type2 == EMotionType::Dynamic);
			return TemplatedSolveVelocityConstraint<EMotionType::Static, EMotionType::Dynamic>(motion_properties1, 0.0f /* Unused */, motion_properties2, motion_properties2->GetInverseMass(), inWorldSpaceAxis, inMinLambda, inMaxLambda);

		default:
			JPH_ASSERT(false);
			break;
		}

		return false;
	}

	/// Iteratively update the velocity constraint. Makes sure d/dt C(...) = 0, where C is the constraint equation.
	/// @param ioBody1 The first body that this constraint is attached to
	/// @param ioBody2 The second body that this constraint is attached to
	/// @param inInvMass1 The inverse mass of body 1 (only used when body 1 is dynamic)
	/// @param inInvMass2 The inverse mass of body 2 (only used when body 2 is dynamic)
	/// @param inWorldSpaceAxis Axis along which the constraint acts (normalized)
	/// @param inMinLambda Minimum value of constraint impulse to apply (N s)
	/// @param inMaxLambda Maximum value of constraint impulse to apply (N s)
	inline bool					SolveVelocityConstraintWithMassOverride(Body &ioBody1, float inInvMass1, Body &ioBody2, float inInvMass2, Vec3Arg inWorldSpaceAxis, float inMinLambda, float inMaxLambda)
	{
		EMotionType motion_type1 = ioBody1.GetMotionType();
		MotionProperties *motion_properties1 = ioBody1.GetMotionPropertiesUnchecked();

		EMotionType motion_type2 = ioBody2.GetMotionType();
		MotionProperties *motion_properties2 = ioBody2.GetMotionPropertiesUnchecked();

		// Dispatch to the correct templated form
		switch (motion_type1)
		{
		case EMotionType::Dynamic:
			switch (motion_type2)
			{
			case EMotionType::Dynamic:
				return TemplatedSolveVelocityConstraint<EMotionType::Dynamic, EMotionType::Dynamic>(motion_properties1, inInvMass1, motion_properties2, inInvMass2, inWorldSpaceAxis, inMinLambda, inMaxLambda);

			case EMotionType::Kinematic:
				return TemplatedSolveVelocityConstraint<EMotionType::Dynamic, EMotionType::Kinematic>(motion_properties1, inInvMass1, motion_properties2, 0.0f /* Unused */, inWorldSpaceAxis, inMinLambda, inMaxLambda);

			case EMotionType::Static:
				return TemplatedSolveVelocityConstraint<EMotionType::Dynamic, EMotionType::Static>(motion_properties1, inInvMass1, motion_properties2, 0.0f /* Unused */, inWorldSpaceAxis, inMinLambda, inMaxLambda);

			default:
				JPH_ASSERT(false);
				break;
			}
			break;

		case EMotionType::Kinematic:
			JPH_ASSERT(motion_type2 == EMotionType::Dynamic);
			return TemplatedSolveVelocityConstraint<EMotionType::Kinematic, EMotionType::Dynamic>(motion_properties1, 0.0f /* Unused */, motion_properties2, inInvMass2, inWorldSpaceAxis, inMinLambda, inMaxLambda);

		case EMotionType::Static:
			JPH_ASSERT(motion_type2 == EMotionType::Dynamic);
			return TemplatedSolveVelocityConstraint<EMotionType::Static, EMotionType::Dynamic>(motion_properties1, 0.0f /* Unused */, motion_properties2, inInvMass2, inWorldSpaceAxis, inMinLambda, inMaxLambda);

		default:
			JPH_ASSERT(false);
			break;
		}

		return false;
	}

	/// Iteratively update the position constraint. Makes sure C(...) = 0.
	/// @param ioBody1 The first body that this constraint is attached to
	/// @param ioBody2 The second body that this constraint is attached to
	/// @param inWorldSpaceAxis Axis along which the constraint acts (normalized)
	/// @param inC Value of the constraint equation (C)
	/// @param inBaumgarte Baumgarte constant (fraction of the error to correct)
	inline bool					SolvePositionConstraint(Body &ioBody1, Body &ioBody2, Vec3Arg inWorldSpaceAxis, float inC, float inBaumgarte) const
	{
		// Only apply position constraint when the constraint is hard, otherwise the velocity bias will fix the constraint
		if (inC != 0.0f && !mSpringPart.IsActive())
		{
			// Calculate lagrange multiplier (lambda) for Baumgarte stabilization:
			//
			// lambda = -K^-1 * beta / dt * C
			//
			// We should divide by inDeltaTime, but we should multiply by inDeltaTime in the Euler step below so they're cancelled out
			float lambda = -mEffectiveMass * inBaumgarte * inC;

			// Directly integrate velocity change for one time step
			//
			// Euler velocity integration:
			// dv = M^-1 P
			//
			// Impulse:
			// P = J^T lambda
			//
			// Euler position integration:
			// x' = x + dv * dt
			//
			// Note we don't accumulate velocities for the stabilization. This is using the approach described in 'Modeling and
			// Solving Constraints' by Erin Catto presented at GDC 2007. On slide 78 it is suggested to split up the Baumgarte
			// stabilization for positional drift so that it does not actually add to the momentum. We combine an Euler velocity
			// integrate + a position integrate and then discard the velocity change.
			if (ioBody1.IsDynamic())
			{
				ioBody1.SubPositionStep((lambda * ioBody1.GetMotionProperties()->GetInverseMass()) * inWorldSpaceAxis);
				ioBody1.SubRotationStep(lambda * Vec3::sLoadFloat3Unsafe(mInvI1_R1PlusUxAxis));
			}
			if (ioBody2.IsDynamic())
			{
				ioBody2.AddPositionStep((lambda * ioBody2.GetMotionProperties()->GetInverseMass()) * inWorldSpaceAxis);
				ioBody2.AddRotationStep(lambda * Vec3::sLoadFloat3Unsafe(mInvI2_R2xAxis));
			}
			return true;
		}

		return false;
	}

	/// Iteratively update the position constraint. Makes sure C(...) = 0.
	/// @param ioBody1 The first body that this constraint is attached to
	/// @param ioBody2 The second body that this constraint is attached to
	/// @param inInvMass1 The inverse mass of body 1 (only used when body 1 is dynamic)
	/// @param inInvMass2 The inverse mass of body 2 (only used when body 2 is dynamic)
	/// @param inWorldSpaceAxis Axis along which the constraint acts (normalized)
	/// @param inC Value of the constraint equation (C)
	/// @param inBaumgarte Baumgarte constant (fraction of the error to correct)
	inline bool					SolvePositionConstraintWithMassOverride(Body &ioBody1, float inInvMass1, Body &ioBody2, float inInvMass2, Vec3Arg inWorldSpaceAxis, float inC, float inBaumgarte) const
	{
		// Only apply position constraint when the constraint is hard, otherwise the velocity bias will fix the constraint
		if (inC != 0.0f && !mSpringPart.IsActive())
		{
			// Calculate lagrange multiplier (lambda) for Baumgarte stabilization:
			//
			// lambda = -K^-1 * beta / dt * C
			//
			// We should divide by inDeltaTime, but we should multiply by inDeltaTime in the Euler step below so they're cancelled out
			float lambda = -mEffectiveMass * inBaumgarte * inC;

			// Directly integrate velocity change for one time step
			//
			// Euler velocity integration:
			// dv = M^-1 P
			//
			// Impulse:
			// P = J^T lambda
			//
			// Euler position integration:
			// x' = x + dv * dt
			//
			// Note we don't accumulate velocities for the stabilization. This is using the approach described in 'Modeling and
			// Solving Constraints' by Erin Catto presented at GDC 2007. On slide 78 it is suggested to split up the Baumgarte
			// stabilization for positional drift so that it does not actually add to the momentum. We combine an Euler velocity
			// integrate + a position integrate and then discard the velocity change.
			if (ioBody1.IsDynamic())
			{
				ioBody1.SubPositionStep((lambda * inInvMass1) * inWorldSpaceAxis);
				ioBody1.SubRotationStep(lambda * Vec3::sLoadFloat3Unsafe(mInvI1_R1PlusUxAxis));
			}
			if (ioBody2.IsDynamic())
			{
				ioBody2.AddPositionStep((lambda * inInvMass2) * inWorldSpaceAxis);
				ioBody2.AddRotationStep(lambda * Vec3::sLoadFloat3Unsafe(mInvI2_R2xAxis));
			}
			return true;
		}

		return false;
	}

	/// Override total lagrange multiplier, can be used to set the initial value for warm starting
	inline void					SetTotalLambda(float inLambda)
	{
		mTotalLambda = inLambda;
	}

	/// Return lagrange multiplier
	inline float				GetTotalLambda() const
	{
		return mTotalLambda;
	}

	/// Read-only diagnostics of the normal row; does not prepare or solve it.
	// Experimental normal-contact caller only; ordinary constraints never use these setters.
	void SetSourceContactEffectiveMass(float inMass) { mEffectiveMass = inMass; }
	void SetSourceContactLambda(float inLambda) { mTotalLambda = inLambda; }
	template <EMotionType Type1, EMotionType Type2>
	bool ApplySourceContactDelta(MotionProperties *inMotion1, float inInvMass1, MotionProperties *inMotion2, float inInvMass2, Vec3Arg inAxis, float inDelta, bool inPremultipliedLinear) const { return ApplyVelocityStep<Type1,Type2>(inMotion1,inInvMass1,inMotion2,inInvMass2,inAxis,inDelta,inPremultipliedLinear); }


 void TraceNativeSpringBodyV57(const char *event,const Body &body,Vec3Arg axis,float delta) const {
  const char *path=std::getenv("RRR3D_NATIVE_SPRING_BODY_V57_TRACE");if(!path||!*path)return;
  static unsigned sequence=0;if(sequence>=256)return;FILE *fp=std::fopen(path,"a");if(!fp)return;
  Vec3 v=body.GetLinearVelocity(),w=body.GetAngularVelocity();
  std::fprintf(fp,"{\"event\":\"%s\",\"sequence\":%u,\"axis\":[%.9g,%.9g,%.9g],\"linear\":[%.9g,%.9g,%.9g],\"angular\":[%.9g,%.9g,%.9g],\"delta\":%.9g}\n",event,sequence++,axis.GetX(),axis.GetY(),axis.GetZ(),v.GetX(),v.GetY(),v.GetZ(),w.GetX(),w.GetY(),w.GetZ(),delta);std::fclose(fp);
 }
 void TraceNativeSplitV53(const char *event,const motorrock_native_spring_v53::State &s,float jv,float delta) const {
  const char *path=std::getenv("RRR3D_NATIVE_SPRING_V53_TRACE");if(path==nullptr||*path==0)return;
  static unsigned lines=0;if(lines>=256)return;FILE *fp=std::fopen(path,"a");if(fp==nullptr)return;
  Vec3 r=GetDiagnosticR1CrossAxis(), inv=GetDiagnosticInvI1R1CrossAxis();
  std::fprintf(fp,"{\"event\":\"%s\",\"sequence\":%u,\"effective_mass\":%.9g,\"position_term\":%.9g,\"impulse_softness\":%.9g,\"unbiased\":%.9g,\"applied\":%.9g,\"jv\":%.9g,\"delta\":%.9g,\"r_cross_axis\":[%.9g,%.9g,%.9g],\"inv_i_r_cross_axis\":[%.9g,%.9g,%.9g]}\n",event,lines++,mEffectiveMass,s.position_term,s.impulse_softness,s.unbiased,mTotalLambda,jv,delta,r.GetX(),r.GetY(),r.GetZ(),inv.GetX(),inv.GetY(),inv.GetZ());std::fclose(fp);
 }
 static bool UsesNativeWheelFrameV57() {
  const char *flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_WHEEL_ROW_FRAME_V57");
  return flag&&std::strcmp(flag,"1")==0;
 }
 static Vec3 NativeWheelFrameAxisV57(const Body &body,Vec3Arg axis) {
  using namespace motorrock_native_wheel_frame_v57;
  using Vector=motorrock_native_wheel_frame_v57::Vector;
  auto q=body.GetRotation();Quaternion rotation{{-q.GetX(),-q.GetZ(),-q.GetY(),q.GetW()}};
  Vector source{{-axis.GetX(),-axis.GetZ(),-axis.GetY()}};
  auto prepared=body_axis(rotation,source);
  return Vec3(-prepared[0],-prepared[2],-prepared[1]);
 }
 void PrepareNativeWheelFrameV57(const Body &body,Vec3Arg world_point,Vec3Arg axis) {
  using namespace motorrock_native_wheel_frame_v57;
  using Vector=motorrock_native_wheel_frame_v57::Vector;
  auto q=body.GetRotation();Quaternion rotation{{-q.GetX(),-q.GetZ(),-q.GetY(),q.GetW()}};
  Vector source_axis{{-axis.GetX(),-axis.GetZ(),-axis.GetY()}},point{{world_point.GetX(),world_point.GetZ(),world_point.GetY()}};
  auto c=body.GetCenterOfMassPosition();Vector center{{float(c.GetX()),float(c.GetZ()),float(c.GetY())}};
  auto prepared_axis=body_axis(rotation,source_axis),prepared_lever=body_lever(rotation,point,center);
  auto jacobian=cross(prepared_lever,prepared_axis);
  Vec3(jacobian[0],jacobian[2],jacobian[1]).StoreFloat3(&mR1PlusUxAxis);
 }
 bool SolveNativeWheelFrameV57(Body &body1,Body &body2,Vec3Arg axis,float minimum,float maximum) {
  JPH_ASSERT(body1.IsDynamic()&&body2.IsStatic());
  return SolveVelocityConstraint(body1,body2,NativeWheelFrameAxisV57(body1,axis),minimum,maximum);
 }
 static Vec3 NativeWheelLocalAxisV58(const Body &body, Vec3Arg local_axis) {
  namespace frame=motorrock_native_wheel_frame_v57;
  auto q=body.GetRotation();frame::Quaternion rotation{{-q.GetX(),-q.GetZ(),-q.GetY(),q.GetW()}};
  auto axis=frame::forward(rotation,{{local_axis.GetX(),local_axis.GetY(),local_axis.GetZ()}},true);
  return Vec3(-axis[0],-axis[2],-axis[1]);
 }
 void PrepareNativeWheelLocalFrameV58(const Body &body, Vec3Arg local_point, Vec3Arg local_axis) {
  namespace frame=motorrock_native_wheel_frame_v57;
  auto q=body.GetRotation();frame::Quaternion rotation{{-q.GetX(),-q.GetZ(),-q.GetY(),q.GetW()}};
  auto axis=frame::forward(rotation,{{local_axis.GetX(),local_axis.GetY(),local_axis.GetZ()}},true);
  auto lever=frame::forward(rotation,{{local_point.GetX(),local_point.GetY(),local_point.GetZ()}},false);
  auto jacobian=frame::cross(lever,axis);
  Vec3(jacobian[0],jacobian[2],jacobian[1]).StoreFloat3(&mR1PlusUxAxis);
 }
 float GetDiagnosticGeometryBiasV57() const { return mSpringPart.GetDiagnosticGeometricBiasV53(); }
 float GetDiagnosticConstantBiasV57() const { return mSpringPart.GetDiagnosticConstantBiasV53(); }
 void PrepareNativeWheelRowInertiaV56(const Body &body1,const Body &body2,Vec3Arg axis,bool hard) {
  using namespace motorrock_native_wheel_row_v56;
  auto vector=[](Vec3Arg v){return motorrock_native_wheel_row_v56::Vector{{v.GetX(),v.GetZ(),v.GetY()}};};
  auto prepare=[&](const Body &body,Vec3Arg jacobian,Float3 &output){
   auto m=body.GetInverseInertia();motorrock_native_wheel_row_v56::Matrix source;const unsigned permutation[3]={0,2,1};
   for(unsigned r=0;r<3;++r)for(unsigned c=0;c<3;++c)source[r*3+c]=m.GetColumn3(permutation[c])[permutation[r]];
   auto product=multiply(source,vector(jacobian));Vec3(product[0],product[2],product[1]).StoreFloat3(&output);
   return inverse_mass(vector(axis),vector(jacobian),product,body.GetMotionPropertiesUnchecked()->GetInverseMass());
  };
  float k=prepare(body1,GetDiagnosticR1CrossAxis(),mInvI1_R1PlusUxAxis);
  if(body2.IsDynamic())k=add(k,prepare(body2,GetDiagnosticR2CrossAxis(),mInvI2_R2xAxis));
  if(hard&&k>0.0f)mEffectiveMass=div(1.0f,k);
 }
 template <EMotionType Type2>
 bool TemplatedSolveNativeWheelAnchorV57(Body &body1,Body &body2,Vec3 axis,float minimum,float maximum,bool use_frame) {
  if constexpr(Type2==EMotionType::Static)if(use_frame)axis=NativeWheelFrameAxisV57(body1,axis);
  using namespace motorrock_native_spring_v53;
  auto *mp1=body1.GetMotionPropertiesUnchecked();
  auto *mp2=Type2!=EMotionType::Static?body2.GetMotionPropertiesUnchecked():nullptr;
  float jv=axis.Dot(mp1->GetLinearVelocity());
  if constexpr(Type2!=EMotionType::Static)jv=axis.Dot(mp1->GetLinearVelocity()-mp2->GetLinearVelocity());
  jv+=GetDiagnosticR1CrossAxis().Dot(mp1->GetAngularVelocity());
  if constexpr(Type2!=EMotionType::Static)jv-=GetDiagnosticR2CrossAxis().Dot(mp2->GetAngularVelocity());
  // Original scalar solver interleaves linear/angular terms. Two Dot sums
  // round differently on cancellation even with identical prepared rows.
  const char *nativeTireJvV85=std::getenv("RRR3D_EXPERIMENT_NATIVE_TIRE_JV_V85");
  if constexpr(Type2==EMotionType::Static)if(nativeTireJvV85 && std::strcmp(nativeTireJvV85,"1")==0) {
   auto source=[](Vec3Arg value,float *out){out[0]=value.GetX();out[1]=value.GetZ();out[2]=value.GetY();};
   float n[3],r[3],v[3],w[3];source(axis,n);source(GetDiagnosticR1CrossAxis(),r);
   source(mp1->GetLinearVelocity(),v);source(mp1->GetAngularVelocity(),w);
   jv=body0_jv(n,r,v,w);
  }
  float velocity=mul(add(mSpringPart.GetDiagnosticConstantBiasV53(),jv),mEffectiveMass);
  float position=mul(mSpringPart.GetDiagnosticGeometricBiasV53(),mul(0.7f,mEffectiveMass));
  const char *nativeTireStepV85=std::getenv("RRR3D_EXPERIMENT_NATIVE_TIRE_STEP_V85");
  if constexpr(Type2==EMotionType::Static)if(nativeTireStepV85 && std::strcmp(nativeTireStepV85,"1")==0) {
   // Preserve the original unclamped delta. Only a clipped bound replaces it
   // with bound - oldLambda; rounded newLambda - oldLambda is not equivalent.
   const auto result=solve(velocity,position,0.0f,0.0f,mTotalLambda,1.0f,minimum,maximum);
   mTotalLambda=result.biased;
   return ApplyVelocityStep<EMotionType::Dynamic,Type2>(mp1,mp1->GetInverseMass(),mp2,0,axis,result.delta);
  }
  float total=add(mTotalLambda,sub(velocity,position));if(total>maximum)total=maximum;else if(minimum>total)total=minimum;
  return TemplatedSolveVelocityConstraintApplyLambda<EMotionType::Dynamic,Type2>(mp1,mp1->GetInverseMass(),mp2,0,axis,total);
 }
 void SetNativeWheelAnchorV57(float velocity,float geometry) { mSpringPart.SetNativeWheelAnchorV57(velocity,geometry); }
 void ClearNativeWheelAnchorGeometryV57() { mSpringPart.ClearNativeWheelAnchorGeometryV57(); }
 bool SolveNativeWheelAnchorV57(Body &body1,Body &body2,Vec3Arg axis,float minimum,float maximum,bool use_frame=false) {
  JPH_ASSERT(body1.IsDynamic()&&body2.IsStatic());
  return IsActive()?TemplatedSolveNativeWheelAnchorV57<EMotionType::Static>(body1,body2,axis,minimum,maximum,use_frame):false;
 }
 void PrepareNativeSplitSpringV53(motorrock_native_spring_v53::State &s) const {
  s.enabled=true; s.native_jv=false; s.unbiased=0.0f;
  s.impulse_softness=motorrock_native_spring_v53::mul(mSpringPart.GetDiagnosticSoftnessV53(),mEffectiveMass);
  s.position_term=motorrock_native_spring_v53::mul(mSpringPart.GetDiagnosticGeometricBiasV53(),mEffectiveMass);
  s.velocity_bias=motorrock_native_spring_v53::sub(mSpringPart.GetDiagnosticConstantBiasV53(),mSpringPart.GetDiagnosticGeometricBiasV53());
  TraceNativeSplitV53("prepare",s,0.0f,0.0f);
 }

 void PrepareNativeSpringCoefficientsV53(const Body &b1,const Body &b2,Vec3Arg axis,float dt,float raw_error,float stiffness,float damping,float bias_factor,motorrock_native_spring_v53::State &s) {
  using namespace motorrock_native_spring_v53;
  auto inverse=[&](const Body &b,Vec3Arg r,Vec3Arg inv) {
   float norm=add(add(mul(axis.GetX(),axis.GetX()),mul(axis.GetY(),axis.GetY())),mul(axis.GetZ(),axis.GetZ()));
   float k=mul(norm,b.GetMotionPropertiesUnchecked()->GetInverseMass());
   k=add(k,mul(r.GetX(),inv.GetX()));k=add(k,mul(r.GetY(),inv.GetY()));return add(k,mul(r.GetZ(),inv.GetZ()));
  };
  float k=inverse(b1,GetDiagnosticR1CrossAxis(),GetDiagnosticInvI1R1CrossAxis());
  if(b2.IsDynamic()) k=add(k,inverse(b2,GetDiagnosticR2CrossAxis(),GetDiagnosticInvI2R2CrossAxis()));
  if(!(k>0.0f))return;
  auto coefficients=prepare(k,dt,stiffness,damping,bias_factor);
  mEffectiveMass=coefficients.mass;s.impulse_softness=coefficients.softness;
  s.position_term=mul(mul(raw_error,div(1.0f,dt)),coefficients.position_coefficient);s.native_jv=true;
  TraceNativeSplitV53("native-prepare",s,0.0f,0.0f);
 }
 void PrepareNativeFinalSpringV53(motorrock_native_spring_v53::State &s) const {
  s.impulse_softness=motorrock_native_spring_v53::final_softness(s.impulse_softness,s.unbiased,mTotalLambda);
  s.position_term=0.0f;
  TraceNativeSplitV53("final",s,0.0f,0.0f);
 }
 template <EMotionType Type2>
 bool TemplatedSolveNativeSplitSpringV53(Body &body1, Body &body2, Vec3 axis, motorrock_native_spring_v53::State &s, float minimum, float maximum) {
  MotionProperties *mp1=body1.GetMotionPropertiesUnchecked();
  MotionProperties *mp2=Type2!=EMotionType::Static?body2.GetMotionPropertiesUnchecked():nullptr;
  float jv;
  if constexpr(Type2!=EMotionType::Static) jv=axis.Dot(mp1->GetLinearVelocity()-mp2->GetLinearVelocity());
  else jv=axis.Dot(mp1->GetLinearVelocity());
  jv+=Vec3::sLoadFloat3Unsafe(mR1PlusUxAxis).Dot(mp1->GetAngularVelocity());
  if constexpr(Type2!=EMotionType::Static) jv-=Vec3::sLoadFloat3Unsafe(mR2xAxis).Dot(mp2->GetAngularVelocity());
  if(s.native_jv) {
   auto source=[](Vec3Arg v,float *a){a[0]=v.GetX();a[1]=v.GetZ();a[2]=v.GetY();};
   float n[3],r[3],v[3],w[3];source(axis,n);source(GetDiagnosticR1CrossAxis(),r);source(mp1->GetLinearVelocity(),v);source(mp1->GetAngularVelocity(),w);
   jv=motorrock_native_spring_v53::body0_jv(n,r,v,w);
   if constexpr(Type2!=EMotionType::Static) {
    source(GetDiagnosticR2CrossAxis(),r);source(mp2->GetLinearVelocity(),v);source(mp2->GetAngularVelocity(),w);
    jv=motorrock_native_spring_v53::sub(jv,motorrock_native_spring_v53::body1_jv(n,r,v,w));
   }
  }
  TraceNativeSpringBodyV57("input",body1,axis,0.0f);
  TraceNativeSplitV53("input",s,jv,0.0f);
  float velocity=motorrock_native_spring_v53::mul(motorrock_native_spring_v53::sub(jv,s.velocity_bias),mEffectiveMass);
  auto result=motorrock_native_spring_v53::solve(velocity,s.position_term,s.impulse_softness,s.unbiased,mTotalLambda,1.0f,minimum,maximum);
  s.unbiased=result.unbiased; mTotalLambda=result.biased;
  TraceNativeSplitV53("result",s,jv,result.delta);
  float inv2=0.0f; if constexpr(Type2==EMotionType::Dynamic) inv2=mp2->GetInverseMass();
  bool changed=ApplyVelocityStep<EMotionType::Dynamic,Type2>(mp1,mp1->GetInverseMass(),mp2,inv2,axis,result.delta);
  TraceNativeSpringBodyV57("result",body1,axis,result.delta);return changed;
 }
 bool SolveNativeSplitSpringV53(Body &body1,Body &body2,Vec3Arg axis,motorrock_native_spring_v53::State &s,float minimum,float maximum) {
  JPH_ASSERT(body1.IsDynamic());
  switch(body2.GetMotionType()) {
   case EMotionType::Dynamic:return TemplatedSolveNativeSplitSpringV53<EMotionType::Dynamic>(body1,body2,axis,s,minimum,maximum);
   case EMotionType::Kinematic:return TemplatedSolveNativeSplitSpringV53<EMotionType::Kinematic>(body1,body2,axis,s,minimum,maximum);
   default:return TemplatedSolveNativeSplitSpringV53<EMotionType::Static>(body1,body2,axis,s,minimum,maximum);
  }
 }
	float GetDiagnosticEffectiveMass() const { return mEffectiveMass; }
	float GetDiagnosticVelocityBias() const { return mSpringPart.GetBias(mTotalLambda); }
	Vec3 GetDiagnosticR1CrossAxis() const { return Vec3::sLoadFloat3Unsafe(mR1PlusUxAxis); }
	Vec3 GetDiagnosticR2CrossAxis() const { return Vec3::sLoadFloat3Unsafe(mR2xAxis); }
	Vec3 GetDiagnosticInvI1R1CrossAxis() const { return Vec3::sLoadFloat3Unsafe(mInvI1_R1PlusUxAxis); }
	Vec3 GetDiagnosticInvI2R2CrossAxis() const { return Vec3::sLoadFloat3Unsafe(mInvI2_R2xAxis); }

	/// Save state of this constraint part
	void						SaveState(StateRecorder &inStream) const
	{
		inStream.Write(mTotalLambda);
	}

	/// Restore state of this constraint part
	void						RestoreState(StateRecorder &inStream)
	{
		inStream.Read(mTotalLambda);
	}

private:
	Float3						mR1PlusUxAxis;
	Float3						mR2xAxis;
	Float3						mInvI1_R1PlusUxAxis;
	Float3						mInvI2_R2xAxis;
	float						mEffectiveMass = 0.0f;
	SpringPart					mSpringPart;
	float						mTotalLambda = 0.0f;
};

JPH_NAMESPACE_END
