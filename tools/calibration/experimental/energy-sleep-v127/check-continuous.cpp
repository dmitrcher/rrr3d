#include "NativeEnergySleepV127.h"
#include <cstdio>
#include <cstring>
namespace e=motorrock::energysleepv127;
float value(unsigned x){float y;std::memcpy(&y,&x,4);return y;}unsigned bits(float x){unsigned y;std::memcpy(&y,&x,4);return y;}
int main(){unsigned w[21];e::State state;while(std::scanf("%u",w)==1){for(unsigned i=1;i<21;++i)if(std::scanf("%u",w+i)!=1)return 2;e::Input in;for(unsigned i=0;i<3;++i){in.position[i]=value(w[i]);in.previousPosition[i]=value(w[3+i]);in.inverseInertia[i]=value(w[14+i]);}for(unsigned i=0;i<4;++i){in.rotation[i]=value(w[6+i]);in.previousRotation[i]=value(w[10+i]);}in.inverseMass=value(w[17]);in.threshold=value(w[18]);in.dt=value(w[19]);in.interactionCount=w[20];auto r=e::step(state,in);float data[]={state.linear[0],state.linear[1],state.linear[2],state.angular[0],state.angular[1],state.angular[2],state.wake};for(unsigned i=0;i<7;++i)std::printf("%s%u",i?" ":"",bits(data[i]));std::printf(" %u\n",r.sleep);}}
