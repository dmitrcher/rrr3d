#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "NativeQuaternionPoseV43.h"
#include "NativeContactNormalV38.h"
#include "NativeMaterialFrameV36.h"
using namespace motorrock;
int main(){int mode;while(scanf("%d",&mode)==1){
 float v[17];int count=mode==2?17:10;for(int i=0;i<count;++i)if(scanf("%f",v+i)!=1)return 2;
 if(mode<2){contactframe::Quaternion q={{v[0],v[1],v[2],v[3]}};contactframe::Vector p={{v[4],v[5],v[6]}},d={{v[7],v[8],v[9]}};auto out=mode==0?quaternionposev43::center(q,p,d):quaternionposev43::origin(q,p,d);for(int i=0;i<3;++i)printf("%.9g ",out[i]);puts("");}
 else {contactframe::Vector r={{v[0],v[1],v[2]}};auto n=contactnormalv38::decode({{v[3],v[4],v[5],v[6]}});auto c=framev36::cross(r,n);contactframe::Vector t;for(int i=0;i<3;++i)t[i]=derivederror::add(derivederror::add(derivederror::mul(v[7+i],c[0]),derivederror::mul(v[10+i],c[1])),derivederror::mul(v[13+i],c[2]));auto response=framev36::cross(t,r);float dot=derivederror::add(derivederror::add(derivederror::mul(n[2],response[2]),derivederror::mul(n[0],response[0])),derivederror::mul(n[1],response[1]));printf("%.9g\n",contactframe::store(1.0/derivederror::add(v[16],dot)));}
}return 0;}
