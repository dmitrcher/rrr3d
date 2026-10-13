#include "NativeSpringSplitV53.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
using namespace motorrock_native_spring_v53;
float f(uint32_t n){float v;std::memcpy(&v,&n,4);return v;}
uint32_t bits(float v){uint32_t n;std::memcpy(&n,&v,4);return n;}
int main(){unsigned exact=0,old_errors=0;
{float error=f(1027550381u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1077098977u)return 1;float position=mul(rate,mul(f(1142779923u),0.7f));if(bits(position)!=1150963646u)return 2;++exact;if(bits(div(error,dt))!=1077098977u)++old_errors;}
{float error=f(1027550877u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1077099442u)return 1;float position=mul(rate,mul(f(1142779913u),0.7f));if(bits(position)!=1150964037u)return 2;++exact;if(bits(div(error,dt))!=1077099442u)++old_errors;}
{float error=f(1026246573u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1075876658u)return 1;float position=mul(rate,mul(f(1143264344u),0.7f));if(bits(position)!=1150336636u)return 2;++exact;if(bits(div(error,dt))!=1075876658u)++old_errors;}
{float error=f(1026247053u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1075877108u)return 1;float position=mul(rate,mul(f(1143264677u),0.7f));if(bits(position)!=1150337334u)return 2;++exact;if(bits(div(error,dt))!=1075877108u)++old_errors;}
{float error=f(1027550381u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1077098977u)return 1;float position=mul(rate,mul(f(1142779923u),0.7f));if(bits(position)!=1150963646u)return 2;++exact;if(bits(div(error,dt))!=1077098977u)++old_errors;}
{float error=f(1027550877u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1077099442u)return 1;float position=mul(rate,mul(f(1142779913u),0.7f));if(bits(position)!=1150964037u)return 2;++exact;if(bits(div(error,dt))!=1077099442u)++old_errors;}
{float error=f(1026246573u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1075876658u)return 1;float position=mul(rate,mul(f(1143264344u),0.7f));if(bits(position)!=1150336636u)return 2;++exact;if(bits(div(error,dt))!=1075876658u)++old_errors;}
{float error=f(1026247053u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1075877108u)return 1;float position=mul(rate,mul(f(1143264677u),0.7f));if(bits(position)!=1150337334u)return 2;++exact;if(bits(div(error,dt))!=1075877108u)++old_errors;}
{float error=f(987295904u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1036738965u)return 1;float position=mul(rate,mul(f(1142700243u),0.7f));if(bits(position)!=1110294951u)return 2;++exact;if(bits(div(error,dt))!=1036738965u)++old_errors;}
{float error=f(987280032u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=1036724085u)return 1;float position=mul(rate,mul(f(1142700211u),0.7f));if(bits(position)!=1110282202u)return 2;++exact;if(bits(div(error,dt))!=1036724085u)++old_errors;}
{float error=f(3153201628u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=3202541885u)return 1;float position=mul(rate,mul(f(1143653684u),0.7f));if(bits(position)!=3277056397u)return 2;++exact;if(bits(div(error,dt))!=3202541885u)++old_errors;}
{float error=f(3153206108u),dt=f(1015580809u);float rate=mul(error,div(1.0f,dt));if(bits(rate)!=3202546085u)return 1;float position=mul(rate,mul(f(1143653620u),0.7f));if(bits(position)!=3277060240u)return 2;++exact;if(bits(div(error,dt))!=3202546085u)++old_errors;}
std::printf("{\"exact\":%u,\"old_division_errors\":%u}\n",exact,old_errors);return 0;}
