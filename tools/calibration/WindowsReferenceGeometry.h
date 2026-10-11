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
 std::string caseName;
 bool frictionEnabled;
public:
 int tick;
 explicit CalibrationTrackContact(const std::string& name,bool friction=true):caseName(name),frictionEnabled(friction),tick(0){}
 virtual bool onContactConstraint(NxU32& flags,const NxShape* s0,const NxShape* s1,NxU32 feature0,NxU32 feature1,NxContactCallbackData& data){
  if(!frictionEnabled){
   data.staticFriction0=data.staticFriction1=data.dynamicFriction0=data.dynamicFriction1=0;
   flags|=NX_CCC_STATICFRICTION0|NX_CCC_STATICFRICTION1|NX_CCC_DYNAMICFRICTION0|NX_CCC_DYNAMICFRICTION1;
  }
  const NxShape* car=s0->getActor().isDynamic()?s0:s1;
  if(car->getActor().isDynamic()){
   const NxVec3 point0=s0->getActor().isDynamic()?s0->getActor().getCMassGlobalPose()*data.localpos0:data.localpos0;
   const NxVec3 point1=s1->getActor().isDynamic()?s1->getActor().getCMassGlobalPose()*data.localpos1:data.localpos1;
   std::ostringstream points;points<<"{\"tick\":"<<tick<<",\"car_shape\":"<<(car==s0?0:1)
    <<",\"actor0_id\":"<<reinterpret_cast<size_t>(s0->getActor().userData)
    <<",\"actor1_id\":"<<reinterpret_cast<size_t>(s1->getActor().userData)
    <<",\"constraint_error\":"<<motorrock::calibration::vector(motorrock::calibration::V(data.error.x,data.error.y,data.error.z))
    <<",\"constraint_target\":"<<motorrock::calibration::vector(motorrock::calibration::V(data.target.x,data.target.y,data.target.z))
    <<",\"point0\":"<<motorrock::calibration::vector(motorrock::calibration::V(point0.x,point0.y,point0.z))
    <<",\"point1\":"<<motorrock::calibration::vector(motorrock::calibration::V(point1.x,point1.y,point1.z))
    <<",\"triangle\":"<<((s0->isTriangleMesh()||s1->isTriangleMesh())?"true":"false")
    <<",\"static_friction0\":"<<data.staticFriction0<<",\"static_friction1\":"<<data.staticFriction1
    <<",\"friction0\":"<<data.dynamicFriction0<<",\"friction1\":"<<data.dynamicFriction1;
   points<<",\"feature0\":"<<feature0<<",\"feature1\":"<<feature1;
   const NxTriangleMeshShape* diagnosticMesh=s0->isTriangleMesh()?s0->isTriangleMesh():s1->isTriangleMesh();
   if(diagnosticMesh){
    NxTriangle diagnosticTriangle;
    diagnosticMesh->getTriangle(diagnosticTriangle,0,0,s0->isTriangleMesh()?feature0:feature1,true,true);
    points<<",\"triangle_vertices\":[";
    for(int vi=0;vi<3;++vi){
     if(vi)points<<',';
     const NxVec3& dv=diagnosticTriangle.verts[vi];
     points<<motorrock::calibration::vector(motorrock::calibration::V(dv.x,dv.y,dv.z));
    }
    points<<']';
   }
   NxMat33 rawFrame(data.localorientation0);
   if(s0->getActor().isDynamic())rawFrame=s0->getActor().getCMassGlobalPose().M*rawFrame;
   for(int axis=0;axis<3;++axis){NxVec3 direction=rawFrame.getColumn(axis);points<<",\"axis"<<axis<<"\":"<<motorrock::calibration::vector(motorrock::calibration::V(direction.x,direction.y,direction.z));}
   points<<'}';
   motorrock::parity::event("calibration.body-points",caseName,points.str());
  }
  if(!car->getActor().isDynamic() || !(s0->isTriangleMesh() || s1->isTriangleMesh()))return true;
  NxMat33 rotation(data.localorientation0);
  if(s0->getActor().isDynamic())rotation=s0->getActor().getCMassGlobalPose().M*rotation;
  const NxMat33 originalFrame=rotation;
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
  std::ostringstream diagnostic;
  diagnostic<<"{\"tick\":"<<tick<<",\"car_shape\":"<<(car==s0?0:1);
  for(int axis=0;axis<3;++axis){NxVec3 before=originalFrame.getColumn(axis),after=frame.getColumn(axis);
   diagnostic<<",\"before_axis"<<axis<<"\":"<<motorrock::calibration::vector(motorrock::calibration::V(before.x,before.y,before.z))
             <<",\"after_axis"<<axis<<"\":"<<motorrock::calibration::vector(motorrock::calibration::V(after.x,after.y,after.z));}
  diagnostic<<",\"friction0\":"<<data.dynamicFriction0<<",\"friction1\":"<<data.dynamicFriction1<<'}';
  motorrock::parity::event("calibration.body-contact",caseName,diagnostic.str());
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
class CalibrationBodyReport : public NxUserContactReport {
 std::string caseName;
public:
 int tick;
 explicit CalibrationBodyReport(const std::string& name):caseName(name),tick(0){}
 virtual void onContactNotify(NxContactPair& pair,NxU32 events){
  std::ostringstream s;
  s<<"{\"tick\":"<<tick<<",\"events\":"<<events
   <<",\"normal_force\":"<<motorrock::calibration::vector(motorrock::calibration::V(pair.sumNormalForce.x,pair.sumNormalForce.y,pair.sumNormalForce.z))
   <<",\"friction_force\":"<<motorrock::calibration::vector(motorrock::calibration::V(pair.sumFrictionForce.x,pair.sumFrictionForce.y,pair.sumFrictionForce.z))<<'}';
  motorrock::parity::event("calibration.body-impulse",caseName,s.str());
  NxContactStreamIterator contacts(pair.stream); int pointIndex=0;
  while(contacts.goNextPair()) while(contacts.goNextPatch()) while(contacts.goNextPoint()) {
   const NxVec3 point=contacts.getPoint(),normal=contacts.getPatchNormal();
   std::ostringstream p;p<<"{\"tick\":"<<tick<<",\"index\":"<<pointIndex++
    <<",\"point\":"<<motorrock::calibration::vector(motorrock::calibration::V(point.x,point.y,point.z))
    <<",\"normal\":"<<motorrock::calibration::vector(motorrock::calibration::V(normal.x,normal.y,normal.z))
    <<",\"separation\":"<<contacts.getSeparation()<<",\"normal_force\":"<<contacts.getPointNormalForce()<<'}';
   motorrock::parity::event("calibration.point-force",caseName,p.str());
  }
 }
};
#endif
