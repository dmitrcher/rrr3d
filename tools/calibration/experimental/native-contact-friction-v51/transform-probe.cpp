#include "NativeMaterialFrameV36.h"
#include "NativeContactCache.h"
#include "NativePhysicalPoseV42.h"
#include "NativePublicAngular.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
namespace cf=motorrock::contactframe;
static float get(std::istream& in){uint32_t w;in>>w;float x;std::memcpy(&x,&w,4);return x;}
static cf::Quaternion quat(std::istream& in){cf::Quaternion q;for(int i=0;i<4;++i)q[i]=get(in);return q;}
static cf::Vector vec(std::istream& in){cf::Vector q;for(int i=0;i<3;++i)q[i]=get(in);return q;}
static bool check(std::istream& in,float x){uint32_t expected,actual;in>>expected;std::memcpy(&actual,&x,4);if(expected!=actual){std::cerr<<" expected "<<expected<<" actual "<<actual;return false;}return true;}
int main(int argc,char**argv){if(argc!=2)return 2;setenv("RRR3D_EXPERIMENT_NATIVE_CACHE_LOCAL_POINT_ORDER","1",1);setenv("RRR3D_EXPERIMENT_NATIVE_CACHE_LEVER_ORDER","1",1);std::ifstream in(argv[1]);char type;unsigned count=0,fields=0;
while(in>>type){bool ok=true;
 if(type=='q'){auto a=quat(in),b=quat(in);auto r=motorrock::framev36::solverMultiply(a,b);for(int j=0;j<4;++j){ok=check(in,r[j])&&ok;++fields;}}
 else if(type=='o'){auto a=quat(in);auto r=motorrock::nativepublicangular::orientation({a[0],a[1],a[2],a[3]});for(int c=0;c<3;++c)for(int j=0;j<3;++j){ok=check(in,r[j][c])&&ok;++fields;}}
 else if(type=='p'||type=='r'){auto a=quat(in);auto position=vec(in),point=vec(in);motorrock::nativecache::Pose pose{motorrock::physicalposev42::rotation(a),position};auto r=type=='p'?motorrock::nativecache::localBody0Point(pose,point):motorrock::nativecache::solverLever(pose,point);for(int j=0;j<3;++j){ok=check(in,r[j])&&ok;++fields;}}
 else return 3;
 if(!ok){std::cerr<<" record "<<count<<" type "<<type<<'\n';return 1;}++count;
}std::cout<<"Verified "<<count<<" records, "<<fields<<" float32 words\n";return 0;}
