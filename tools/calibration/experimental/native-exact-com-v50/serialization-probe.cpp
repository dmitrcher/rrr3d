#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/StreamWrapper.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>
#include <Jolt/Physics/Collision/TransformedShape.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
static bool same(JPH::Vec3 a,JPH::Vec3 b){JPH::Float3 x,y;a.StoreFloat3(&x);b.StoreFloat3(&y);return std::memcmp(&x,&y,sizeof(x))==0;}
int main(int argc,char**argv){
 if(argc!=2)return 1;JPH::RegisterDefaultAllocator();JPH::Factory::sInstance=new JPH::Factory();JPH::RegisterTypes();
 int status=0;{
 JPH::Ref<JPH::Shape> box=new JPH::BoxShape(JPH::Vec3(1.5f,.3f,.8f),0);
 const JPH::Vec3 pos(-.0392984f,.225793f,-.017845f),target(.1f,-.6f,0);
 auto inner=JPH::RotatedTranslatedShapeSettings(pos,JPH::Quat::sIdentity(),box).Create();if(inner.HasError())return 2;
 const auto offset=target-inner.Get()->GetCenterOfMass();
 auto old=JPH::OffsetCenterOfMassShapeSettings(offset,inner.Get()).Create();if(old.HasError())return 3;
 auto legacy=static_cast<JPH::OffsetCenterOfMassShape*>(old.Get().GetPtr());legacy->SetUserData(0x12345678cafebabeULL);
 std::stringstream legacyBytes(std::ios::in|std::ios::out|std::ios::binary);JPH::StreamOutWrapper legacyOut(legacyBytes);legacy->SaveBinaryState(legacyOut);
 {std::ofstream f(argv[1],std::ios::binary);f<<legacyBytes.str();if(!f)return 4;}
#ifdef MOTORROCK_EXACT_COM_TEST
 if(same(legacy->GetCenterOfMass(),target))return 5;
 auto exactResult=JPH::OffsetCenterOfMassShapeSettings(offset,inner.Get()).Create();if(exactResult.HasError())return 6;
 auto exact=static_cast<JPH::OffsetCenterOfMassShape*>(exactResult.Get().GetPtr());exact->SetUserData(0xabcdef0187654321ULL);exact->SetExactCenterOfMass(target);
 if(!exact->HasExactCenterOfMass()||!same(exact->GetCenterOfMass(),target)||!same(exact->GetOffset(),offset))return 7;
 std::stringstream stream(std::ios::in|std::ios::out|std::ios::binary);JPH::StreamOutWrapper out(stream);JPH::Shape::ShapeToIDMap shapeMap;JPH::Shape::MaterialToIDMap materialMap;
 legacy->SaveWithChildren(out,shapeMap,materialMap);exact->SaveWithChildren(out,shapeMap,materialMap);const JPH::uint32 tail=0x23456789U;out.Write(tail);if(out.IsFailed())return 8;
 stream.seekg(0);JPH::StreamInWrapper in(stream);JPH::Shape::IDToShapeMap shapes;JPH::Shape::IDToMaterialMap materials;
 auto a=JPH::Shape::sRestoreWithChildren(in,shapes,materials);auto b=JPH::Shape::sRestoreWithChildren(in,shapes,materials);if(a.HasError()||b.HasError()||in.IsFailed())return 9;
 auto restoredOld=static_cast<JPH::OffsetCenterOfMassShape*>(a.Get().GetPtr());auto restored=static_cast<JPH::OffsetCenterOfMassShape*>(b.Get().GetPtr());
 if(restoredOld->HasExactCenterOfMass()||!same(restoredOld->GetCenterOfMass(),legacy->GetCenterOfMass())||!restored->HasExactCenterOfMass()||!same(restored->GetCenterOfMass(),target)||!same(restored->GetOffset(),offset))return 10;
 if(restoredOld->GetUserData()!=legacy->GetUserData()||restored->GetUserData()!=exact->GetUserData()||restoredOld->GetInnerShape()!=restored->GetInnerShape())return 11;
 JPH::uint32 end=0;in.Read(end);if(end!=tail||in.IsFailed()||stream.peek()!=std::char_traits<char>::eof())return 12;
 const auto before=exact->GetLocalBounds(),after=restored->GetLocalBounds();if(!same(before.mMin,after.mMin)||!same(before.mMax,after.mMax))return 13;
 const auto world=JPH::Mat44::sRotationTranslation(JPH::Quat::sIdentity(),JPH::Vec3(10,20,30));auto wa=exact->GetWorldSpaceBounds(world,JPH::Vec3::sReplicate(1)),wb=restored->GetWorldSpaceBounds(world,JPH::Vec3::sReplicate(1));if(!same(wa.mMin,wb.mMin)||!same(wa.mMax,wb.mMax))return 14;
 std::cout<<"Exact COM, relative geometry, child sharing, user data and adjacent stream record survive serialization\n";
#else
 std::cout<<"Original unmodified offset shape serialized\n";
#endif
 }
 JPH::UnregisterTypes();delete JPH::Factory::sInstance;JPH::Factory::sInstance=nullptr;return status;
}
