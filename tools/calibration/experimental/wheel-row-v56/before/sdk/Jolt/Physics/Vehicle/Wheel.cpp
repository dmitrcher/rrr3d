// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Vehicle/Wheel.h>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>
#include <Jolt/ObjectStream/TypeDeclarations.h>
#include <cstdio>
#include <cstdlib>

JPH_NAMESPACE_BEGIN

JPH_IMPLEMENT_SERIALIZABLE_VIRTUAL(WheelSettings)
{
	JPH_ADD_ATTRIBUTE(WheelSettings, mSuspensionForcePoint)
	JPH_ADD_ATTRIBUTE(WheelSettings, mPosition)
	JPH_ADD_ATTRIBUTE(WheelSettings, mSuspensionDirection)
	JPH_ADD_ATTRIBUTE(WheelSettings, mSteeringAxis)
	JPH_ADD_ATTRIBUTE(WheelSettings, mWheelForward)
	JPH_ADD_ATTRIBUTE(WheelSettings, mWheelUp)
	JPH_ADD_ATTRIBUTE(WheelSettings, mSuspensionMinLength)
	JPH_ADD_ATTRIBUTE(WheelSettings, mSuspensionMaxLength)
	JPH_ADD_ATTRIBUTE(WheelSettings, mSuspensionPreloadLength)
	JPH_ADD_ATTRIBUTE(WheelSettings, mSuspensionSpringBiasFactor)
	JPH_ADD_ENUM_ATTRIBUTE_WITH_ALIAS(WheelSettings, mSuspensionSpring.mMode, "mSuspensionSpringMode")
	JPH_ADD_ATTRIBUTE_WITH_ALIAS(WheelSettings, mSuspensionSpring.mFrequency, "mSuspensionFrequency") // Renaming attributes to stay compatible with old versions of the library
	JPH_ADD_ATTRIBUTE_WITH_ALIAS(WheelSettings, mSuspensionSpring.mDamping, "mSuspensionDamping")
	JPH_ADD_ATTRIBUTE(WheelSettings, mRadius)
	JPH_ADD_ATTRIBUTE(WheelSettings, mWidth)
	JPH_ADD_ATTRIBUTE(WheelSettings, mEnableSuspensionForcePoint)
}

void WheelSettings::SaveBinaryState(StreamOut &inStream) const
{
	inStream.Write(mSuspensionForcePoint);
	inStream.Write(mPosition);
	inStream.Write(mSuspensionDirection);
	inStream.Write(mSteeringAxis);
	inStream.Write(mWheelForward);
	inStream.Write(mWheelUp);
	inStream.Write(mSuspensionMinLength);
	inStream.Write(mSuspensionMaxLength);
	inStream.Write(mSuspensionPreloadLength);
	inStream.Write(mSuspensionSpringBiasFactor);
	mSuspensionSpring.SaveBinaryState(inStream);
	inStream.Write(mRadius);
	inStream.Write(mWidth);
	inStream.Write(mEnableSuspensionForcePoint);
}

void WheelSettings::RestoreBinaryState(StreamIn &inStream)
{
	inStream.Read(mSuspensionForcePoint);
	inStream.Read(mPosition);
	inStream.Read(mSuspensionDirection);
	inStream.Read(mSteeringAxis);
	inStream.Read(mWheelForward);
	inStream.Read(mWheelUp);
	inStream.Read(mSuspensionMinLength);
	inStream.Read(mSuspensionMaxLength);
	inStream.Read(mSuspensionPreloadLength);
	inStream.Read(mSuspensionSpringBiasFactor);
	mSuspensionSpring.RestoreBinaryState(inStream);
	inStream.Read(mRadius);
	inStream.Read(mWidth);
	inStream.Read(mEnableSuspensionForcePoint);
}

