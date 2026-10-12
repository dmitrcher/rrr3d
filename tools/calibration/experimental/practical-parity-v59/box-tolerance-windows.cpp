#include <NxPhysics.h>
#include <cstdio>
#include <cstring>
#include <cmath>

static void words(const char *name,const NxVec3 &v){unsigned u[3];float f[3]={v.x,v.y,v.z};std::memcpy(u,f,sizeof(u));std::printf("\"%s\":[%u,%u,%u]",name,u[0],u[1],u[2]);}
int main(){
 NxSDKCreateError error;NxPhysicsSDK *sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk){std::fprintf(stderr,"SDK version %u create error %u\n",NX_PHYSICS_SDK_VERSION,(unsigned)error);return 2;}
 NxSceneDesc settings;settings.gravity=NxVec3(0,0,-20);NxScene *scene=sdk->createScene(settings);if(!scene)return 3;
 unsigned sequence=0;
 const float halves[]={.002f,.02f,.2f,1.5f,20.f,200.f};
 const float offsets[]={-1e-3f,0,1e-8f,1e-7f,2e-7f,5e-7f,1e-6f,2e-6f,5e-6f,1e-5f,2e-5f,5e-5f,1e-4f,2e-4f,5e-4f,1e-3f,.01f};
 const float directions[]={0,-1e-15f,-1e-8f,-1e-4f};
 for(unsigned geometry=0;geometry<6;++geometry){unsigned rotation=0,skin=0;
  NxBoxShapeDesc box;box.dimensions=NxVec3(halves[geometry],.3f,.3f);box.skinWidth=.025f;
  NxActorDesc actor;actor.shapes.pushBack(&box);actor.globalPose.t=NxVec3(0,0,0);
  NxQuat q;if(rotation){q.fromAngleAxisFast(rotation==1?.2f:rotation==2?.7f:-.4f,rotation==2?NxVec3(0,1,0):NxVec3(1,0,0));actor.globalPose.M.fromQuat(q);}
  NxActor *body=scene->createActor(actor);if(!body)return 4;NxShape *shape=body->getShapes()[0];NxMat34 pose=shape->getGlobalPose();
  for(unsigned ray=0;ray<68;++ray){
   NxVec3 local(box.dimensions.x+offsets[ray/4],0,.9f),direction(directions[ray%4],0,-1);
   NxRay query;query.orig=pose*local;query.dir=pose.M*direction;query.dir.normalize();NxRaycastHit hit;std::memset(&hit,0,sizeof(hit));
   const bool found=shape->raycast(query,10,NX_RAYCAST_IMPACT|NX_RAYCAST_NORMAL|NX_RAYCAST_DISTANCE|NX_RAYCAST_FACE_INDEX,hit,false);
   std::printf("{\"query\":%u,\"geometry\":%u,\"rotation\":%u,\"skin\":%u,\"ray\":%u,\"hit\":%s,",sequence++,geometry,rotation,skin,ray,found?"true":"false");
   words("half",box.dimensions);std::printf(",");words("position",pose.t);std::printf(",");words("column0",pose.M.getColumn(0));std::printf(",");words("column1",pose.M.getColumn(1));std::printf(",");words("column2",pose.M.getColumn(2));std::printf(",");words("start",query.orig);std::printf(",");words("direction",query.dir);
   NxVec3 matrixLocal=pose%query.orig;NxMat34 inverse;pose.getInverseRT(inverse);NxVec3 inverseLocal=inverse*query.orig;
   NxQuat actorQ=body->getGlobalOrientationQuat();NxVec3 quaternionLocal=query.orig-pose.t;actorQ.inverseRotate(quaternionLocal);NxVec3 quaternionWorld=query.orig,inversePosition=-pose.t;actorQ.inverseRotate(quaternionWorld);actorQ.inverseRotate(inversePosition);quaternionWorld+=inversePosition;
   std::printf(",");words("matrix_local",matrixLocal);std::printf(",");words("inverse_local",inverseLocal);std::printf(",");words("quaternion_delta_local",quaternionLocal);std::printf(",");words("quaternion_world_local",quaternionWorld);
   unsigned qbits[4];float qvalues[4]={actorQ.x,actorQ.y,actorQ.z,actorQ.w};std::memcpy(qbits,qvalues,sizeof(qbits));std::printf(",\"quaternion\":[%u,%u,%u,%u]",qbits[0],qbits[1],qbits[2],qbits[3]);
   if(found){unsigned distance;std::memcpy(&distance,&hit.distance,4);std::printf(",\"distance\":%u,\"face\":%u,",distance,hit.faceID);words("point",hit.worldImpact);std::printf(",");words("normal",hit.worldNormal);}std::printf("}\n");
  }
  scene->releaseActor(*body);
 }
 sdk->releaseScene(*scene);sdk->release();return sequence==408?0:5;
}
