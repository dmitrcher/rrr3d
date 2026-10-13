#include "NativeSpringSplitV53.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <initializer_list>
using namespace motorrock_native_spring_v53;
float value(uint32_t x){float v;std::memcpy(&v,&x,4);return v;}
uint32_t bits(float v){uint32_t x;std::memcpy(&x,&v,4);return x;}
struct Fixture {uint32_t row[28],body0[12],body1[12],after0[12],after1[12],afterLambda;};
int main(){Fixture fixtures[]={
#include "native-tire-fixtures.inc"
};unsigned checked=0,failed=0;unsigned index=0;
for(const auto& f:fixtures){float r[28],b0[12],b1[12];for(int i=0;i<28;++i)r[i]=value(f.row[i]);for(int i=0;i<12;++i){b0[i]=value(f.body0[i]);b1[i]=value(f.body1[i]);}
float jv=sub(body0_jv(r,r+8,b0,b0+4),body1_jv(r+4,r+12,b1,b1+4));
// Live static-anchor path uses -source jv because its constraint axis is opposite.
float velocity=mul(sub(r[27],jv),r[7]);float position=mul(r[3],r[11]);
auto solved=solve(velocity,position,r[15],r[23],r[24],r[19],r[26],r[25]);float total=solved.biased;
if(bits(total)!=f.afterLambda){++failed;std::printf("lambda row %u %08x != %08x\n",index,bits(total),f.afterLambda);}++checked;
float delta=solved.delta;
if(delta!=0)for(int i=0;i<3;++i){b0[i]=add(b0[i],mul(r[i],mul(b0[11],delta)));b0[i+4]=add(b0[i+4],mul(r[i+16],delta));b1[i]=sub(b1[i],mul(r[i+4],mul(b1[11],delta)));b1[i+4]=sub(b1[i+4],mul(r[i+20],delta));}
for(int i:{0,1,2,4,5,6}){if(bits(b0[i])!=f.after0[i]||bits(b1[i])!=f.after1[i]){++failed;std::printf("body row %u component %d failed\n",index,i);}checked+=2;}++index;
}
std::printf("Native40tyre rows: %u checked words, %u mismatches\n",checked,failed);return failed?1:0;}
