#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE __attribute__((noinline))
#endif
// Diagnostic copies of original GameCar and current port expressions.
// No PhysX/Jolt runtime, gearbox progression, or torque fitting occurs here.
NOINLINE float original(float maximum,float ratio,float differential,float sem){const float cGameK=1.15f;return maximum*ratio*differential*sem*cGameK;}
NOINLINE float collapsed(float maximum,float ratio,float differential,float efficiency){return maximum*ratio*differential*efficiency;}
unsigned bits(float v){unsigned u;std::memcpy(&u,&v,4);return u;}
int main(){const float torques[]={300,600,2000,5000};const float sems[]={.7f,.9f,1.f};const float ratios[]={1.5f,2.66f,1.78f,1.30f,1.f,.74f};for(int t=0;t<4;++t)for(int s=0;s<3;++s)for(int g=0;g<6;++g){float efficiency=sems[s]*1.15f;float a=original(torques[t],ratios[g],3.42f,sems[s]),b=collapsed(torques[t],ratios[g],3.42f,efficiency);std::printf("{\"torque_index\":%d,\"sem_index\":%d,\"gear\":%d,\"source_bits\":%u,\"collapsed_bits\":%u,\"source\":%.9g,\"collapsed\":%.9g}\n",t,s,g,bits(a),bits(b),a,b);}return 0;}
