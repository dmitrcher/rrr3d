#ifndef MOTORROCK_WINDOWS_REFERENCE_GEOMETRY_H
#define MOTORROCK_WINDOWS_REFERENCE_GEOMETRY_H
#include <NxCooking.h>
#include <NxStream.h>
#include <cstring>
// Memory-only cooking stream; no assets or renderer are needed for this probe.
class CalibrationStream : public NxStream {
 std::vector<NxU8> bytes; mutable size_t cursor;
 template<class T> T read() const {T value;readBuffer(&value,sizeof(value));return value;}
 template<class T> NxStream& store(T value){return storeBuffer(&value,sizeof(value));}
public:
 CalibrationStream():cursor(0){}
 virtual NxU8 readByte() const{return read<NxU8>();}
 virtual NxU16 readWord() const{return read<NxU16>();}
 virtual NxU32 readDword() const{return read<NxU32>();}
 virtual NxF32 readFloat() const{return read<NxF32>();}
 virtual NxF64 readDouble() const{return read<NxF64>();}
 virtual void readBuffer(void* data,NxU32 size) const {if(cursor+size>bytes.size())throw std::runtime_error("Cooked mesh stream truncated");std::memcpy(data,&bytes[cursor],size);cursor+=size;}
 virtual NxStream& storeByte(NxU8 v){return store(v);}
 virtual NxStream& storeWord(NxU16 v){return store(v);}
 virtual NxStream& storeDword(NxU32 v){return store(v);}
 virtual NxStream& storeFloat(NxReal v){return store(v);}
 virtual NxStream& storeDouble(NxF64 v){return store(v);}
 virtual NxStream& storeBuffer(const void* data,NxU32 size){const NxU8* p=static_cast<const NxU8*>(data);bytes.insert(bytes.end(),p,p+size);return *this;}
};
// GameCar::OnContactModify makes the primary friction axis free on track
// triangles. Preserve the SDK friction frame and the original triangle-normal fallback.
class CalibrationTrackContact : public NxUserContactModify {
public:
 virtual bool onContactConstraint(NxU32& flags,const NxShape* s0,const NxShape* s1,NxU32 feature0,NxU32 feature1,NxContactCallbackData& data){
  const NxShape* car=s0->getActor().isDynamic()?s0:s1;
  if(!car->getActor().isDynamic() || !(s0->isTriangleMesh() || s1->isTriangleMesh()))return true;
  NxMat33 rotation(data.localorientation0);
  if(s0->getActor().isDynamic())rotation=s0->getActor().getCMassGlobalPose().M*rotation;
  const NxTriangleMeshShape* mesh=s0->isTriangleMesh()?s0->isTriangleMesh():s1->isTriangleMesh();
  NxTriangle triangle;mesh->getTriangle(triangle,0,0,s0->isTriangleMesh()?feature0:feature1,true,true);
  NxVec3 normal;triangle.normal(normal);// Physx.cpp actorIndex denotes the OTHER actor: car at shape 1 receives index 0.
  if(car==s1)normal=-normal;
  // Use the SDK's material friction frame, as GameCar::OnContactModify does.
  // Replacing it with the unprojected actor-up vector changes contact normals.
  NxVec3 second=rotation.getColumn(2),first=second.cross(normal);
  if(first.magnitude()<.5f){first=rotation.getColumn(1);second=normal.cross(first);}
  if(second.magnitude()<=.1f)return true;
  first.normalize();second.normalize();normal=first.cross(second);normal.normalize();
  NxMat33 frame;frame.setColumn(0,normal);frame.setColumn(1,first);frame.setColumn(2,second);
  NxMat33 inverse,local;
  if(s0->getActor().isDynamic()){s0->getActor().getCMassGlobalPose().M.getInverse(inverse);local=inverse*frame;}else local=frame;
  local.toQuat(data.localorientation0);
  if(s1->getActor().isDynamic()){s1->getActor().getCMassGlobalPose().M.getInverse(inverse);local=inverse*frame;}else local=frame;
  local.toQuat(data.localorientation1);
  data.staticFriction0=data.dynamicFriction0=0;
  flags|=NX_CCC_LOCALORIENTATION0|NX_CCC_LOCALORIENTATION1|NX_CCC_STATICFRICTION0|NX_CCC_DYNAMICFRICTION0;
  return true;
 }
};
#endif
