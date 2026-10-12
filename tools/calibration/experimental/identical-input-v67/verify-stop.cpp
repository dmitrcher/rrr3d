#include "NativeSpringSplitV53.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
using namespace motorrock_native_spring_v53;
float f(uint32_t w){float x;memcpy(&x,&w,4);return x;}
uint32_t bits(float x){uint32_t w;memcpy(&w,&x,4);return w;}
int main(){unsigned passed=0,failed=0,generic_mismatch=0;
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(3208688877u),f(1065002711u),f(2979011024u)},v[]={f(1090519042u),f(1073017511u),f(1069665966u)},w[]={f(3163889889u),f(1076225192u),f(3129361237u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1082268333u)++passed;else{++failed;printf("JV 0 %u expected 1082268333\n",bits(jv));}
float velocity=mul(-jv,f(1134544052u));float position=mul(f(1075303328u),f(1130343676u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3298977043u)++passed;else{++failed;printf("unbiased 0 %u expected 3298977043\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 0 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(1075303328u));float generic=add(f(0u),mul(f(1134544052u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(1061114292u),f(1065051128u),f(832483859u)},v[]={f(1090519042u),f(1073017511u),f(1069665966u)},w[]={f(3163889889u),f(1076225192u),f(3129361237u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1082226751u)++passed;else{++failed;printf("JV 1 %u expected 1082226751\n",bits(jv));}
float velocity=mul(-jv,f(1134562608u));float position=mul(f(1074583140u),f(1130369654u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3298943910u)++passed;else{++failed;printf("unbiased 1 %u expected 3298943910\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 1 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(1074583140u));float generic=add(f(0u),mul(f(1134562608u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(3208688877u),f(1065002711u),f(2979011024u)},v[]={f(1105694938u),f(1072979264u),f(1057806871u)},w[]={f(1079366008u),f(3244406330u),f(1083032336u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3246157432u)++passed;else{++failed;printf("JV 2 %u expected 3246157432\n",bits(jv));}
float velocity=mul(-jv,f(1134544052u));float position=mul(f(1075303328u),f(1130343676u));
auto result=solve(velocity,position,f(0u),f(3298977043u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1164567196u)++passed;else{++failed;printf("unbiased 2 %u expected 1164567196\n",bits(result.unbiased));}
if(bits(result.biased)==1166865813u)++passed;else{++failed;printf("biased 2 %u expected 1166865813\n",bits(result.biased));}
float bias=mul(0.7f,f(1075303328u));float generic=add(f(0u),mul(f(1134544052u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1166865813u)++generic_mismatch;
}
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(1061114292u),f(1065051128u),f(832483859u)},v[]={f(1105694938u),f(1072979264u),f(1082778724u)},w[]={f(3231766092u),f(3240059316u),f(1078028110u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3239291492u)++passed;else{++failed;printf("JV 3 %u expected 3239291492\n",bits(jv));}
float velocity=mul(-jv,f(1134562608u));float position=mul(f(1074583140u),f(1130369654u));
auto result=solve(velocity,position,f(0u),f(3298943910u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1154442434u)++passed;else{++failed;printf("unbiased 3 %u expected 1154442434\n",bits(result.unbiased));}
if(bits(result.biased)==1159320260u)++passed;else{++failed;printf("biased 3 %u expected 1159320260\n",bits(result.biased));}
float bias=mul(0.7f,f(1074583140u));float generic=add(f(0u),mul(f(1134562608u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1159320260u)++generic_mismatch;
}
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(3208688877u),f(1065002711u),f(2979011024u)},v[]={f(1106465590u),f(1072985693u),f(1084281520u)},w[]={f(3217697428u),f(3238339911u),f(1067844678u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3220664145u)++passed;else{++failed;printf("JV 4 %u expected 3220664145\n",bits(jv));}
float velocity=mul(-jv,f(1134544052u));float position=mul(f(1075303328u),f(1130343676u));
auto result=solve(velocity,position,f(0u),f(1164567196u),f(1166865813u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1166556886u)++passed;else{++failed;printf("unbiased 4 %u expected 1166556886\n",bits(result.unbiased));}
if(bits(result.biased)==1167044127u)++passed;else{++failed;printf("biased 4 %u expected 1167044127\n",bits(result.biased));}
float bias=mul(0.7f,f(1075303328u));float generic=add(f(1166865813u),mul(f(1134544052u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1167044127u)++generic_mismatch;
}
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(1061114292u),f(1065051128u),f(832483859u)},v[]={f(1106465590u),f(1072985693u),f(1084433682u)},w[]={f(3219052230u),f(3238256003u),f(1067616672u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3230234751u)++passed;else{++failed;printf("JV 5 %u expected 3230234751\n",bits(jv));}
float velocity=mul(-jv,f(1134562608u));float position=mul(f(1074583140u),f(1130369654u));
auto result=solve(velocity,position,f(0u),f(1154442434u),f(1159320260u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1161667782u)++passed;else{++failed;printf("unbiased 5 %u expected 1161667782\n",bits(result.unbiased));}
if(bits(result.biased)==1162933176u)++passed;else{++failed;printf("biased 5 %u expected 1162933176\n",bits(result.biased));}
float bias=mul(0.7f,f(1074583140u));float generic=add(f(1159320260u),mul(f(1134562608u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1162933176u)++generic_mismatch;
}
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(3208688877u),f(1065002711u),f(2979011024u)},v[]={f(1106443605u),f(1072945248u),f(1084332425u)},w[]={f(3209266646u),f(3236655102u),f(1056566442u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3217543973u)++passed;else{++failed;printf("JV 6 %u expected 3217543973\n",bits(jv));}
float velocity=mul(-jv,f(1134544052u));float position=mul(f(1075303328u),f(1130343676u));
auto result=solve(velocity,position,f(0u),f(1166556886u),f(1167044127u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1167578508u)++passed;else{++failed;printf("unbiased 6 %u expected 1167578508\n",bits(result.unbiased));}
if(bits(result.biased)==1166979031u)++passed;else{++failed;printf("biased 6 %u expected 1166979031\n",bits(result.biased));}
float bias=mul(0.7f,f(1075303328u));float generic=add(f(1167044127u),mul(f(1134544052u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1166979031u)++generic_mismatch;
}
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(1061114292u),f(1065051128u),f(832483859u)},v[]={f(1106443605u),f(1072945248u),f(1084276876u)},w[]={f(3208277466u),f(3236716366u),f(1056899389u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3224473345u)++passed;else{++failed;printf("JV 7 %u expected 3224473345\n",bits(jv));}
float velocity=mul(-jv,f(1134562608u));float position=mul(f(1074583140u),f(1130369654u));
auto result=solve(velocity,position,f(0u),f(1161667782u),f(1162933176u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1165305353u)++passed;else{++failed;printf("unbiased 7 %u expected 1165305353\n",bits(result.unbiased));}
if(bits(result.biased)==1164551051u)++passed;else{++failed;printf("biased 7 %u expected 1164551051\n",bits(result.biased));}
float bias=mul(0.7f,f(1074583140u));float generic=add(f(1162933176u),mul(f(1134562608u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1164551051u)++generic_mismatch;
}
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(3208688877u),f(1065002711u),f(2979011024u)},v[]={f(1106461685u),f(1072931188u),f(1083636276u)},w[]={f(3199149744u),f(3235682827u),f(1049462585u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3219331405u)++passed;else{++failed;printf("JV 8 %u expected 3219331405\n",bits(jv));}
float velocity=mul(-jv,f(1134544052u));float position=mul(f(0u),f(1130343676u));
auto result=solve(velocity,position,f(0u),f(1167578508u),f(1166979031u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1168739571u)++passed;else{++failed;printf("unbiased 8 %u expected 1168739571\n",bits(result.unbiased));}
if(bits(result.biased)==1168140094u)++passed;else{++failed;printf("biased 8 %u expected 1168140094\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1166979031u),mul(f(1134544052u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1168140094u)++generic_mismatch;
}
{
float n[]={f(2982794752u),f(2944401408u),f(1065353243u)},j[]={f(1061114292u),f(1065051128u),f(832483859u)},v[]={f(1106461685u),f(1072931188u),f(1084627053u)},w[]={f(3216139489u),f(3234590122u),f(1036757042u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3221683019u)++passed;else{++failed;printf("JV 9 %u expected 3221683019\n",bits(jv));}
float velocity=mul(-jv,f(1134562608u));float position=mul(f(0u),f(1130369654u));
auto result=solve(velocity,position,f(0u),f(1165305353u),f(1164551051u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1167043589u)++passed;else{++failed;printf("unbiased 9 %u expected 1167043589\n",bits(result.unbiased));}
if(bits(result.biased)==1166666438u)++passed;else{++failed;printf("biased 9 %u expected 1166666438\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1164551051u),mul(f(1134562608u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1166666438u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(3208823268u),f(1063776346u),f(2997384948u)},v[]={f(1090087746u),f(1072932320u),f(1085191623u)},w[]={f(3182848845u),f(1088420690u),f(3154468223u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1094576012u)++passed;else{++failed;printf("JV 10 %u expected 1094576012\n",bits(jv));}
float velocity=mul(-jv,f(1135182211u));float position=mul(f(1061675068u),f(1131237098u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3313204189u)++passed;else{++failed;printf("unbiased 10 %u expected 3313204189\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 10 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(1061675068u));float generic=add(f(0u),mul(f(1135182211u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(1060978610u),f(1063989628u),f(851121806u)},v[]={f(1090087746u),f(1072932320u),f(1085191623u)},w[]={f(3182848845u),f(1088420690u),f(3154468223u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1094529216u)++passed;else{++failed;printf("JV 11 %u expected 1094529216\n",bits(jv));}
float velocity=mul(-jv,f(1135256386u));float position=mul(f(1015389719u),f(1131340943u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3313251853u)++passed;else{++failed;printf("unbiased 11 %u expected 3313251853\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 11 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(1015389719u));float generic=add(f(0u),mul(f(1135256386u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(3208823268u),f(1063776346u),f(2997384948u)},v[]={f(1098232595u),f(1072675949u),f(1054838648u)},w[]={f(1075053964u),f(3227602636u),f(1064528064u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3230688768u)++passed;else{++failed;printf("JV 12 %u expected 3230688768\n",bits(jv));}
float velocity=mul(-jv,f(1135182211u));float position=mul(f(1061675068u),f(1131237098u));
auto result=solve(velocity,position,f(0u),f(3313204189u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3306938234u)++passed;else{++failed;printf("unbiased 12 %u expected 3306938234\n",bits(result.unbiased));}
if(bits(result.biased)==1151864768u)++passed;else{++failed;printf("biased 12 %u expected 1151864768\n",bits(result.biased));}
float bias=mul(0.7f,f(1061675068u));float generic=add(f(0u),mul(f(1135182211u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1151864768u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(1060978610u),f(1063989628u),f(851121806u)},v[]={f(1098232595u),f(1072675949u),f(1070026132u)},w[]={f(3181290688u),f(3222848101u),f(1051901502u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3207687517u)++passed;else{++failed;printf("JV 13 %u expected 3207687517\n",bits(jv));}
float velocity=mul(-jv,f(1135256386u));float position=mul(f(1015389719u),f(1131340943u));
auto result=solve(velocity,position,f(0u),f(3313251853u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3312283028u)++passed;else{++failed;printf("unbiased 13 %u expected 3312283028\n",bits(result.unbiased));}
if(bits(result.biased)==1130930698u)++passed;else{++failed;printf("biased 13 %u expected 1130930698\n",bits(result.biased));}
float bias=mul(0.7f,f(1015389719u));float generic=add(f(0u),mul(f(1135256386u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1130930698u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(3208823268u),f(1063776346u),f(2997384948u)},v[]={f(1096146649u),f(1072660127u),f(1074872121u)},w[]={f(3191308564u),f(3213801684u),f(3199059809u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1068671802u)++passed;else{++failed;printf("JV 14 %u expected 1068671802\n",bits(jv));}
float velocity=mul(-jv,f(1135182211u));float position=mul(f(1061675068u),f(1131237098u));
auto result=solve(velocity,position,f(0u),f(3306938234u),f(1151864768u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3308876162u)++passed;else{++failed;printf("unbiased 14 %u expected 3308876162\n",bits(result.unbiased));}
if(bits(result.biased)==1143702866u)++passed;else{++failed;printf("biased 14 %u expected 1143702866\n",bits(result.biased));}
float bias=mul(0.7f,f(1061675068u));float generic=add(f(1151864768u),mul(f(1135182211u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1143702866u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(1060978610u),f(1063989628u),f(851121806u)},v[]={f(1096146649u),f(1072660127u),f(1071399792u)},w[]={f(1065208045u),f(3218458355u),f(3174517600u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063998914u)++passed;else{++failed;printf("JV 15 %u expected 1063998914\n",bits(jv));}
float velocity=mul(-jv,f(1135256386u));float position=mul(f(1015389719u),f(1131340943u));
auto result=solve(velocity,position,f(0u),f(3312283028u),f(1130930698u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3313534106u)++passed;else{++failed;printf("unbiased 15 %u expected 3313534106\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 15 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(1015389719u));float generic=add(f(1130930698u),mul(f(1135256386u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(3208823268u),f(1063776346u),f(2997384948u)},v[]={f(1096189149u),f(1072644081u),f(1072870649u)},w[]={f(1049867614u),f(3216523896u),f(3193847528u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1052684640u)++passed;else{++failed;printf("JV 16 %u expected 1052684640\n",bits(jv));}
float velocity=mul(-jv,f(1135182211u));float position=mul(f(1061675068u),f(1131237098u));
auto result=solve(velocity,position,f(0u),f(3308876162u),f(1143702866u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3309393339u)++passed;else{++failed;printf("unbiased 16 %u expected 3309393339\n",bits(result.unbiased));}
if(bits(result.biased)==1136346305u)++passed;else{++failed;printf("biased 16 %u expected 1136346305\n",bits(result.biased));}
float bias=mul(0.7f,f(1061675068u));float generic=add(f(1143702866u),mul(f(1135182211u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1136346305u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(1060978610u),f(1063989628u),f(851121806u)},v[]={f(1096189149u),f(1072644081u),f(1070692778u)},w[]={f(1062708693u),f(3218727340u),f(3181310276u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1060242984u)++passed;else{++failed;printf("JV 17 %u expected 1060242984\n",bits(jv));}
float velocity=mul(-jv,f(1135256386u));float position=mul(f(1015389719u),f(1131340943u));
auto result=solve(velocity,position,f(0u),f(3313534106u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3314020148u)++passed;else{++failed;printf("unbiased 17 %u expected 3314020148\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 17 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(1015389719u));float generic=add(f(0u),mul(f(1135256386u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(3208823268u),f(1063776346u),f(2997384948u)},v[]={f(1096184937u),f(1072627842u),f(1072722017u)},w[]={f(1049711794u),f(3216320732u),f(3194854965u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1052944928u)++passed;else{++failed;printf("JV 18 %u expected 1052944928\n",bits(jv));}
float velocity=mul(-jv,f(1135182211u));float position=mul(f(0u),f(1131237098u));
auto result=solve(velocity,position,f(2147483648u),f(3309393339u),f(1136346305u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3309921288u)++passed;else{++failed;printf("unbiased 18 %u expected 3309921288\n",bits(result.unbiased));}
if(bits(result.biased)==1131783352u)++passed;else{++failed;printf("biased 18 %u expected 1131783352\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1136346305u),mul(f(1135182211u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1131783352u)++generic_mismatch;
}
{
float n[]={f(3001871360u),f(2966421504u),f(1065353241u)},j[]={f(1060978610u),f(1063989628u),f(851121806u)},v[]={f(1096184937u),f(1072627842u),f(1071820982u)},w[]={f(1057182755u),f(3217232347u),f(3190983814u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1061161304u)++passed;else{++failed;printf("JV 19 %u expected 1061161304\n",bits(jv));}
float velocity=mul(-jv,f(1135256386u));float position=mul(f(0u),f(1131340943u));
auto result=solve(velocity,position,f(0u),f(3314020148u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3314544447u)++passed;else{++failed;printf("unbiased 19 %u expected 3314544447\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 19 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(0u),mul(f(1135256386u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504760u),f(1069539123u),f(0u)},v[]={f(0u),f(0u),f(3243944614u)},w[]={f(0u),f(0u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3243944614u)++passed;else{++failed;printf("JV 20 %u expected 3243944614\n",bits(jv));}
float velocity=mul(-jv,f(1137295511u));float position=mul(f(3217310357u),f(1133328899u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1168921725u)++passed;else{++failed;printf("unbiased 20 %u expected 1168921725\n",bits(result.unbiased));}
if(bits(result.biased)==1169808674u)++passed;else{++failed;printf("biased 20 %u expected 1169808674\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(0u),mul(f(1137295511u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1169808674u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062937712u),f(1069538867u),f(2147483648u)},v[]={f(0u),f(0u),f(3240766315u)},w[]={f(3237178617u),f(1080583056u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3241887196u)++passed;else{++failed;printf("JV 21 %u expected 3241887196\n",bits(jv));}
float velocity=mul(-jv,f(1137362411u));float position=mul(f(3217310357u),f(1133375729u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1167349224u)++passed;else{++failed;printf("unbiased 21 %u expected 1167349224\n",bits(result.unbiased));}
if(bits(result.biased)==1168240660u)++passed;else{++failed;printf("biased 21 %u expected 1168240660\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(0u),mul(f(1137362411u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1168240660u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504448u),f(3213566830u),f(0u)},v[]={f(0u),f(0u),f(3237975805u)},w[]={f(3212815002u),f(1088010214u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3244842626u)++passed;else{++failed;printf("JV 22 %u expected 3244842626\n",bits(jv));}
float velocity=mul(-jv,f(1140038187u));float position=mul(f(3220834037u),f(1135248772u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1172118949u)++passed;else{++failed;printf("unbiased 22 %u expected 1172118949\n",bits(result.unbiased));}
if(bits(result.biased)==1173483270u)++passed;else{++failed;printf("biased 22 %u expected 1173483270\n",bits(result.biased));}
float bias=mul(0.7f,f(3220834037u));float generic=add(f(0u),mul(f(1140038187u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1173483270u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062936832u),f(3213566491u),f(0u)},v[]={f(0u),f(0u),f(3229782260u)},w[]={f(3240942729u),f(1079654323u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3246933514u)++passed;else{++failed;printf("JV 23 %u expected 3246933514\n",bits(jv));}
float velocity=mul(-jv,f(1140136472u));float position=mul(f(3221281275u),f(1135317572u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1174567147u)++passed;else{++failed;printf("unbiased 23 %u expected 1174567147\n",bits(result.unbiased));}
if(bits(result.biased)==1175274578u)++passed;else{++failed;printf("biased 23 %u expected 1175274578\n",bits(result.biased));}
float bias=mul(0.7f,f(3221281275u));float generic=add(f(0u),mul(f(1140136472u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1175274578u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504760u),f(1069539123u),f(0u)},v[]={f(1005556994u),f(0u),f(1061379121u)},w[]={f(3169178118u),f(3196143875u),f(990762671u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1053903357u)++passed;else{++failed;printf("JV 24 %u expected 1053903357\n",bits(jv));}
float velocity=mul(-jv,f(1137295511u));float position=mul(f(3217310357u),f(1133328899u));
auto result=solve(velocity,position,f(0u),f(1168921725u),f(1169808674u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1168583929u)++passed;else{++failed;printf("unbiased 24 %u expected 1168583929\n",bits(result.unbiased));}
if(bits(result.biased)==1170357826u)++passed;else{++failed;printf("biased 24 %u expected 1170357826\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1169808674u),mul(f(1137295511u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1170357826u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062937712u),f(1069538867u),f(2147483648u)},v[]={f(1005556994u),f(0u),f(1064198874u)},w[]={f(3202765842u),f(3176240900u),f(990762671u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1055937946u)++passed;else{++failed;printf("JV 25 %u expected 1055937946\n",bits(jv));}
float velocity=mul(-jv,f(1137362411u));float position=mul(f(3217310357u),f(1133375729u));
auto result=solve(velocity,position,f(0u),f(1167349224u),f(1168240660u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1166959357u)++passed;else{++failed;printf("unbiased 25 %u expected 1166959357\n",bits(result.unbiased));}
if(bits(result.biased)==1168742230u)++passed;else{++failed;printf("biased 25 %u expected 1168742230\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1168240660u),mul(f(1137362411u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1168742230u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504448u),f(3213566830u),f(0u)},v[]={f(1005556994u),f(0u),f(1066088741u)},w[]={f(3178449282u),f(1040945010u),f(990762671u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1065196704u)++passed;else{++failed;printf("JV 26 %u expected 1065196704\n",bits(jv));}
float velocity=mul(-jv,f(1140038187u));float position=mul(f(3220834037u),f(1135248772u));
auto result=solve(velocity,position,f(0u),f(1172118949u),f(1173483270u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1171130463u)++passed;else{++failed;printf("unbiased 26 %u expected 1171130463\n",bits(result.unbiased));}
if(bits(result.biased)==1173859104u)++passed;else{++failed;printf("biased 26 %u expected 1173859104\n",bits(result.biased));}
float bias=mul(0.7f,f(3220834037u));float generic=add(f(1173483270u),mul(f(1140038187u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1173859104u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062936832u),f(3213566491u),f(0u)},v[]={f(1005556994u),f(0u),f(1067024108u)},w[]={f(3199053800u),f(1025604386u),f(990762671u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063104380u)++passed;else{++failed;printf("JV 27 %u expected 1063104380\n",bits(jv));}
float velocity=mul(-jv,f(1140136472u));float position=mul(f(3221281275u),f(1135317572u));
auto result=solve(velocity,position,f(0u),f(1174567147u),f(1175274578u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1173859805u)++passed;else{++failed;printf("unbiased 27 %u expected 1173859805\n",bits(result.unbiased));}
if(bits(result.biased)==1175547325u)++passed;else{++failed;printf("biased 27 %u expected 1175547325\n",bits(result.biased));}
float bias=mul(0.7f,f(3221281275u));float generic=add(f(1175274578u),mul(f(1140136472u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1175547325u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504760u),f(1069539123u),f(0u)},v[]={f(999639064u),f(0u),f(1066397855u)},w[]={f(1055947424u),f(3197080796u),f(975251419u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1050245845u)++passed;else{++failed;printf("JV 28 %u expected 1050245845\n",bits(jv));}
float velocity=mul(-jv,f(1137295511u));float position=mul(f(3217310357u),f(1133328899u));
auto result=solve(velocity,position,f(0u),f(1168583929u),f(1170357826u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1168336210u)++passed;else{++failed;printf("unbiased 28 %u expected 1168336210\n",bits(result.unbiased));}
if(bits(result.biased)==1170997055u)++passed;else{++failed;printf("biased 28 %u expected 1170997055\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1170357826u),mul(f(1137295511u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1170997055u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062937712u),f(1069538867u),f(2147483648u)},v[]={f(999639064u),f(0u),f(1067876405u)},w[]={f(1021307808u),f(3180193071u),f(975251419u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1067201482u)++passed;else{++failed;printf("JV 29 %u expected 1067201482\n",bits(jv));}
float velocity=mul(-jv,f(1137362411u));float position=mul(f(3217310357u),f(1133375729u));
auto result=solve(velocity,position,f(0u),f(1166959357u),f(1168742230u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1165875090u)++passed;else{++failed;printf("unbiased 29 %u expected 1165875090\n",bits(result.unbiased));}
if(bits(result.biased)==1168620110u)++passed;else{++failed;printf("biased 29 %u expected 1168620110\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1168742230u),mul(f(1137362411u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1168620110u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504448u),f(3213566830u),f(0u)},v[]={f(999639064u),f(0u),f(1067795524u)},w[]={f(995536448u),f(3181743312u),f(975251419u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1068508965u)++passed;else{++failed;printf("JV 30 %u expected 1068508965\n",bits(jv));}
float velocity=mul(-jv,f(1140038187u));float position=mul(f(3220834037u),f(1135248772u));
auto result=solve(velocity,position,f(0u),f(1171130463u),f(1173859104u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1169757303u)++passed;else{++failed;printf("unbiased 30 %u expected 1169757303\n",bits(result.unbiased));}
if(bits(result.biased)==1173850265u)++passed;else{++failed;printf("biased 30 %u expected 1173850265\n",bits(result.biased));}
float bias=mul(0.7f,f(3220834037u));float generic=add(f(1173859104u),mul(f(1140038187u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1173850265u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062936832u),f(3213566491u),f(0u)},v[]={f(999639064u),f(0u),f(1067868175u)},w[]={f(3164026833u),f(3182753108u),f(975251419u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1068541304u)++passed;else{++failed;printf("JV 31 %u expected 1068541304\n",bits(jv));}
float velocity=mul(-jv,f(1140136472u));float position=mul(f(3221281275u),f(1135317572u));
auto result=solve(velocity,position,f(0u),f(1173859805u),f(1175547325u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1172474321u)++passed;else{++failed;printf("unbiased 31 %u expected 1172474321\n",bits(result.unbiased));}
if(bits(result.biased)==1175562014u)++passed;else{++failed;printf("biased 31 %u expected 1175562014\n",bits(result.biased));}
float bias=mul(0.7f,f(3221281275u));float generic=add(f(1175547325u),mul(f(1140136472u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1175562014u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504760u),f(1069539123u),f(0u)},v[]={f(999639114u),f(0u),f(1067462409u)},w[]={f(1043491070u),f(3193153430u),f(975251644u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1061856697u)++passed;else{++failed;printf("JV 32 %u expected 1061856697\n",bits(jv));}
float velocity=mul(-jv,f(1137295511u));float position=mul(f(3217310357u),f(1133328899u));
auto result=solve(velocity,position,f(0u),f(1168336210u),f(1170997055u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1167682057u)++passed;else{++failed;printf("unbiased 32 %u expected 1167682057\n",bits(result.unbiased));}
if(bits(result.biased)==1171229851u)++passed;else{++failed;printf("biased 32 %u expected 1171229851\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1170997055u),mul(f(1137295511u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1171229851u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062937712u),f(1069538867u),f(2147483648u)},v[]={f(999639114u),f(0u),f(1068039797u)},w[]={f(985842176u),f(3187568850u),f(975251644u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066488258u)++passed;else{++failed;printf("JV 33 %u expected 1066488258\n",bits(jv));}
float velocity=mul(-jv,f(1137362411u));float position=mul(f(3217310357u),f(1133375729u));
auto result=solve(velocity,position,f(0u),f(1165875090u),f(1168620110u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1163989211u)++passed;else{++failed;printf("unbiased 33 %u expected 1163989211\n",bits(result.unbiased));}
if(bits(result.biased)==1168568607u)++passed;else{++failed;printf("biased 33 %u expected 1168568607\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1168620110u),mul(f(1137362411u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1168568607u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504448u),f(3213566830u),f(0u)},v[]={f(999639114u),f(0u),f(1068034624u)},w[]={f(3093133824u),f(3187667999u),f(975251644u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1069174620u)++passed;else{++failed;printf("JV 34 %u expected 1069174620\n",bits(jv));}
float velocity=mul(-jv,f(1140038187u));float position=mul(f(3220834037u),f(1135248772u));
auto result=solve(velocity,position,f(0u),f(1169757303u),f(1173850265u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1168304966u)++passed;else{++failed;printf("unbiased 34 %u expected 1168304966\n",bits(result.unbiased));}
if(bits(result.biased)==1173762249u)++passed;else{++failed;printf("biased 34 %u expected 1173762249\n",bits(result.biased));}
float bias=mul(0.7f,f(3220834037u));float generic=add(f(1173850265u),mul(f(1140038187u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1173762249u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062936832u),f(3213566491u),f(0u)},v[]={f(999639114u),f(0u),f(1067904084u)},w[]={f(1025490897u),f(3185853567u),f(975251644u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1069200437u)++passed;else{++failed;printf("JV 35 %u expected 1069200437\n",bits(jv));}
float velocity=mul(-jv,f(1140136472u));float position=mul(f(3221281275u),f(1135317572u));
auto result=solve(velocity,position,f(0u),f(1172474321u),f(1175562014u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1171009953u)++passed;else{++failed;printf("unbiased 35 %u expected 1171009953\n",bits(result.unbiased));}
if(bits(result.biased)==1175537261u)++passed;else{++failed;printf("biased 35 %u expected 1175537261\n",bits(result.biased));}
float bias=mul(0.7f,f(3221281275u));float generic=add(f(1175562014u),mul(f(1140136472u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1175537261u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504760u),f(1069539123u),f(0u)},v[]={f(999639114u),f(0u),f(1067846739u)},w[]={f(1018414682u),f(3185056545u),f(975251644u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066361370u)++passed;else{++failed;printf("JV 36 %u expected 1066361370\n",bits(jv));}
float velocity=mul(-jv,f(1137295511u));float position=mul(f(0u),f(1133328899u));
auto result=solve(velocity,position,f(0u),f(1167682057u),f(1171229851u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1166756364u)++passed;else{++failed;printf("unbiased 36 %u expected 1166756364\n",bits(result.unbiased));}
if(bits(result.biased)==1170304158u)++passed;else{++failed;printf("biased 36 %u expected 1170304158\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1171229851u),mul(f(1137295511u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1170304158u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062937712u),f(1069538867u),f(2147483648u)},v[]={f(999639114u),f(0u),f(1066895558u)},w[]={f(1050472455u),f(3195479565u),f(975251644u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066061751u)++passed;else{++failed;printf("JV 37 %u expected 1066061751\n",bits(jv));}
float velocity=mul(-jv,f(1137362411u));float position=mul(f(0u),f(1133375729u));
auto result=solve(velocity,position,f(0u),f(1163989211u),f(1168568607u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1162187789u)++passed;else{++failed;printf("unbiased 37 %u expected 1162187789\n",bits(result.unbiased));}
if(bits(result.biased)==1167667896u)++passed;else{++failed;printf("biased 37 %u expected 1167667896\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1168568607u),mul(f(1137362411u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1167667896u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3210504448u),f(3213566830u),f(0u)},v[]={f(999639114u),f(0u),f(1065999474u)},w[]={f(1025752744u),f(3200063393u),f(975251644u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1069078426u)++passed;else{++failed;printf("JV 38 %u expected 1069078426\n",bits(jv));}
float velocity=mul(-jv,f(1140038187u));float position=mul(f(0u),f(1135248772u));
auto result=solve(velocity,position,f(0u),f(1168304966u),f(1173762249u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1166864071u)++passed;else{++failed;printf("unbiased 38 %u expected 1166864071\n",bits(result.unbiased));}
if(bits(result.biased)==1172321354u)++passed;else{++failed;printf("biased 38 %u expected 1172321354\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1173762249u),mul(f(1140038187u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172321354u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1062936832u),f(3213566491u),f(0u)},v[]={f(999639114u),f(0u),f(1063529500u)},w[]={f(1057067330u),f(3193238697u),f(975251644u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1069971800u)++passed;else{++failed;printf("JV 39 %u expected 1069971800\n",bits(jv));}
float velocity=mul(-jv,f(1140136472u));float position=mul(f(0u),f(1135317572u));
auto result=solve(velocity,position,f(0u),f(1171009953u),f(1175537261u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1169453270u)++passed;else{++failed;printf("unbiased 39 %u expected 1169453270\n",bits(result.unbiased));}
if(bits(result.biased)==1174758919u)++passed;else{++failed;printf("biased 39 %u expected 1174758919\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1175537261u),mul(f(1140136472u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1174758919u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505686u),f(1069540014u),f(773389069u)},v[]={f(999639114u),f(0u),f(1051925235u)},w[]={f(3159913577u),f(3168679942u),f(975242843u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1050942439u)++passed;else{++failed;printf("JV 40 %u expected 1050942439\n",bits(jv));}
float velocity=mul(-jv,f(1137294252u));float position=mul(f(3201534665u),f(1133328018u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3271643703u)++passed;else{++failed;printf("unbiased 40 %u expected 3271643703\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 40 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3201534665u));float generic=add(f(0u),mul(f(1137294252u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937030u),f(1069540014u),f(763993440u)},v[]={f(999639114u),f(2886575273u),f(1055945329u)},w[]={f(3198204059u),f(1039033986u),f(963022716u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1052784664u)++passed;else{++failed;printf("JV 41 %u expected 1052784664\n",bits(jv));}
float velocity=mul(-jv,f(1137361944u));float position=mul(f(3200269265u),f(1133375402u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3273146240u)++passed;else{++failed;printf("unbiased 41 %u expected 3273146240\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 41 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3200269265u));float generic=add(f(0u),mul(f(1137361944u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505724u),f(3213565826u),f(2904057632u)},v[]={f(999639114u),f(2894686176u),f(1058344749u)},w[]={f(3171325824u),f(1048621478u),f(974992927u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1051507264u)++passed;else{++failed;printf("JV 42 %u expected 1051507264\n",bits(jv));}
float velocity=mul(-jv,f(1140037376u));float position=mul(f(3205458661u),f(1135248205u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3273939629u)++passed;else{++failed;printf("unbiased 42 %u expected 3273939629\n",bits(result.unbiased));}
if(bits(result.biased)==1104515912u)++passed;else{++failed;printf("biased 42 %u expected 1104515912\n",bits(result.biased));}
float bias=mul(0.7f,f(3205458661u));float generic=add(f(0u),mul(f(1140037376u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1104515912u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937084u),f(3213565723u),f(2918484982u)},v[]={f(999639114u),f(2900158440u),f(1060714636u)},w[]={f(3200669894u),f(1040432686u),f(956333890u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1048645771u)++passed;else{++failed;printf("JV 43 %u expected 1048645771\n",bits(jv));}
float velocity=mul(-jv,f(1140136977u));float position=mul(f(3205833061u),f(1135317925u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3270977015u)++passed;else{++failed;printf("unbiased 43 %u expected 3270977015\n",bits(result.unbiased));}
if(bits(result.biased)==1117300557u)++passed;else{++failed;printf("biased 43 %u expected 1117300557\n",bits(result.biased));}
float bias=mul(0.7f,f(3205833061u));float generic=add(f(0u),mul(f(1140136977u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1117300557u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505686u),f(1069540014u),f(773389069u)},v[]={f(999639114u),f(910024916u),f(1063483391u)},w[]={f(1019714695u),f(3161499888u),f(976042445u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1062761955u)++passed;else{++failed;printf("JV 44 %u expected 1062761955\n",bits(jv));}
float velocity=mul(-jv,f(1137294252u));float position=mul(f(3201534665u),f(1133328018u));
auto result=solve(velocity,position,f(0u),f(3271643703u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3286973530u)++passed;else{++failed;printf("unbiased 44 %u expected 3286973530\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 44 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3201534665u));float generic=add(f(0u),mul(f(1137294252u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937030u),f(1069540014u),f(763993440u)},v[]={f(999639114u),f(910024915u),f(1063621848u)},w[]={f(997160312u),f(3147651208u),f(975575737u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063553549u)++passed;else{++failed;printf("JV 45 %u expected 1063553549\n",bits(jv));}
float velocity=mul(-jv,f(1137361944u));float position=mul(f(3200269265u),f(1133375402u));
auto result=solve(velocity,position,f(0u),f(3273146240u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3288371678u)++passed;else{++failed;printf("unbiased 45 %u expected 3288371678\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 45 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3200269265u));float generic=add(f(0u),mul(f(1137361944u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505724u),f(3213565826u),f(2904057632u)},v[]={f(999639114u),f(910024915u),f(1063698903u)},w[]={f(1014472323u),f(976727064u),f(975841811u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063467794u)++passed;else{++failed;printf("JV 46 %u expected 1063467794\n",bits(jv));}
float velocity=mul(-jv,f(1140037376u));float position=mul(f(3205458661u),f(1135248205u));
auto result=solve(velocity,position,f(0u),f(3273939629u),f(1104515912u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3289723448u)++passed;else{++failed;printf("unbiased 46 %u expected 3289723448\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 46 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3205458661u));float generic=add(f(1104515912u),mul(f(1140037376u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937084u),f(3213565723u),f(2918484982u)},v[]={f(999639114u),f(910024915u),f(1063663987u)},w[]={f(1017551292u),f(992240776u),f(975962590u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063910305u)++passed;else{++failed;printf("JV 47 %u expected 1063910305\n",bits(jv));}
float velocity=mul(-jv,f(1140136977u));float position=mul(f(3205833061u),f(1135317925u));
auto result=solve(velocity,position,f(0u),f(3270977015u),f(1117300557u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3289311355u)++passed;else{++failed;printf("unbiased 47 %u expected 3289311355\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 47 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3205833061u));float generic=add(f(1117300557u),mul(f(1140136977u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505686u),f(1069540014u),f(773389069u)},v[]={f(999639114u),f(910024916u),f(1063399054u)},w[]={f(3164351158u),f(1015343454u),f(975071160u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1064082503u)++passed;else{++failed;printf("JV 48 %u expected 1064082503\n",bits(jv));}
float velocity=mul(-jv,f(1137294252u));float position=mul(f(3201534665u),f(1133328018u));
auto result=solve(velocity,position,f(0u),f(3286973530u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3293763650u)++passed;else{++failed;printf("unbiased 48 %u expected 3293763650\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 48 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3201534665u));float generic=add(f(0u),mul(f(1137294252u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937030u),f(1069540014u),f(763993440u)},v[]={f(999639114u),f(910024916u),f(1063323058u)},w[]={f(3153863260u),f(1009838452u),f(975327326u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063484130u)++passed;else{++failed;printf("JV 49 %u expected 1063484130\n",bits(jv));}
float velocity=mul(-jv,f(1137361944u));float position=mul(f(3200269265u),f(1133375402u));
auto result=solve(velocity,position,f(0u),f(3288371678u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3294275705u)++passed;else{++failed;printf("unbiased 49 %u expected 3294275705\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 49 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3200269265u));float generic=add(f(0u),mul(f(1137361944u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505724u),f(3213565826u),f(2904057632u)},v[]={f(999639114u),f(910024916u),f(1063377077u)},w[]={f(967954720u),f(1013980372u),f(975513855u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063104933u)++passed;else{++failed;printf("JV 50 %u expected 1063104933\n",bits(jv));}
float velocity=mul(-jv,f(1140037376u));float position=mul(f(3205458661u),f(1135248205u));
auto result=solve(velocity,position,f(0u),f(3289723448u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3296635754u)++passed;else{++failed;printf("unbiased 50 %u expected 3296635754\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 50 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3205458661u));float generic=add(f(0u),mul(f(1140037376u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937084u),f(3213565723u),f(2918484982u)},v[]={f(999639114u),f(910024916u),f(1063406372u)},w[]={f(3146022442u),f(1012351797u),f(975412521u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063108722u)++passed;else{++failed;printf("JV 51 %u expected 1063108722\n",bits(jv));}
float velocity=mul(-jv,f(1140136977u));float position=mul(f(3205833061u),f(1135317925u));
auto result=solve(velocity,position,f(0u),f(3289311355u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3296268602u)++passed;else{++failed;printf("unbiased 51 %u expected 3296268602\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 51 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3205833061u));float generic=add(f(0u),mul(f(1140136977u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505686u),f(1069540014u),f(773389069u)},v[]={f(999639114u),f(910024916u),f(1063368973u)},w[]={f(3156041490u),f(1014430771u),f(975286685u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063886871u)++passed;else{++failed;printf("JV 52 %u expected 1063886871\n",bits(jv));}
float velocity=mul(-jv,f(1137294252u));float position=mul(f(3201534665u),f(1133328018u));
auto result=solve(velocity,position,f(0u),f(3293763650u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3298259615u)++passed;else{++failed;printf("unbiased 52 %u expected 3298259615\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 52 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3201534665u));float generic=add(f(0u),mul(f(1137294252u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937030u),f(1069540014u),f(763993440u)},v[]={f(999639114u),f(910024916u),f(1063347274u)},w[]={f(3150995410u),f(1012767034u),f(975359828u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063596121u)++passed;else{++failed;printf("JV 53 %u expected 1063596121\n",bits(jv));}
float velocity=mul(-jv,f(1137361944u));float position=mul(f(3200269265u),f(1133375402u));
auto result=solve(velocity,position,f(0u),f(3294275705u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3298473514u)++passed;else{++failed;printf("unbiased 53 %u expected 3298473514\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 53 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3200269265u));float generic=add(f(0u),mul(f(1137361944u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505724u),f(3213565826u),f(2904057632u)},v[]={f(999639114u),f(910024916u),f(1063365834u)},w[]={f(3144407710u),f(1014190125u),f(975423916u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063147017u)++passed;else{++failed;printf("JV 54 %u expected 1063147017\n",bits(jv));}
float velocity=mul(-jv,f(1140037376u));float position=mul(f(3205458661u),f(1135248205u));
auto result=solve(velocity,position,f(0u),f(3296635754u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3300145513u)++passed;else{++failed;printf("unbiased 54 %u expected 3300145513\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 54 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3205458661u));float generic=add(f(0u),mul(f(1140037376u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937084u),f(3213565723u),f(2918484982u)},v[]={f(999639114u),f(910024916u),f(1063376267u)},w[]={f(3148419558u),f(1013610136u),f(975387828u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063041216u)++passed;else{++failed;printf("JV 55 %u expected 1063041216\n",bits(jv));}
float velocity=mul(-jv,f(1140136977u));float position=mul(f(3205833061u),f(1135317925u));
auto result=solve(velocity,position,f(0u),f(3296268602u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3299958238u)++passed;else{++failed;printf("unbiased 55 %u expected 3299958238\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 55 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(3205833061u));float generic=add(f(0u),mul(f(1140136977u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505686u),f(1069540014u),f(773389069u)},v[]={f(999639114u),f(910024916u),f(1063364875u)},w[]={f(3152057687u),f(1014243393u),f(975349498u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063838636u)++passed;else{++failed;printf("JV 56 %u expected 1063838636\n",bits(jv));}
float velocity=mul(-jv,f(1137294252u));float position=mul(f(0u),f(1133328018u));
auto result=solve(velocity,position,f(0u),f(3298259615u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3301266430u)++passed;else{++failed;printf("unbiased 56 %u expected 3301266430\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 56 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(0u),mul(f(1137294252u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937030u),f(1069540014u),f(763993440u)},v[]={f(999639114u),f(910024917u),f(1063027321u)},w[]={f(1026736978u),f(3155610355u),f(976487316u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063422602u)++passed;else{++failed;printf("JV 57 %u expected 1063422602\n",bits(jv));}
float velocity=mul(-jv,f(1137361944u));float position=mul(f(0u),f(1133375402u));
auto result=solve(velocity,position,f(0u),f(3298473514u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3301413344u)++passed;else{++failed;printf("unbiased 57 %u expected 3301413344\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 57 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(0u),mul(f(1137361944u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(3210505724u),f(3213565826u),f(2904057632u)},v[]={f(999639114u),f(910024918u),f(1062732406u)},w[]={f(3110285056u),f(3170364210u),f(975468959u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063287394u)++passed;else{++failed;printf("JV 58 %u expected 1063287394\n",bits(jv));}
float velocity=mul(-jv,f(1140037376u));float position=mul(f(0u),f(1135248205u));
auto result=solve(velocity,position,f(0u),f(3300145513u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3303645070u)++passed;else{++failed;printf("unbiased 58 %u expected 3303645070\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 58 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(0u),mul(f(1140037376u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(759437056u),f(2912143360u),f(1065353214u)},j[]={f(1062937084u),f(3213565723u),f(2918484982u)},v[]={f(999639114u),f(910024919u),f(1062446994u)},w[]={f(1026425018u),f(3162356397u),f(976456232u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1063339462u)++passed;else{++failed;printf("JV 59 %u expected 1063339462\n",bits(jv));}
float velocity=mul(-jv,f(1140136977u));float position=mul(f(0u),f(1135317925u));
auto result=solve(velocity,position,f(0u),f(3299958238u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==3303492092u)++passed;else{++failed;printf("unbiased 59 %u expected 3303492092\n",bits(result.unbiased));}
if(bits(result.biased)==0u)++passed;else{++failed;printf("biased 59 %u expected 0\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(0u),mul(f(1140136977u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=0u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534880u),f(1066192128u),f(0u)},v[]={f(0u),f(0u),f(3243944614u)},w[]={f(0u),f(0u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3243944614u)++passed;else{++failed;printf("JV 60 %u expected 3243944614\n",bits(jv));}
float velocity=mul(-jv,f(1141155330u));float position=mul(f(3217310357u),f(1136244022u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1172478868u)++passed;else{++failed;printf("unbiased 60 %u expected 1172478868\n",bits(result.unbiased));}
if(bits(result.biased)==1173645173u)++passed;else{++failed;printf("biased 60 %u expected 1173645173\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(0u),mul(f(1141155330u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1173645173u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051130u),f(1066192128u),f(2147483648u)},v[]={f(0u),f(0u),f(3239774215u)},w[]={f(3235064977u),f(1082385671u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3241815230u)++passed;else{++failed;printf("JV 61 %u expected 3241815230\n",bits(jv));}
float velocity=mul(-jv,f(1141155377u));float position=mul(f(3217310357u),f(1136244088u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1170272221u)++passed;else{++failed;printf("unbiased 61 %u expected 1170272221\n",bits(result.unbiased));}
if(bits(result.biased)==1171438533u)++passed;else{++failed;printf("biased 61 %u expected 1171438533\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(0u),mul(f(1141155377u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1171438533u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534784u),f(3213675773u),f(0u)},v[]={f(0u),f(0u),f(3234334743u)},w[]={f(3211057238u),f(1089858669u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3244112554u)++passed;else{++failed;printf("JV 62 %u expected 3244112554\n",bits(jv));}
float velocity=mul(-jv,f(1141155375u));float position=mul(f(3217310837u),f(1136244085u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1172652985u)++passed;else{++failed;printf("unbiased 62 %u expected 1172652985\n",bits(result.unbiased));}
if(bits(result.biased)==1173819340u)++passed;else{++failed;printf("biased 62 %u expected 1173819340\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310837u));float generic=add(f(0u),mul(f(1141155375u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1173819340u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051136u),f(3213675728u),f(0u)},v[]={f(0u),f(0u),f(3222195478u)},w[]={f(3237087543u),f(1080114407u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3243801297u)++passed;else{++failed;printf("JV 63 %u expected 3243801297\n",bits(jv));}
float velocity=mul(-jv,f(1141155401u));float position=mul(f(3217310837u),f(1136244121u));
auto result=solve(velocity,position,f(0u),f(0u),f(0u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1172330466u)++passed;else{++failed;printf("unbiased 63 %u expected 1172330466\n",bits(result.unbiased));}
if(bits(result.biased)==1173496824u)++passed;else{++failed;printf("biased 63 %u expected 1173496824\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310837u));float generic=add(f(0u),mul(f(1141155401u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1173496824u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534880u),f(1066192128u),f(0u)},v[]={f(0u),f(0u),f(1071306725u)},w[]={f(3213043003u),f(3205535523u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1074138417u)++passed;else{++failed;printf("JV 64 %u expected 1074138417\n",bits(jv));}
float velocity=mul(-jv,f(1141155330u));float position=mul(f(3217310357u),f(1136244022u));
auto result=solve(velocity,position,f(0u),f(1172478868u),f(1173645173u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1170202806u)++passed;else{++failed;printf("unbiased 64 %u expected 1170202806\n",bits(result.unbiased));}
if(bits(result.biased)==1172535417u)++passed;else{++failed;printf("biased 64 %u expected 1172535417\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1173645173u),mul(f(1141155330u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172535417u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051130u),f(1066192128u),f(2147483648u)},v[]={f(0u),f(0u),f(1069328762u)},w[]={f(3206685255u),f(3209635181u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==3172731552u)++passed;else{++failed;printf("JV 65 %u expected 3172731552\n",bits(jv));}
float velocity=mul(-jv,f(1141155377u));float position=mul(f(3217310357u),f(1136244088u));
auto result=solve(velocity,position,f(0u),f(1170272221u),f(1171438533u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1170313619u)++passed;else{++failed;printf("unbiased 65 %u expected 1170313619\n",bits(result.unbiased));}
if(bits(result.biased)==1172646242u)++passed;else{++failed;printf("biased 65 %u expected 1172646242\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1171438533u),mul(f(1141155377u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172646242u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534784u),f(3213675773u),f(0u)},v[]={f(0u),f(0u),f(1072096968u)},w[]={f(3182408614u),f(3203346970u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1075428206u)++passed;else{++failed;printf("JV 66 %u expected 1075428206\n",bits(jv));}
float velocity=mul(-jv,f(1141155375u));float position=mul(f(3217310837u),f(1136244085u));
auto result=solve(velocity,position,f(0u),f(1172652985u),f(1173819340u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1170042753u)++passed;else{++failed;printf("unbiased 66 %u expected 1170042753\n",bits(result.unbiased));}
if(bits(result.biased)==1172375463u)++passed;else{++failed;printf("biased 66 %u expected 1172375463\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310837u));float generic=add(f(1173819340u),mul(f(1141155375u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172375463u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051136u),f(3213675728u),f(0u)},v[]={f(0u),f(0u),f(1069434707u)},w[]={f(1054978063u),f(3188562383u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1074041807u)++passed;else{++failed;printf("JV 67 %u expected 1074041807\n",bits(jv));}
float velocity=mul(-jv,f(1141155401u));float position=mul(f(3217310837u),f(1136244121u));
auto result=solve(velocity,position,f(0u),f(1172330466u),f(1173496824u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1170079416u)++passed;else{++failed;printf("unbiased 67 %u expected 1170079416\n",bits(result.unbiased));}
if(bits(result.biased)==1172412132u)++passed;else{++failed;printf("biased 67 %u expected 1172412132\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310837u));float generic=add(f(1173496824u),mul(f(1141155401u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172412132u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534880u),f(1066192128u),f(0u)},v[]={f(0u),f(0u),f(1067508058u)},w[]={f(1031049888u),f(1036796522u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1067936505u)++passed;else{++failed;printf("JV 68 %u expected 1067936505\n",bits(jv));}
float velocity=mul(-jv,f(1141155330u));float position=mul(f(3217310357u),f(1136244022u));
auto result=solve(velocity,position,f(0u),f(1170202806u),f(1172535417u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1168781512u)++passed;else{++failed;printf("unbiased 68 %u expected 1168781512\n",bits(result.unbiased));}
if(bits(result.biased)==1172280428u)++passed;else{++failed;printf("biased 68 %u expected 1172280428\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1172535417u),mul(f(1141155330u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172280428u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051130u),f(1066192128u),f(2147483648u)},v[]={f(0u),f(0u),f(1067141098u)},w[]={f(1040676901u),f(1029624894u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1068732872u)++passed;else{++failed;printf("JV 69 %u expected 1068732872\n",bits(jv));}
float velocity=mul(-jv,f(1141155377u));float position=mul(f(3217310357u),f(1136244088u));
auto result=solve(velocity,position,f(0u),f(1170313619u),f(1172646242u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1168789156u)++passed;else{++failed;printf("unbiased 69 %u expected 1168789156\n",bits(result.unbiased));}
if(bits(result.biased)==1172288090u)++passed;else{++failed;printf("biased 69 %u expected 1172288090\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1172646242u),mul(f(1141155377u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172288090u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534784u),f(3213675773u),f(0u)},v[]={f(0u),f(0u),f(1066562860u)},w[]={f(1016253896u),f(3163261764u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066572423u)++passed;else{++failed;printf("JV 70 %u expected 1066572423\n",bits(jv));}
float velocity=mul(-jv,f(1141155375u));float position=mul(f(3217310837u),f(1136244085u));
auto result=solve(velocity,position,f(0u),f(1170042753u),f(1172375463u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1168798155u)++passed;else{++failed;printf("unbiased 70 %u expected 1168798155\n",bits(result.unbiased));}
if(bits(result.biased)==1172297220u)++passed;else{++failed;printf("biased 70 %u expected 1172297220\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310837u));float generic=add(f(1172375463u),mul(f(1141155375u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172297220u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051136u),f(3213675728u),f(0u)},v[]={f(0u),f(0u),f(1066557866u)},w[]={f(1016784215u),f(3162930536u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066865112u)++passed;else{++failed;printf("JV 71 %u expected 1066865112\n",bits(jv));}
float velocity=mul(-jv,f(1141155401u));float position=mul(f(3217310837u),f(1136244121u));
auto result=solve(velocity,position,f(0u),f(1170079416u),f(1172412132u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1168796899u)++passed;else{++failed;printf("unbiased 71 %u expected 1168796899\n",bits(result.unbiased));}
if(bits(result.biased)==1172295973u)++passed;else{++failed;printf("biased 71 %u expected 1172295973\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310837u));float generic=add(f(1172412132u),mul(f(1141155401u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172295973u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534880u),f(1066192128u),f(0u)},v[]={f(0u),f(0u),f(1066475221u)},w[]={f(992467320u),f(3150669332u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066396846u)++passed;else{++failed;printf("JV 72 %u expected 1066396846\n",bits(jv));}
float velocity=mul(-jv,f(1141155330u));float position=mul(f(3217310357u),f(1136244022u));
auto result=solve(velocity,position,f(0u),f(1168781512u),f(1172280428u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1167559664u)++passed;else{++failed;printf("unbiased 72 %u expected 1167559664\n",bits(result.unbiased));}
if(bits(result.biased)==1172224886u)++passed;else{++failed;printf("biased 72 %u expected 1172224886\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1172280428u),mul(f(1141155330u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172224886u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051130u),f(1066192128u),f(2147483648u)},v[]={f(0u),f(0u),f(1066443232u)},w[]={f(1007788216u),f(3156636237u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066422708u)++passed;else{++failed;printf("JV 73 %u expected 1066422708\n",bits(jv));}
float velocity=mul(-jv,f(1141155377u));float position=mul(f(3217310357u),f(1136244088u));
auto result=solve(velocity,position,f(0u),f(1168789156u),f(1172288090u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1167563952u)++passed;else{++failed;printf("unbiased 73 %u expected 1167563952\n",bits(result.unbiased));}
if(bits(result.biased)==1172229197u)++passed;else{++failed;printf("biased 73 %u expected 1172229197\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310357u));float generic=add(f(1172288090u),mul(f(1141155377u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172229197u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534784u),f(3213675773u),f(0u)},v[]={f(0u),f(0u),f(1066404382u)},w[]={f(983420336u),f(3161789746u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066532495u)++passed;else{++failed;printf("JV 74 %u expected 1066532495\n",bits(jv));}
float velocity=mul(-jv,f(1141155375u));float position=mul(f(3217310837u),f(1136244085u));
auto result=solve(velocity,position,f(0u),f(1168798155u),f(1172297220u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1167558729u)++passed;else{++failed;printf("unbiased 74 %u expected 1167558729\n",bits(result.unbiased));}
if(bits(result.biased)==1172224149u)++passed;else{++failed;printf("biased 74 %u expected 1172224149\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310837u));float generic=add(f(1172297220u),mul(f(1141155375u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172224149u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051136u),f(3213675728u),f(0u)},v[]={f(0u),f(0u),f(1066336490u)},w[]={f(1013956248u),f(3151451002u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066517665u)++passed;else{++failed;printf("JV 75 %u expected 1066517665\n",bits(jv));}
float velocity=mul(-jv,f(1141155401u));float position=mul(f(3217310837u),f(1136244121u));
auto result=solve(velocity,position,f(0u),f(1168796899u),f(1172295973u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1167559390u)++passed;else{++failed;printf("unbiased 75 %u expected 1167559390\n",bits(result.unbiased));}
if(bits(result.biased)==1172224823u)++passed;else{++failed;printf("biased 75 %u expected 1172224823\n",bits(result.biased));}
float bias=mul(0.7f,f(3217310837u));float generic=add(f(1172295973u),mul(f(1141155401u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1172224823u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534880u),f(1066192128u),f(0u)},v[]={f(0u),f(0u),f(1066272531u)},w[]={f(989979824u),f(984504872u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066268478u)++passed;else{++failed;printf("JV 76 %u expected 1066268478\n",bits(jv));}
float velocity=mul(-jv,f(1141155330u));float position=mul(f(0u),f(1136244022u));
auto result=solve(velocity,position,f(0u),f(1167559664u),f(1172224886u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1166354445u)++passed;else{++failed;printf("unbiased 76 %u expected 1166354445\n",bits(result.unbiased));}
if(bits(result.biased)==1171019667u)++passed;else{++failed;printf("biased 76 %u expected 1171019667\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1172224886u),mul(f(1141155330u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1171019667u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051130u),f(1066192128u),f(2147483648u)},v[]={f(0u),f(0u),f(1064577553u)},w[]={f(1048929453u),f(3190030281u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1065633711u)++passed;else{++failed;printf("JV 77 %u expected 1065633711\n",bits(jv));}
float velocity=mul(-jv,f(1141155377u));float position=mul(f(0u),f(1136244088u));
auto result=solve(velocity,position,f(0u),f(1167563952u),f(1172229197u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1166440954u)++passed;else{++failed;printf("unbiased 77 %u expected 1166440954\n",bits(result.unbiased));}
if(bits(result.biased)==1171106200u)++passed;else{++failed;printf("biased 77 %u expected 1171106200\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1172229197u),mul(f(1141155377u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1171106200u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(3212534784u),f(3213675773u),f(0u)},v[]={f(0u),f(0u),f(1062300047u)},w[]={f(1024495060u),f(3197765474u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066311899u)++passed;else{++failed;printf("JV 78 %u expected 1066311899\n",bits(jv));}
float velocity=mul(-jv,f(1141155375u));float position=mul(f(0u),f(1136244085u));
auto result=solve(velocity,position,f(0u),f(1167558729u),f(1172224149u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1166347879u)++passed;else{++failed;printf("unbiased 78 %u expected 1166347879\n",bits(result.unbiased));}
if(bits(result.biased)==1171013299u)++passed;else{++failed;printf("biased 78 %u expected 1171013299\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1172224149u),mul(f(1141155375u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1171013299u)++generic_mismatch;
}
{
float n[]={f(0u),f(0u),f(1065353216u)},j[]={f(1065051136u),f(3213675728u),f(0u)},v[]={f(0u),f(0u),f(1059662784u)},w[]={f(1050123308u),f(3188538969u),f(0u)};
float jv=body0_jv(n,j,v,w);
if(bits(jv)==1066220028u)++passed;else{++failed;printf("JV 79 %u expected 1066220028\n",bits(jv));}
float velocity=mul(-jv,f(1141155401u));float position=mul(f(0u),f(1136244121u));
auto result=solve(velocity,position,f(0u),f(1167559390u),f(1172224823u),1.0f,f(0u),f(2139095039u));
if(bits(result.unbiased)==1166360438u)++passed;else{++failed;printf("unbiased 79 %u expected 1166360438\n",bits(result.unbiased));}
if(bits(result.biased)==1171025870u)++passed;else{++failed;printf("biased 79 %u expected 1171025870\n",bits(result.biased));}
float bias=mul(0.7f,f(0u));float generic=add(f(1172224823u),mul(f(1141155401u),sub(-jv,bias)));
if(generic<f(0u))generic=f(0u);if(generic>f(2139095039u))generic=f(2139095039u);
if(bits(generic)!=1171025870u)++generic_mismatch;
}
printf("{\"passed\":%u,\"failed\":%u,\"generic_mismatches\":%u}\n",passed,failed,generic_mismatch);return failed?1:0;}
