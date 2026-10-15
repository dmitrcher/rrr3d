#include "SourceMeshManifold.h"
#include <cstdio>
#include <cstring>
namespace sm=r3d::physics::sourcemesh;
float read(){unsigned w;if(scanf("%u",&w)!=1)throw 1;float f;memcpy(&f,&w,4);return f;}unsigned word(float f){unsigned w;memcpy(&w,&f,4);return w;}sm::PxVec3 vec(){float x=read(),y=read(),z=read();return {x,y,z};}
int main(){try{auto half=vec(),x=vec(),y=vec(),z=vec(),p=vec();sm::Cm::Matrix34 box{{x,y,z},p};std::array<sm::PxVec3,3> triangle={vec(),vec(),vec()};auto plane=sm::trianglePlane(triangle);auto r=sm::findRotationMatrixFromZ(plane.n);auto tr=sm::nativeprojection::flatten(r,box);float old=sm::nativeedge::dot(plane.n,triangle[0]),next=sm::nativeprojection::planeHeight(r.column2,triangle[0]);printf("plane %u %u %u rotationZ %u %u %u oldLD %u nextLD %u\n",word(plane.n.x),word(plane.n.y),word(plane.n.z),word(r.column2.x),word(r.column2.y),word(r.column2.z),word(old),word(next));for(int side:{-1,1}){sm::PxVec3 point{half.x,side*half.y,-half.z};float pz=sm::nativeprojection::z(point,tr);printf("corner %d pz %u old %u new %u\n",side,word(pz),word(sm::nativeedge::sub(pz,old)),word(sm::nativeedge::sub(pz,next)));}return 0;}catch(int){return 2;}}
