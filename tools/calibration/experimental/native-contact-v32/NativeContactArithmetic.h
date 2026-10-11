// Experimental MotorRock contact arithmetic recovered from the observed x86 Native kernel.
// SPDX-License-Identifier: MIT
#pragma once
#include <Jolt/Physics/Constraints/ConstraintPart/AxisConstraintPart.h>
#include <cstdlib>
#include <cstring>
#include <string>
JPH_NAMESPACE_BEGIN
class NativeContactArithmetic
{
public:
    enum : uint { Mass = 1, Bias = 2, PointVelocity = 4, OriginalDelta = 8, ForceFirst = 16 };
    uint mMask = 0, mIteration = 0;
    bool mDynamicFirst = false, mDiagnosticError = false;
    Vec3 mR = Vec3::sZero(), mNormal = Vec3::sZero(); // Native Z-up polar coordinates
    float mInvI[9]{}; // Native column-major inverse inertia
    float mInvMass = 0, mError = 0, mRawBias = 0, mScaledBias = 0;
    static float Add(float a, float b) { volatile float r = a + b; return r; }
    static float Sub(float a, float b) { volatile float r = a - b; return r; }
    static float Mul(float a, float b) { volatile float r = a * b; return r; }
    static float Div(float a, float b) { volatile float r = a / b; return r; }
    static Vec3 Polar(Vec3Arg a) { return Vec3(a.GetX(), a.GetZ(), a.GetY()); }
    static Vec3 Axial(Vec3Arg a) { return -Polar(a); }
    static Vec3 Cross(Vec3Arg a, Vec3Arg b) {
        return Vec3(Sub(Mul(a.GetY(), b.GetZ()), Mul(a.GetZ(), b.GetY())),
                    Sub(Mul(a.GetZ(), b.GetX()), Mul(a.GetX(), b.GetZ())),
                    Sub(Mul(a.GetX(), b.GetY()), Mul(a.GetY(), b.GetX())));
    }
    static float DotXYZ(Vec3Arg a, Vec3Arg b) { return Add(Add(Mul(a.GetX(), b.GetX()), Mul(a.GetY(), b.GetY())), Mul(a.GetZ(), b.GetZ())); }
    static float DotXZY(Vec3Arg a, Vec3Arg b) { return Add(Add(Mul(a.GetX(), b.GetX()), Mul(a.GetZ(), b.GetZ())), Mul(a.GetY(), b.GetY())); }
    Vec3 Tensor(Vec3Arg a) const {
        float r[3]; for (uint j = 0; j < 3; ++j)
            r[j] = Add(Add(Mul(mInvI[j], a.GetX()), Mul(mInvI[3+j], a.GetY())), Mul(mInvI[6+j], a.GetZ()));
        return Vec3(r[0],r[1],r[2]);
    }
    static uint Flags() {
        const char *names[] = {"MASS", "BIAS", "POINT_VELOCITY", "ORIGINAL_DELTA", "FORCE_FIRST"};
        uint mask = 0; for (uint i = 0; i < 5; ++i) {
            std::string name = std::string("RRR3D_EXPERIMENT_NATIVE_CONTACT_") + names[i];
            const char *v = std::getenv(name.c_str()); if (v && std::strcmp(v,"1") == 0) mask |= 1u << i;
        }
        return mask;
    }
    void Prepare(uint inMask, bool inDynamicFirst, Vec3Arg inR, Mat44Arg inInvI, float inInvMass,
                 Vec3Arg inAxis, float inError, float inDeltaTime, bool inDiagnosticError, AxisConstraintPart &ioRow) {
        mMask=inMask; mIteration=0; mDynamicFirst=inDynamicFirst; mDiagnosticError=inDiagnosticError;
        mR=Polar(inR); mNormal=Polar(inDynamicFirst? -inAxis : inAxis); mInvMass=inInvMass; mError=inError;
        const uint p[] = {0,2,1}; for (uint c=0;c<3;++c) {
            Vec3 column=inInvI.GetColumn3(p[c]); for (uint r=0;r<3;++r) mInvI[c*3+r]=column[p[r]];
        }
        if (mMask & Mass) {
            Vec3 response=Tensor(Cross(mR,mNormal));
            float k=Div(1.0f,Add(mInvMass,DotXYZ(mNormal,Cross(response,mR))));
            ioRow.SetSourceContactEffectiveMass(k);
        }
        mRawBias=Mul(mError,Div(1.0f,inDeltaTime));
        mScaledBias=Mul(Mul(mRawBias,ioRow.GetDiagnosticEffectiveMass()),0.8f);
    }
    void MakeUnbiased() { mScaledBias=max(0.0f,mScaledBias); }
    float Delta(const MotionProperties *inMotion, Vec3Arg inAxis, const AxisConstraintPart &inRow, bool inSplitBias) const {
        float jv;
        if (mMask & PointVelocity) {
            Vec3 linear=Polar(inMotion->GetLinearVelocity()), angular=Axial(inMotion->GetAngularVelocity());
            Vec3 spin=Cross(angular,mR);
            Vec3 point(Add(linear.GetX(),spin.GetX()),Add(linear.GetY(),spin.GetY()),Add(linear.GetZ(),spin.GetZ()));
            jv=-DotXZY(mNormal,point);
        } else {
            if (mDynamicFirst) jv=inAxis.Dot(inMotion->GetLinearVelocity())+inRow.GetDiagnosticR1CrossAxis().Dot(inMotion->GetAngularVelocity());
            else jv=inAxis.Dot(-inMotion->GetLinearVelocity())-inRow.GetDiagnosticR2CrossAxis().Dot(inMotion->GetAngularVelocity());
        }
        const float k=inRow.GetDiagnosticEffectiveMass();
        if (mMask & Bias) return Sub(Mul(k,jv),mScaledBias);
        if (inSplitBias) return Sub(Mul(k,jv),Mul(k,inRow.GetDiagnosticVelocityBias()));
        return Mul(k,Sub(jv,inRow.GetDiagnosticVelocityBias()));
    }
    bool Apply(MotionProperties *ioMotion, float inDelta) const {
        if (inDelta==0.0f) return false;
        Vec3 force(Mul(mNormal.GetX(),inDelta),Mul(mNormal.GetY(),inDelta),Mul(mNormal.GetZ(),inDelta));
        Vec3 linear(Mul(force.GetX(),mInvMass),Mul(force.GetY(),mInvMass),Mul(force.GetZ(),mInvMass));
        ioMotion->AddLinearVelocityStep(Polar(linear));
        ioMotion->AddAngularVelocityStep(Axial(Tensor(Cross(mR,force))));
        return true;
    }
};
JPH_NAMESPACE_END
