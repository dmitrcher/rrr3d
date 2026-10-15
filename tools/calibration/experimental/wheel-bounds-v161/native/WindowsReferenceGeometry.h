#ifndef MOTORROCK_WINDOWS_REFERENCE_GEOMETRY_H
#define MOTORROCK_WINDOWS_REFERENCE_GEOMETRY_H
#include <NxCooking.h>
#include <NxStream.h>
#include <cstring>
#include <float.h>
#include <intrin.h>
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
 bool floatingEnvironmentReported;
 int localPointTick;
 unsigned localPointIndex;
 bool callerReported;
public:
 int tick;
 explicit CalibrationTrackContact(const std::string& name,bool friction=true):caseName(name),frictionEnabled(friction),floatingEnvironmentReported(false),localPointTick(-1),localPointIndex(0),callerReported(false),tick(0){}
 virtual bool onContactConstraint(NxU32& flags,const NxShape* s0,const NxShape* s1,NxU32 feature0,NxU32 feature1,NxContactCallbackData& data){
  const char* callerTrace=std::getenv("NX_CONTACT_CALLER_TRACE");
  const char* callerAll=std::getenv("NX_CONTACT_CALLER_ALL");
  if((!callerReported || (callerAll && std::strcmp(callerAll,"1")==0)) && callerTrace && std::strcmp(callerTrace,"1")==0) {
   callerReported=true;
   const size_t address=reinterpret_cast<size_t>(_ReturnAddress());
   const HMODULE core=GetModuleHandleA("PhysXCore.dll");
   const size_t base=reinterpret_cast<size_t>(core);
   bool inCore=false;size_t offset=0;
   if(core){const IMAGE_DOS_HEADER* dos=reinterpret_cast<const IMAGE_DOS_HEADER*>(core);const IMAGE_NT_HEADERS* nt=reinterpret_cast<const IMAGE_NT_HEADERS*>(base+dos->e_lfanew);inCore=address>=base && address<base+nt->OptionalHeader.SizeOfImage;if(inCore)offset=address-base;}
   // Version-specific read-only stack diagnostic, never part of the solver.
   // In the verified 32-bit Core DLL the dispatcher at RVA207440 allocates
   // C8 bytes, saves four registers and pushes six callback arguments.
   size_t parentOffset=0;bool parentKnown=false;
   if(inCore && offset==0x207775 && sizeof(size_t)==4){const unsigned char* slot=reinterpret_cast<const unsigned char*>(_AddressOfReturnAddress());const size_t parent=*reinterpret_cast<const size_t*>(slot+0xf4);parentKnown=parent>=base && parent<base+reinterpret_cast<const IMAGE_NT_HEADERS*>(base+reinterpret_cast<const IMAGE_DOS_HEADER*>(core)->e_lfanew)->OptionalHeader.SizeOfImage;if(parentKnown)parentOffset=parent-base;}
   std::ostringstream caller;caller<<"{\"tick\":"<<tick<<",\"in_physx_core\":"<<(inCore?"true":"false")<<",\"caller_rva\":"<<offset<<",\"parent_known\":"<<(parentKnown?"true":"false")<<",\"parent_rva\":"<<parentOffset<<'}';
   motorrock::parity::event("calibration.contact-caller",caseName,caller.str());
  }
  if(!floatingEnvironmentReported && std::getenv("NX_FLOAT_ENV_TRACE")) {
   floatingEnvironmentReported=true;
   std::ostringstream environment;environment<<"{\"tick\":"<<tick<<",\"phase\":\"contact-callback\",\"control\":"<<_controlfp(0,0)<<'}';
   motorrock::parity::event("calibration.floating-environment",caseName,environment.str());
  }
  if(!frictionEnabled){
   data.staticFriction0=data.staticFriction1=data.dynamicFriction0=data.dynamicFriction1=0;
   flags|=NX_CCC_STATICFRICTION0|NX_CCC_STATICFRICTION1|NX_CCC_DYNAMICFRICTION0|NX_CCC_DYNAMICFRICTION1;
  }
  const NxShape* car=s0->getActor().isDynamic()?s0:s1;
  if(car->getActor().isDynamic()){
   const char* localTrace=std::getenv("NX_LOCAL_POINT_TRACE");
   if(localTrace && std::strcmp(localTrace,"1")==0) {
    if(localPointTick!=tick){localPointTick=tick;localPointIndex=0;}
    std::ostringstream localPoints;localPoints<<std::setprecision(9)
     <<"{\"tick\":"<<tick<<",\"index\":"<<localPointIndex++<<",\"phase\":\"pre-modify\""
     <<",\"local_position0\":"<<motorrock::calibration::vector(motorrock::calibration::V(data.localpos0.x,data.localpos0.y,data.localpos0.z))
     <<",\"local_position1\":"<<motorrock::calibration::vector(motorrock::calibration::V(data.localpos1.x,data.localpos1.y,data.localpos1.z));
    for(int ai=0;ai<2;++ai) {
     const NxActor& a=(ai==0?s0:s1)->getActor();
     NxMat34 pose;pose.id();if(a.isDynamic())pose=a.getCMassGlobalPose();
     localPoints<<",\"actor"<<ai<<"_id\":"<<reinterpret_cast<size_t>(a.userData)
      <<",\"actor"<<ai<<"_dynamic\":"<<(a.isDynamic()?"true":"false")
      <<",\"point_frame"<<ai<<"_position\":"<<motorrock::calibration::vector(motorrock::calibration::V(pose.t.x,pose.t.y,pose.t.z));
     for(int axis=0;axis<3;++axis){const NxVec3 v=pose.M.getColumn(axis);localPoints<<",\"point_frame"<<ai<<"_axis"<<axis<<"\":"<<motorrock::calibration::vector(motorrock::calibration::V(v.x,v.y,v.z));}
    }
    localPoints<<'}';motorrock::parity::event("calibration.local-points",caseName,localPoints.str());
   }
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
   if(std::getenv("NX_CONTACT_QUAT_TRACE")) {
    for(int actor=0;actor<2;++actor) {
     const NxActor& diagnosticActor=(actor==0?s0:s1)->getActor();
     const NxMat33 basis=diagnosticActor.getCMassGlobalPose().M;
     for(int axis=0;axis<3;++axis) {
      const NxVec3 v=basis.getColumn(axis);
      points<<",\"actor"<<actor<<"_axis"<<axis<<"\":"<<motorrock::calibration::vector(motorrock::calibration::V(v.x,v.y,v.z));
     }
     const NxQuat actorQuaternion=diagnosticActor.getGlobalOrientationQuat();
     points<<std::setprecision(9)<<",\"actor"<<actor<<"_quaternion\":["<<actorQuaternion.x<<','<<actorQuaternion.y<<','<<actorQuaternion.z<<','<<actorQuaternion.w<<']';
    }
    points<<std::setprecision(9)<<",\"local_quaternion0\":["<<data.localorientation0.x<<','<<data.localorientation0.y<<','<<data.localorientation0.z<<','<<data.localorientation0.w<<']'
     <<",\"local_quaternion1\":["<<data.localorientation1.x<<','<<data.localorientation1.y<<','<<data.localorientation1.z<<','<<data.localorientation1.w<<']';
   }
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
  if(std::getenv("NX_CONTACT_QUAT_TRACE")) {
   const NxMat33 result(data.localorientation0);
   const NxVec3 decoded=result.getColumn(0);
   std::ostringstream q;q<<std::setprecision(9)<<"{\"tick\":"<<tick
    <<",\"local_quaternion0\":["<<data.localorientation0.x<<','<<data.localorientation0.y<<','<<data.localorientation0.z<<','<<data.localorientation0.w<<']'
    <<",\"local_quaternion1\":["<<data.localorientation1.x<<','<<data.localorientation1.y<<','<<data.localorientation1.z<<','<<data.localorientation1.w<<']'
    <<",\"decoded_local_normal\":"<<motorrock::calibration::vector(motorrock::calibration::V(decoded.x,decoded.y,decoded.z))<<'}';
   motorrock::parity::event("calibration.contact-quaternion",caseName,q.str());
  }
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
   std::ostringstream p;p<<std::setprecision(9)<<"{\"tick\":"<<tick<<",\"index\":"<<pointIndex++
    <<",\"point\":"<<motorrock::calibration::vector(motorrock::calibration::V(point.x,point.y,point.z))
    <<",\"normal\":"<<motorrock::calibration::vector(motorrock::calibration::V(normal.x,normal.y,normal.z))
    <<",\"separation\":"<<contacts.getSeparation()<<",\"normal_force\":"<<contacts.getPointNormalForce()<<'}';
   motorrock::parity::event("calibration.point-force",caseName,p.str());
  }
 }
};
#endif