Wheel::Wheel(const WheelSettings &inSettings) :
	mSettings(&inSettings),
	mSuspensionLength(inSettings.mSuspensionMaxLength)
{
	JPH_ASSERT(inSettings.mSuspensionDirection.IsNormalized());
	JPH_ASSERT(inSettings.mSteeringAxis.IsNormalized());
	JPH_ASSERT(inSettings.mWheelForward.IsNormalized());
	JPH_ASSERT(inSettings.mWheelUp.IsNormalized());
	JPH_ASSERT(inSettings.mSuspensionMinLength >= 0.0f);
	JPH_ASSERT(inSettings.mSuspensionMaxLength >= inSettings.mSuspensionMinLength);
	JPH_ASSERT(inSettings.mSuspensionMaxLength + inSettings.mSuspensionPreloadLength >= inSettings.mSuspensionMinLength); // signed-rest-length support
	JPH_ASSERT(inSettings.mSuspensionSpring.mMode == ESpringMode::FrequencyAndDamping? inSettings.mSuspensionSpring.mFrequency > 0.0f : inSettings.mSuspensionSpring.mStiffness >= 0.0f); // source zero-stiffness suspension support
	JPH_ASSERT(inSettings.mSuspensionSpring.mDamping >= 0.0f);
	JPH_ASSERT(inSettings.mRadius > 0.0f);
	JPH_ASSERT(inSettings.mWidth >= 0.0f);
}

static void TraceTireV55(const char* event,const AxisConstraintPart &part,const Body &body,Vec3Arg axis,Vec3Arg position,float minimum,float maximum) {
 const char* path=std::getenv("RRR3D_NATIVE_TIRE_V55_TRACE");if(!path||!*path)return;
 static unsigned sequence=0;if(sequence>=256)return;FILE* fp=std::fopen(path,"a");if(!fp)return;
 Vec3 r=part.GetDiagnosticR1CrossAxis(),inv=part.GetDiagnosticInvI1R1CrossAxis();Vec3 v=body.GetLinearVelocity(),w=body.GetAngularVelocity();
 std::fprintf(fp,"{\"event\":\"%s\",\"sequence\":%u,\"mass\":%.9g,\"bias\":%.9g,\"lambda\":%.9g,\"minimum\":%.9g,\"maximum\":%.9g,\"axis\":[%.9g,%.9g,%.9g],\"position\":[%.9g,%.9g,%.9g],\"r_cross_axis\":[%.9g,%.9g,%.9g],\"inv_i_r_cross_axis\":[%.9g,%.9g,%.9g],\"linear\":[%.9g,%.9g,%.9g],\"angular\":[%.9g,%.9g,%.9g]}\n",event,sequence++,part.GetDiagnosticEffectiveMass(),part.GetDiagnosticVelocityBias(),part.GetTotalLambda(),minimum,maximum,axis.GetX(),axis.GetY(),axis.GetZ(),position.GetX(),position.GetY(),position.GetZ(),r.GetX(),r.GetY(),r.GetZ(),inv.GetX(),inv.GetY(),inv.GetZ(),v.GetX(),v.GetY(),v.GetZ(),w.GetX(),w.GetY(),w.GetZ());std::fclose(fp);
}

bool Wheel::SolveLongitudinalConstraintPart(const VehicleConstraint &inConstraint, float inMinImpulse, float inMaxImpulse)
{
	TraceTireV55("longitudinal-input",mLongitudinalPart,*inConstraint.GetVehicleBody(),-mContactLongitudinal,mContactPosition,inMinImpulse,inMaxImpulse);
	const bool changed=mLongitudinalPart.SolveVelocityConstraint(*inConstraint.GetVehicleBody(), *mContactBody, -mContactLongitudinal, inMinImpulse, inMaxImpulse);
	TraceTireV55("longitudinal-result",mLongitudinalPart,*inConstraint.GetVehicleBody(),-mContactLongitudinal,mContactPosition,inMinImpulse,inMaxImpulse);return changed;
}

bool Wheel::SolveLateralConstraintPart(const VehicleConstraint &inConstraint, float inMinImpulse, float inMaxImpulse)
{
	TraceTireV55("lateral-input",mLateralPart,*inConstraint.GetVehicleBody(),-mContactLateral,mContactPosition,inMinImpulse,inMaxImpulse);
	const bool changed=mLateralPart.SolveVelocityConstraint(*inConstraint.GetVehicleBody(), *mContactBody, -mContactLateral, inMinImpulse, inMaxImpulse);
	TraceTireV55("lateral-result",mLateralPart,*inConstraint.GetVehicleBody(),-mContactLateral,mContactPosition,inMinImpulse,inMaxImpulse);return changed;
}

JPH_NAMESPACE_END
