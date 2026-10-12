#include <NxPhysics.h>
#include <cstdio>
#include <cstring>
#include <cmath>

static void words(const char *name,const NxVec3 &v){unsigned u[3];float f[3]={v.x,v.y,v.z};std::memcpy(u,f,sizeof(u));std::printf("\"%s\":[%u,%u,%u]",name,u[0],u[1],u[2]);}
int main(){
 NxSDKCreateError error;NxPhysicsSDK *sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk){std::fprintf(stderr,"SDK version %u create error %u\n",NX_PHYSICS_SDK_VERSION,(unsigned)error);return 2;}
 NxSceneDesc settings;settings.gravity=NxVec3(0,0,-20);NxScene *scene=sdk->createScene(settings);if(!scene)return 3;
 unsigned sequence=0;
 for(unsigned geometry=0;geometry<3;++geometry)for(unsigned rotation=0;rotation<4;++rotation)for(unsigned skin=0;skin<4;++skin){
  NxBoxShapeDesc box;box.dimensions=geometry==0?NxVec3(20,.25f,.3f):geometry==1?NxVec3(1.5f,.8f,.3f):NxVec3(.02f,.3f,.002f);
  const float skins[4]={0,.01f,.025f,.1f};box.skinWidth=skins[skin];
  NxActorDesc actor;actor.shapes.pushBack(&box);actor.globalPose.t=NxVec3(8,1,10.3f);
  NxQuat q;if(rotation){q.fromAngleAxisFast(rotation==1?.2f:rotation==2?.7f:-.4f,rotation==2?NxVec3(0,1,0):NxVec3(1,0,0));actor.globalPose.M.fromQuat(q);}
  NxActor *body=scene->createActor(actor);if(!body)return 4;NxShape *shape=body->getShapes()[0];NxMat34 pose=shape->getGlobalPose();
  for(unsigned ray=0;ray<8;++ray){
   NxVec3 local(0,0,box.dimensions.z+.6f),direction(0,0,-1);
   if(ray==1){local.x=box.dimensions.x*.75f;direction=NxVec3(.03f,.02f,-1);}
   if(ray==2){local.x=box.dimensions.x+.01f;}
   if(ray==3){local.z=-box.dimensions.z-.6f;direction=NxVec3(0,0,1);}
   if(ray==4){local=NxVec3(0,0,0);}
   if(ray==5){direction=NxVec3(1,0,0);}
   if(ray==6){local.x=box.dimensions.x;}
   if(ray==7){direction=NxVec3(1.0e-15f,0,-1);}
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
 sdk->releaseScene(*scene);sdk->release();return sequence==384?0:5;
}
