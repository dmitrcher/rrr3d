#include <Jolt/Jolt.h>
#include <Jolt/Core/StreamWrapper.h>
#include <Jolt/Physics/Vehicle/Wheel.h>
#include <sstream>
#include <iostream>

int main()
{
    JPH::WheelSettings source;
    if (source.mSuspensionSpringBiasFactor != 1.0f) return 1;
    source.mSuspensionSpringBiasFactor = .7f;
    source.mSuspensionMinLength = .35f;
    source.mSuspensionMaxLength = .9f;
    source.mSuspensionPreloadLength = -.05f;
    source.mSuspensionSpring = JPH::SpringSettings(JPH::ESpringMode::StiffnessAndDamping, 35000, 4500);
    source.mRadius = .4f;
    std::stringstream bytes(std::ios::in | std::ios::out | std::ios::binary);
    JPH::StreamOutWrapper out(bytes);
    source.SaveBinaryState(out);
    if (out.IsFailed()) return 2;
    bytes.seekg(0);
    JPH::StreamInWrapper in(bytes);
    JPH::WheelSettings restored;
    restored.RestoreBinaryState(in);
    if (in.IsFailed() || restored.mSuspensionSpringBiasFactor != .7f ||
        restored.mSuspensionSpring.mStiffness != 35000 || restored.mSuspensionSpring.mDamping != 4500 ||
        restored.mSuspensionMinLength != .35f || restored.mSuspensionMaxLength != .9f ||
        restored.mSuspensionPreloadLength != -.05f || restored.mRadius != .4f) return 3;
    if (bytes.peek() != std::char_traits<char>::eof()) return 4;
    std::cout << "Experimental bias setting and adjacent wheel fields survive binary round trip\n";
}
