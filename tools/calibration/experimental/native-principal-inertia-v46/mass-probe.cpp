#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/MassProperties.h>
#include <cstdio>
static void emit(const char *label, int type, const JPH::MassProperties &p) {
    const int source[3] = {0, 2, 1};
    printf("%s %d %.9g", label, type, p.mMass);
    for (int i=0; i<3; ++i) for (int j=0; j<3; ++j)
        printf(" %.9g", p.mInertia(source[i], source[j]));
    printf("\n");
}
int main() {
    JPH::RegisterDefaultAllocator();
    for (int type=0; type<6; ++type) {
        JPH::MassProperties p;
        p.SetMassAndInertiaOfSolidBox(JPH::Vec3(3, .6f, 1.6f), 1);
        p.Translate(type==5 ? JPH::Vec3(.2f,.1f,.1f) : type>=3 ? JPH::Vec3(.2f,.1f,0) : JPH::Vec3::sZero());
        emit("box",type,p);
        for (int i=0; i<4; ++i) {
            JPH::MassProperties w;
            float r=.4f, r2=r*r;
            w.mMass=JPH::JPH_PI*r2*r;
            w.mInertia=JPH::Mat44::sScale(w.mMass*r2*JPH::Vec3(1.0f/3.0f,.5f,1.0f/3.0f));
            w.Translate(JPH::Vec3(i<2?1.1f:-1.1f,0,i%2==0?.75f:-.75f));
            p.mMass+=w.mMass;
            for (int a=0;a<3;++a) p.mInertia.SetColumn4(a,p.mInertia.GetColumn4(a)+w.mInertia.GetColumn4(a));
        }
        emit("unscaled",type,p);
        p.ScaleToMass(1200); emit("scaled",type,p);
        JPH::Mat44 axes; JPH::Vec3 principal;
        if (!p.DecomposePrincipalMomentsOfInertia(axes,principal)) return 2;
        printf("principal %d %.9g %.9g %.9g\n",type,principal[0],principal[1],principal[2]);
    }
}
