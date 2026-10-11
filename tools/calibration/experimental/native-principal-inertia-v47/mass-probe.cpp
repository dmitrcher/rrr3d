#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/MassProperties.h>
#define MOTORROCK_EXPERIMENT_SPRING_PROBES 1
#include "Fixture.h"
#include <cstdio>
#include <set>
namespace cal=motorrock::calibration;
static JPH::Vec3 jolt(cal::V v) { return JPH::Vec3(float(v.x),float(v.z),float(v.y)); }
static void shape(const std::string &name,int index,cal::V center,const JPH::MassProperties &mass) {
    fprintf(stderr,"{\"case\":\"%s\",\"index\":%d,\"mass\":%.9g,\"center\":[%.9g,%.9g,%.9g],\"tensor\":[",name.c_str(),index,mass.mMass,float(center.x),float(center.y),float(center.z));
    int axis[3]={0,2,1};for(int i=0;i<3;++i)for(int j=0;j<3;++j){if(i||j)fprintf(stderr,",");fprintf(stderr,"%.9g",mass.mInertia(axis[i],axis[j]));}fprintf(stderr,"]}\n");
}
int main(int argc,char **argv) {
    JPH::RegisterDefaultAllocator();
    if(argc!=2)return 2;
    std::set<std::string> names;std::ifstream input(argv[1]);std::string line;
    while(std::getline(input,line))names.insert(line);
    for(const auto &source:cal::cases()) {
        if(!names.count(source.name))continue;
        JPH::MassProperties combined;
        combined.SetMassAndInertiaOfSolidBox(2.0f*jolt(source.halfExtents),1.0f);
        combined.Translate(jolt(source.shapePosition));
        shape(source.name,0,source.shapePosition,combined);
        int shapeIndex=1;
        for(const auto &wheel:source.wheels) {
            JPH::MassProperties proxy;
            float r=float(wheel.radius),r2=r*r;
            proxy.mMass=JPH::JPH_PI*r2*r;
            proxy.mInertia=JPH::Mat44::sScale(proxy.mMass*r2*JPH::Vec3(1.0f/3.0f,.5f,1.0f/3.0f));
            proxy.Translate(jolt(wheel.position));
            shape(source.name,shapeIndex++,wheel.position,proxy);
            combined.mMass+=proxy.mMass;
            for(int a=0;a<3;++a)combined.mInertia.SetColumn4(a,combined.mInertia.GetColumn4(a)+proxy.mInertia.GetColumn4(a));
        }
        combined.ScaleToMass(float(source.mass));
        printf("{\"case\":\"%s\",\"dense\":[",source.name.c_str());
        int axes[3]={0,2,1};
        for(int i=0;i<3;++i)for(int j=0;j<3;++j){if(i||j)printf(",");printf("%.9g",combined.mInertia(axes[i],axes[j]));}
        JPH::Mat44 rotation;JPH::Vec3 principal;
        if(!combined.DecomposePrincipalMomentsOfInertia(rotation,principal))return 3;
        constexpr int permutations[6][3]={{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
        int best=0;float bestScore=-1;
        for(int k=0;k<6;++k){float score=0;for(int a=0;a<3;++a){float x=rotation.GetColumn3(permutations[k][a])[a];score+=x*x;}if(score>bestScore){bestScore=score;best=k;}}
        printf("],\"principal\":[");
        for(int i=0;i<3;++i){if(i)printf(",");printf("%.9g",principal[permutations[best][axes[i]]]);}
        printf("]}\n");
        names.erase(source.name);
    }
    return names.empty()?0:4;
}
