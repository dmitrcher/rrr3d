#include <NxPhysics.h>
#include <NxCooking.h>
#include <NxStream.h>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <cstdio>
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

static void words(const NxVec3& v){unsigned u[3];float f[3]={v.x,v.y,v.z};std::memcpy(u,f,sizeof(u));std::printf("[%u,%u,%u]",u[0],u[1],u[2]);}
int main(){NxSDKCreateError error;NxPhysicsSDK* sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk)return 2;sdk->setParameter(NX_SKIN_WIDTH,.025f);NxCookingInterface* cooking=NxGetCookingLib(NX_PHYSICS_SDK_VERSION);if(!cooking||!cooking->NxInitCooking())return 3;NxSceneDesc settings;NxScene* scene=sdk->createScene(settings);if(!scene)return 4;
for(int geometry=0;geometry<3;++geometry){NxVec3 p[4];
 if(geometry==2){p[0]=NxVec3(8,-10,10);p[1]=NxVec3(8,10,10);p[2]=NxVec3(8,10,16);p[3]=NxVec3(8,-10,16);}
 else{float x[4]={-500,500,500,-500},y[4]={-500,-500,500,500};for(int j=0;j<4;++j)p[j]=NxVec3(x[j],y[j],10+(geometry==1?.1f*y[j]:0));}
 NxU32 indices[6]={0,1,2,0,2,3};NxTriangleMeshDesc d;d.numVertices=4;d.pointStrideBytes=sizeof(NxVec3);d.points=p;d.numTriangles=2;d.triangleStrideBytes=3*sizeof(NxU32);d.triangles=indices;CalibrationStream stream;if(!cooking->NxCookTriangleMesh(d,stream))return 5;NxTriangleMesh* mesh=sdk->createTriangleMesh(stream);if(!mesh)return 6;NxTriangleMeshShapeDesc sd;sd.meshData=mesh;NxActorDesc ad;ad.shapes.pushBack(&sd);NxActor* actor=scene->createActor(ad);if(!actor)return 7;NxTriangleMeshShape* shape=actor->getShapes()[0]->isTriangleMesh();if(!shape)return 8;
 for(unsigned i=0;i<2;++i){NxTriangle t,e;for(int z=0;z<3;++z)e.verts[z]=NxVec3(0,0,0);NxU32 flags=0;shape->getTriangle(t,&e,&flags,i,false,false);std::printf("{\"geometry\":%d,\"triangle\":%u,\"flags\":%u,\"points\":[",geometry,i,flags);for(int j=0;j<3;++j){if(j)std::printf(",");words(t.verts[j]);}std::printf("],\"edge_normals\":[");for(int j=0;j<3;++j){if(j)std::printf(",");words(e.verts[j]);}std::printf("]}\n");}
 scene->releaseActor(*actor);sdk->releaseTriangleMesh(*mesh);
}cooking->NxCloseCooking();sdk->releaseScene(*scene);sdk->release();return 0;}
