#include "NativeWheelFrameV57.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
float f(uint32_t w){float x;memcpy(&x,&w,4);return x;}
uint32_t bits(float x){uint32_t w;memcpy(&w,&x,4);return w;}
int main(){using namespace motorrock_native_wheel_frame_v57;unsigned ok=0,failed=0;
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(3213676184u),f(3208643649u),f(3200741776u)}};
Vector local_axis{{f(1047306768u),f(3136620956u),f(1064899106u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2982794752u){printf("fail row 0 prepared_axis 0 %u expected 2982794752\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2944401408u){printf("fail row 0 prepared_axis 1 %u expected 2944401408\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353243u){printf("fail row 0 prepared_axis 2 %u expected 1065353243\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3208688877u){printf("fail row 0 jacobian 0 %u expected 3208688877\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065002711u){printf("fail row 0 jacobian 1 %u expected 1065002711\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2979011024u){printf("fail row 0 jacobian 2 %u expected 2979011024\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(3213675434u),f(1061159400u),f(3200645770u)}};
Vector local_axis{{f(1047306768u),f(3136620956u),f(1064899106u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2982794752u){printf("fail row 1 prepared_axis 0 %u expected 2982794752\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2944401408u){printf("fail row 1 prepared_axis 1 %u expected 2944401408\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353243u){printf("fail row 1 prepared_axis 2 %u expected 1065353243\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1061114292u){printf("fail row 1 jacobian 0 %u expected 1061114292\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065051128u){printf("fail row 1 jacobian 1 %u expected 1065051128\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=832483859u){printf("fail row 1 jacobian 2 %u expected 832483859\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(3213676184u),f(3208643649u),f(3200741776u)}};
Vector local_axis{{f(1047306768u),f(3136620956u),f(1064899106u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2982794752u){printf("fail row 2 prepared_axis 0 %u expected 2982794752\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2944401408u){printf("fail row 2 prepared_axis 1 %u expected 2944401408\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353243u){printf("fail row 2 prepared_axis 2 %u expected 1065353243\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3208688877u){printf("fail row 2 jacobian 0 %u expected 3208688877\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065002711u){printf("fail row 2 jacobian 1 %u expected 1065002711\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2979011024u){printf("fail row 2 jacobian 2 %u expected 2979011024\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(3213675434u),f(1061159400u),f(3200645770u)}};
Vector local_axis{{f(1047306768u),f(3136620956u),f(1064899106u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2982794752u){printf("fail row 3 prepared_axis 0 %u expected 2982794752\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2944401408u){printf("fail row 3 prepared_axis 1 %u expected 2944401408\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353243u){printf("fail row 3 prepared_axis 2 %u expected 1065353243\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1061114292u){printf("fail row 3 jacobian 0 %u expected 1061114292\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065051128u){printf("fail row 3 jacobian 1 %u expected 1065051128\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=832483859u){printf("fail row 3 jacobian 2 %u expected 832483859\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(1066192034u),f(3208642744u),f(3206161077u)}};
Vector local_axis{{f(3040735185u),f(3134885644u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3194790485u){printf("fail row 4 prepared_axis 0 %u expected 3194790485\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3109462894u){printf("fail row 4 prepared_axis 1 %u expected 3109462894\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1064899155u){printf("fail row 4 prepared_axis 2 %u expected 1064899155\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3208283009u){printf("fail row 4 jacobian 0 %u expected 3208283009\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213687870u){printf("fail row 4 jacobian 1 %u expected 3213687870\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3190910968u){printf("fail row 4 jacobian 2 %u expected 3190910968\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(1066192279u),f(1061159240u),f(3206119113u)}};
Vector local_axis{{f(3040735185u),f(3134885644u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3194790485u){printf("fail row 5 prepared_axis 0 %u expected 3194790485\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3109462894u){printf("fail row 5 prepared_axis 1 %u expected 3109462894\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1064899155u){printf("fail row 5 prepared_axis 2 %u expected 1064899155\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1060837895u){printf("fail row 5 jacobian 0 %u expected 1060837895\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213663813u){printf("fail row 5 jacobian 1 %u expected 3213663813\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1043432867u){printf("fail row 5 jacobian 2 %u expected 1043432867\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(3213676184u),f(3208643649u),f(3200741776u)}};
Vector local_axis{{f(971119136u),f(1065353214u),f(988704191u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3133806131u){printf("fail row 6 prepared_axis 0 %u expected 3133806131\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353233u){printf("fail row 6 prepared_axis 1 %u expected 1065353233\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2955935744u){printf("fail row 6 prepared_axis 2 %u expected 2955935744\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1059175877u){printf("fail row 6 jacobian 0 %u expected 1059175877\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=981424988u){printf("fail row 6 jacobian 1 %u expected 981424988\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3212505811u){printf("fail row 6 jacobian 2 %u expected 3212505811\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(3213676184u),f(3208643649u),f(3200741776u)}};
Vector local_axis{{f(1064899135u),f(894532352u),f(3194790441u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353232u){printf("fail row 7 prepared_axis 0 %u expected 1065353232\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=986322493u){printf("fail row 7 prepared_axis 1 %u expected 986322493\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=851988608u){printf("fail row 7 prepared_axis 2 %u expected 851988608\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=981424709u){printf("fail row 7 jacobian 0 %u expected 981424709\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3206659524u){printf("fail row 7 jacobian 1 %u expected 3206659524\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1061179885u){printf("fail row 7 jacobian 2 %u expected 1061179885\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(3213675434u),f(1061159400u),f(3200645770u)}};
Vector local_axis{{f(971119136u),f(1065353214u),f(988704191u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3133806131u){printf("fail row 8 prepared_axis 0 %u expected 3133806131\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353233u){printf("fail row 8 prepared_axis 1 %u expected 1065353233\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2955935744u){printf("fail row 8 prepared_axis 2 %u expected 2955935744\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1059175877u){printf("fail row 8 jacobian 0 %u expected 1059175877\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=981424988u){printf("fail row 8 jacobian 1 %u expected 981424988\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3212515425u){printf("fail row 8 jacobian 2 %u expected 3212515425\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(3213675434u),f(1061159400u),f(3200645770u)}};
Vector local_axis{{f(1064899135u),f(894532352u),f(3194790441u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353232u){printf("fail row 9 prepared_axis 0 %u expected 1065353232\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=986322493u){printf("fail row 9 prepared_axis 1 %u expected 986322493\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=851988608u){printf("fail row 9 prepared_axis 2 %u expected 851988608\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=981425310u){printf("fail row 9 jacobian 0 %u expected 981425310\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3206659524u){printf("fail row 9 jacobian 1 %u expected 3206659524\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3208623325u){printf("fail row 9 jacobian 2 %u expected 3208623325\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(1066192034u),f(3208642744u),f(3206161077u)}};
Vector local_axis{{f(3040923853u),f(1065353216u),f(987401996u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3137115400u){printf("fail row 10 prepared_axis 0 %u expected 3137115400\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353225u){printf("fail row 10 prepared_axis 1 %u expected 1065353225\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3112282689u){printf("fail row 10 prepared_axis 2 %u expected 3112282689\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1051273243u){printf("fail row 10 jacobian 0 %u expected 1051273243\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=980764932u){printf("fail row 10 jacobian 1 %u expected 980764932\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1067107045u){printf("fail row 10 jacobian 2 %u expected 1067107045\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(1066192034u),f(3208642744u),f(3206161077u)}};
Vector local_axis{{f(1065353228u),f(893419432u),f(893272591u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1064899124u){printf("fail row 11 prepared_axis 0 %u expected 1064899124\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=989674638u){printf("fail row 11 prepared_axis 1 %u expected 989674638\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1047306836u){printf("fail row 11 prepared_axis 2 %u expected 1047306836\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3190851525u){printf("fail row 11 jacobian 0 %u expected 3190851525\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3206142700u){printf("fail row 11 jacobian 1 %u expected 3206142700\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1060837442u){printf("fail row 11 jacobian 2 %u expected 1060837442\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(1066192279u),f(1061159240u),f(3206119113u)}};
Vector local_axis{{f(3040923853u),f(1065353216u),f(987401996u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3137115400u){printf("fail row 12 prepared_axis 0 %u expected 3137115400\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353225u){printf("fail row 12 prepared_axis 1 %u expected 1065353225\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3112282689u){printf("fail row 12 prepared_axis 2 %u expected 3112282689\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1051273009u){printf("fail row 12 jacobian 0 %u expected 1051273009\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=980764826u){printf("fail row 12 jacobian 1 %u expected 980764826\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1067107292u){printf("fail row 12 jacobian 2 %u expected 1067107292\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3126583376u),f(3186507929u),f(979705444u),f(1065239295u)}};
Vector local_point{{f(1066192279u),f(1061159240u),f(3206119113u)}};
Vector local_axis{{f(1065353228u),f(893419432u),f(893272591u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1064899124u){printf("fail row 13 prepared_axis 0 %u expected 1064899124\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=989674638u){printf("fail row 13 prepared_axis 1 %u expected 989674638\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1047306836u){printf("fail row 13 prepared_axis 2 %u expected 1047306836\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1043492315u){printf("fail row 13 jacobian 0 %u expected 1043492315\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3206137548u){printf("fail row 13 jacobian 1 %u expected 3206137548\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3208283510u){printf("fail row 13 jacobian 2 %u expected 3208283510\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3143766580u),f(3190903518u),f(996252496u),f(1065099556u)}};
Vector local_point{{f(3213675956u),f(3208642495u),f(3199851722u)}};
Vector local_axis{{f(1051631978u),f(3154304069u),f(1064346644u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3001871360u){printf("fail row 14 prepared_axis 0 %u expected 3001871360\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2966421504u){printf("fail row 14 prepared_axis 1 %u expected 2966421504\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353241u){printf("fail row 14 prepared_axis 2 %u expected 1065353241\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3208823268u){printf("fail row 14 jacobian 0 %u expected 3208823268\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1063776346u){printf("fail row 14 jacobian 1 %u expected 1063776346\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2997384948u){printf("fail row 14 jacobian 2 %u expected 2997384948\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3143766580u),f(3190903518u),f(996252496u),f(1065099556u)}};
Vector local_point{{f(3213675841u),f(1061159267u),f(3199424223u)}};
Vector local_axis{{f(1051631978u),f(3154304069u),f(1064346644u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3001871360u){printf("fail row 15 prepared_axis 0 %u expected 3001871360\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2966421504u){printf("fail row 15 prepared_axis 1 %u expected 2966421504\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353241u){printf("fail row 15 prepared_axis 2 %u expected 1065353241\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1060978610u){printf("fail row 15 jacobian 0 %u expected 1060978610\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1063989628u){printf("fail row 15 jacobian 1 %u expected 1063989628\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=851121806u){printf("fail row 15 jacobian 2 %u expected 851121806\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3143766580u),f(3190903518u),f(996252496u),f(1065099556u)}};
Vector local_point{{f(3213675956u),f(3208642495u),f(3199851722u)}};
Vector local_axis{{f(1051631978u),f(3154304069u),f(1064346644u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3001871360u){printf("fail row 16 prepared_axis 0 %u expected 3001871360\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2966421504u){printf("fail row 16 prepared_axis 1 %u expected 2966421504\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353241u){printf("fail row 16 prepared_axis 2 %u expected 1065353241\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3208823268u){printf("fail row 16 jacobian 0 %u expected 3208823268\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1063776346u){printf("fail row 16 jacobian 1 %u expected 1063776346\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2997384948u){printf("fail row 16 jacobian 2 %u expected 2997384948\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3143766580u),f(3190903518u),f(996252496u),f(1065099556u)}};
Vector local_point{{f(3213675841u),f(1061159267u),f(3199424223u)}};
Vector local_axis{{f(1051631978u),f(3154304069u),f(1064346644u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3001871360u){printf("fail row 17 prepared_axis 0 %u expected 3001871360\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2966421504u){printf("fail row 17 prepared_axis 1 %u expected 2966421504\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353241u){printf("fail row 17 prepared_axis 2 %u expected 1065353241\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1060978610u){printf("fail row 17 jacobian 0 %u expected 1060978610\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1063989628u){printf("fail row 17 jacobian 1 %u expected 1063989628\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=851121806u){printf("fail row 17 jacobian 2 %u expected 851121806\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3143766580u),f(3190903518u),f(996252496u),f(1065099556u)}};
Vector local_point{{f(3213675956u),f(3208642495u),f(3199851722u)}};
Vector local_axis{{f(993164765u),f(1065352708u),f(1005979860u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3149337622u){printf("fail row 18 prepared_axis 0 %u expected 3149337622\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353006u){printf("fail row 18 prepared_axis 1 %u expected 1065353006\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2973237248u){printf("fail row 18 prepared_axis 2 %u expected 2973237248\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1060494973u){printf("fail row 18 jacobian 0 %u expected 1060494973\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=998379702u){printf("fail row 18 jacobian 1 %u expected 998379702\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3211331071u){printf("fail row 18 jacobian 2 %u expected 3211331071\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3143766580u),f(3190903518u),f(996252496u),f(1065099556u)}};
Vector local_point{{f(3213675956u),f(3208642495u),f(3199851722u)}};
Vector local_axis{{f(1064347148u),f(894197760u),f(3199115991u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353004u){printf("fail row 19 prepared_axis 0 %u expected 1065353004\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1001853984u){printf("fail row 19 prepared_axis 1 %u expected 1001853984\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=848465920u){printf("fail row 19 prepared_axis 2 %u expected 848465920\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=998379686u){printf("fail row 19 jacobian 0 %u expected 998379686\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3207978619u){printf("fail row 19 jacobian 1 %u expected 3207978619\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1061254495u){printf("fail row 19 jacobian 2 %u expected 1061254495\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3143766580u),f(3190903518u),f(996252496u),f(1065099556u)}};
Vector local_point{{f(3213675841u),f(1061159267u),f(3199424223u)}};
Vector local_axis{{f(993164765u),f(1065352708u),f(1005979860u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3149337622u){printf("fail row 20 prepared_axis 0 %u expected 3149337622\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353006u){printf("fail row 20 prepared_axis 1 %u expected 1065353006\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2973237248u){printf("fail row 20 prepared_axis 2 %u expected 2973237248\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1060494974u){printf("fail row 20 jacobian 0 %u expected 1060494974\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=998379703u){printf("fail row 20 jacobian 1 %u expected 998379703\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3211403742u){printf("fail row 20 jacobian 2 %u expected 3211403742\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(3143766580u),f(3190903518u),f(996252496u),f(1065099556u)}};
Vector local_point{{f(3213675841u),f(1061159267u),f(3199424223u)}};
Vector local_axis{{f(1064347148u),f(894197760u),f(3199115991u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353004u){printf("fail row 21 prepared_axis 0 %u expected 1065353004\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1001853984u){printf("fail row 21 prepared_axis 1 %u expected 1001853984\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=848465920u){printf("fail row 21 prepared_axis 2 %u expected 848465920\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=998379742u){printf("fail row 21 jacobian 0 %u expected 998379742\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3207978620u){printf("fail row 21 jacobian 1 %u expected 3207978620\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3208548181u){printf("fail row 21 jacobian 2 %u expected 3208548181\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3217022771u),f(3210504760u),f(1033550848u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 22 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 22 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 22 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3210504760u){printf("fail row 22 jacobian 0 %u expected 3210504760\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1069539123u){printf("fail row 22 jacobian 1 %u expected 1069539123\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 22 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3217022771u),f(3210504760u),f(1033550848u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 23 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 23 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 23 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3210504760u){printf("fail row 23 jacobian 0 %u expected 3210504760\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1069539123u){printf("fail row 23 jacobian 1 %u expected 1069539123\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 23 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3217022515u),f(1062937712u),f(1033550848u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 24 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 24 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 24 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1062937712u){printf("fail row 24 jacobian 0 %u expected 1062937712\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1069538867u){printf("fail row 24 jacobian 1 %u expected 1069538867\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2147483648u){printf("fail row 24 jacobian 2 %u expected 2147483648\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3217022515u),f(1062937712u),f(1033550848u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 25 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 25 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 25 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1062937712u){printf("fail row 25 jacobian 0 %u expected 1062937712\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1069538867u){printf("fail row 25 jacobian 1 %u expected 1069538867\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2147483648u){printf("fail row 25 jacobian 2 %u expected 2147483648\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066083182u),f(3210504448u),f(1033550976u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 26 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 26 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 26 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3210504448u){printf("fail row 26 jacobian 0 %u expected 3210504448\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213566830u){printf("fail row 26 jacobian 1 %u expected 3213566830\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 26 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066083182u),f(3210504448u),f(1033550976u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 27 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 27 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 27 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3210504448u){printf("fail row 27 jacobian 0 %u expected 3210504448\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213566830u){printf("fail row 27 jacobian 1 %u expected 3213566830\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 27 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066082843u),f(1062936832u),f(1033550976u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 28 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 28 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 28 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1062936832u){printf("fail row 28 jacobian 0 %u expected 1062936832\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213566491u){printf("fail row 28 jacobian 1 %u expected 3213566491\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 28 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066082843u),f(1062936832u),f(1033550976u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 29 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 29 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 29 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1062936832u){printf("fail row 29 jacobian 0 %u expected 1062936832\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213566491u){printf("fail row 29 jacobian 1 %u expected 3213566491\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 29 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3217022771u),f(3210504760u),f(1033550848u)}};
Vector local_axis{{f(0u),f(1065353216u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 30 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353216u){printf("fail row 30 prepared_axis 1 %u expected 1065353216\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 30 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3181034496u){printf("fail row 30 jacobian 0 %u expected 3181034496\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 30 jacobian 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3217022771u){printf("fail row 30 jacobian 2 %u expected 3217022771\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3217022771u),f(3210504760u),f(1033550848u)}};
Vector local_axis{{f(1065353216u),f(0u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353216u){printf("fail row 31 prepared_axis 0 %u expected 1065353216\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 31 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 31 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=2147483648u){printf("fail row 31 jacobian 0 %u expected 2147483648\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1033550848u){printf("fail row 31 jacobian 1 %u expected 1033550848\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1063021112u){printf("fail row 31 jacobian 2 %u expected 1063021112\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3217022515u),f(1062937712u),f(1033550848u)}};
Vector local_axis{{f(0u),f(1065353216u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 32 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353216u){printf("fail row 32 prepared_axis 1 %u expected 1065353216\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 32 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3181034496u){printf("fail row 32 jacobian 0 %u expected 3181034496\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 32 jacobian 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3217022515u){printf("fail row 32 jacobian 2 %u expected 3217022515\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3217022515u),f(1062937712u),f(1033550848u)}};
Vector local_axis{{f(1065353216u),f(0u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353216u){printf("fail row 33 prepared_axis 0 %u expected 1065353216\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 33 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 33 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=0u){printf("fail row 33 jacobian 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1033550848u){printf("fail row 33 jacobian 1 %u expected 1033550848\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3210421360u){printf("fail row 33 jacobian 2 %u expected 3210421360\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066083182u),f(3210504448u),f(1033550976u)}};
Vector local_axis{{f(0u),f(1065353216u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 34 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353216u){printf("fail row 34 prepared_axis 1 %u expected 1065353216\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 34 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3181034624u){printf("fail row 34 jacobian 0 %u expected 3181034624\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 34 jacobian 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1066083182u){printf("fail row 34 jacobian 2 %u expected 1066083182\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066083182u),f(3210504448u),f(1033550976u)}};
Vector local_axis{{f(1065353216u),f(0u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353216u){printf("fail row 35 prepared_axis 0 %u expected 1065353216\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 35 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 35 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=2147483648u){printf("fail row 35 jacobian 0 %u expected 2147483648\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1033550976u){printf("fail row 35 jacobian 1 %u expected 1033550976\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1063020800u){printf("fail row 35 jacobian 2 %u expected 1063020800\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066082843u),f(1062936832u),f(1033550976u)}};
Vector local_axis{{f(0u),f(1065353216u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 36 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353216u){printf("fail row 36 prepared_axis 1 %u expected 1065353216\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 36 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3181034624u){printf("fail row 36 jacobian 0 %u expected 3181034624\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 36 jacobian 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1066082843u){printf("fail row 36 jacobian 2 %u expected 1066082843\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066082843u),f(1062936832u),f(1033550976u)}};
Vector local_axis{{f(1065353216u),f(0u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353216u){printf("fail row 37 prepared_axis 0 %u expected 1065353216\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 37 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 37 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=0u){printf("fail row 37 jacobian 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1033550976u){printf("fail row 37 jacobian 1 %u expected 1033550976\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3210420480u){printf("fail row 37 jacobian 2 %u expected 3210420480\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(3217022920u),f(3210505091u),f(1030291877u)}};
Vector local_axis{{f(988185439u),f(968868166u),f(1065353188u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=759437056u){printf("fail row 38 prepared_axis 0 %u expected 759437056\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2912143360u){printf("fail row 38 prepared_axis 1 %u expected 2912143360\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353214u){printf("fail row 38 prepared_axis 2 %u expected 1065353214\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3210505686u){printf("fail row 38 jacobian 0 %u expected 3210505686\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1069540014u){printf("fail row 38 jacobian 1 %u expected 1069540014\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=773389069u){printf("fail row 38 jacobian 2 %u expected 773389069\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(3217022920u),f(3210505091u),f(1030291877u)}};
Vector local_axis{{f(988185439u),f(968868166u),f(1065353188u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=759437056u){printf("fail row 39 prepared_axis 0 %u expected 759437056\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2912143360u){printf("fail row 39 prepared_axis 1 %u expected 2912143360\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353214u){printf("fail row 39 prepared_axis 2 %u expected 1065353214\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3210505686u){printf("fail row 39 jacobian 0 %u expected 3210505686\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1069540014u){printf("fail row 39 jacobian 1 %u expected 1069540014\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=773389069u){printf("fail row 39 jacobian 2 %u expected 773389069\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(3217022777u),f(1062937629u),f(1030123037u)}};
Vector local_axis{{f(988185439u),f(968868166u),f(1065353188u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=759437056u){printf("fail row 40 prepared_axis 0 %u expected 759437056\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2912143360u){printf("fail row 40 prepared_axis 1 %u expected 2912143360\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353214u){printf("fail row 40 prepared_axis 2 %u expected 1065353214\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1062937030u){printf("fail row 40 jacobian 0 %u expected 1062937030\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1069540014u){printf("fail row 40 jacobian 1 %u expected 1069540014\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=763993440u){printf("fail row 40 jacobian 2 %u expected 763993440\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(3217022777u),f(1062937629u),f(1030123037u)}};
Vector local_axis{{f(988185439u),f(968868166u),f(1065353188u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=759437056u){printf("fail row 41 prepared_axis 0 %u expected 759437056\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2912143360u){printf("fail row 41 prepared_axis 1 %u expected 2912143360\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353214u){printf("fail row 41 prepared_axis 2 %u expected 1065353214\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1062937030u){printf("fail row 41 jacobian 0 %u expected 1062937030\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1069540014u){printf("fail row 41 jacobian 1 %u expected 1069540014\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=763993440u){printf("fail row 41 jacobian 2 %u expected 763993440\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(1066082889u),f(3210505588u),f(1029070795u)}};
Vector local_axis{{f(988185439u),f(968868166u),f(1065353188u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=759437056u){printf("fail row 42 prepared_axis 0 %u expected 759437056\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2912143360u){printf("fail row 42 prepared_axis 1 %u expected 2912143360\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353214u){printf("fail row 42 prepared_axis 2 %u expected 1065353214\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3210505724u){printf("fail row 42 jacobian 0 %u expected 3210505724\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213565826u){printf("fail row 42 jacobian 1 %u expected 3213565826\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2904057632u){printf("fail row 42 jacobian 2 %u expected 2904057632\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(1066082889u),f(3210505588u),f(1029070795u)}};
Vector local_axis{{f(988185439u),f(968868166u),f(1065353188u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=759437056u){printf("fail row 43 prepared_axis 0 %u expected 759437056\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2912143360u){printf("fail row 43 prepared_axis 1 %u expected 2912143360\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353214u){printf("fail row 43 prepared_axis 2 %u expected 1065353214\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3210505724u){printf("fail row 43 jacobian 0 %u expected 3210505724\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213565826u){printf("fail row 43 jacobian 1 %u expected 3213565826\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2904057632u){printf("fail row 43 jacobian 2 %u expected 2904057632\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(1066082929u),f(1062937224u),f(1028902215u)}};
Vector local_axis{{f(988185439u),f(968868166u),f(1065353188u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=759437056u){printf("fail row 44 prepared_axis 0 %u expected 759437056\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2912143360u){printf("fail row 44 prepared_axis 1 %u expected 2912143360\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353214u){printf("fail row 44 prepared_axis 2 %u expected 1065353214\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1062937084u){printf("fail row 44 jacobian 0 %u expected 1062937084\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213565723u){printf("fail row 44 jacobian 1 %u expected 3213565723\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2918484982u){printf("fail row 44 jacobian 2 %u expected 2918484982\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(1066082929u),f(1062937224u),f(1028902215u)}};
Vector local_axis{{f(988185439u),f(968868166u),f(1065353188u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=759437056u){printf("fail row 45 prepared_axis 0 %u expected 759437056\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=2912143360u){printf("fail row 45 prepared_axis 1 %u expected 2912143360\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353214u){printf("fail row 45 prepared_axis 2 %u expected 1065353214\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1062937084u){printf("fail row 45 jacobian 0 %u expected 1062937084\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213565723u){printf("fail row 45 jacobian 1 %u expected 3213565723\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2918484982u){printf("fail row 45 jacobian 2 %u expected 2918484982\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(3217022920u),f(3210505091u),f(1030291877u)}};
Vector local_axis{{f(3038241260u),f(1065353214u),f(3116351800u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3073376258u){printf("fail row 46 prepared_axis 0 %u expected 3073376258\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353214u){printf("fail row 46 prepared_axis 1 %u expected 1065353214\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=760749056u){printf("fail row 46 prepared_axis 2 %u expected 760749056\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3176983292u){printf("fail row 46 jacobian 0 %u expected 3176983292\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3038240844u){printf("fail row 46 jacobian 1 %u expected 3038240844\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3217023738u){printf("fail row 46 jacobian 2 %u expected 3217023738\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(3217022920u),f(3210505091u),f(1030291877u)}};
Vector local_axis{{f(1065353189u),f(3014017300u),f(3135669088u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353214u){printf("fail row 47 prepared_axis 0 %u expected 1065353214\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=925892608u){printf("fail row 47 prepared_axis 1 %u expected 925892608\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=759328000u){printf("fail row 47 prepared_axis 2 %u expected 759328000\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3038241334u){printf("fail row 47 jacobian 0 %u expected 3038241334\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1029499644u){printf("fail row 47 jacobian 1 %u expected 1029499644\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1063021774u){printf("fail row 47 jacobian 2 %u expected 1063021774\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(3217022777u),f(1062937629u),f(1030123037u)}};
Vector local_axis{{f(3038241260u),f(1065353214u),f(3116351800u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3073376258u){printf("fail row 48 prepared_axis 0 %u expected 3073376258\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353214u){printf("fail row 48 prepared_axis 1 %u expected 1065353214\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=760749056u){printf("fail row 48 prepared_axis 2 %u expected 760749056\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3176983036u){printf("fail row 48 jacobian 0 %u expected 3176983036\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3038240668u){printf("fail row 48 jacobian 1 %u expected 3038240668\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3217023587u){printf("fail row 48 jacobian 2 %u expected 3217023587\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(3217022777u),f(1062937629u),f(1030123037u)}};
Vector local_axis{{f(1065353189u),f(3014017300u),f(3135669088u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353214u){printf("fail row 49 prepared_axis 0 %u expected 1065353214\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=925892608u){printf("fail row 49 prepared_axis 1 %u expected 925892608\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=759328000u){printf("fail row 49 prepared_axis 2 %u expected 759328000\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3038240825u){printf("fail row 49 jacobian 0 %u expected 3038240825\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1029499388u){printf("fail row 49 jacobian 1 %u expected 1029499388\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3210420942u){printf("fail row 49 jacobian 2 %u expected 3210420942\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(1066082889u),f(3210505588u),f(1029070795u)}};
Vector local_axis{{f(3038241260u),f(1065353214u),f(3116351800u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3073376258u){printf("fail row 50 prepared_axis 0 %u expected 3073376258\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353214u){printf("fail row 50 prepared_axis 1 %u expected 1065353214\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=760749056u){printf("fail row 50 prepared_axis 2 %u expected 760749056\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3176983037u){printf("fail row 50 jacobian 0 %u expected 3176983037\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3038241228u){printf("fail row 50 jacobian 1 %u expected 3038241228\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1066082102u){printf("fail row 50 jacobian 2 %u expected 1066082102\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(1066082889u),f(3210505588u),f(1029070795u)}};
Vector local_axis{{f(1065353189u),f(3014017300u),f(3135669088u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353214u){printf("fail row 51 prepared_axis 0 %u expected 1065353214\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=925892608u){printf("fail row 51 prepared_axis 1 %u expected 925892608\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=759328000u){printf("fail row 51 prepared_axis 2 %u expected 759328000\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3038241158u){printf("fail row 51 jacobian 0 %u expected 3038241158\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1029499389u){printf("fail row 51 jacobian 1 %u expected 1029499389\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1063022267u){printf("fail row 51 jacobian 2 %u expected 1063022267\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(1066082929u),f(1062937224u),f(1028902215u)}};
Vector local_axis{{f(3038241260u),f(1065353214u),f(3116351800u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3073376258u){printf("fail row 52 prepared_axis 0 %u expected 3073376258\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353214u){printf("fail row 52 prepared_axis 1 %u expected 1065353214\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=760749056u){printf("fail row 52 prepared_axis 2 %u expected 760749056\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3176983035u){printf("fail row 52 jacobian 0 %u expected 3176983035\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3038241227u){printf("fail row 52 jacobian 1 %u expected 3038241227\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1066082150u){printf("fail row 52 jacobian 2 %u expected 1066082150\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(960479873u),f(3127280469u),f(917235502u),f(1065353209u)}};
Vector local_point{{f(1066082929u),f(1062937224u),f(1028902215u)}};
Vector local_axis{{f(1065353189u),f(3014017300u),f(3135669088u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353214u){printf("fail row 53 prepared_axis 0 %u expected 1065353214\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=925892608u){printf("fail row 53 prepared_axis 1 %u expected 925892608\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=759328000u){printf("fail row 53 prepared_axis 2 %u expected 759328000\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3038240824u){printf("fail row 53 jacobian 0 %u expected 3038240824\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1029499387u){printf("fail row 53 jacobian 1 %u expected 1029499387\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3210420541u){printf("fail row 53 jacobian 2 %u expected 3210420541\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213675776u),f(3212534880u),f(1020352512u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 54 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 54 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 54 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212534880u){printf("fail row 54 jacobian 0 %u expected 3212534880\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1066192128u){printf("fail row 54 jacobian 1 %u expected 1066192128\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 54 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213675776u),f(3212534880u),f(1020352512u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 55 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 55 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 55 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212534880u){printf("fail row 55 jacobian 0 %u expected 3212534880\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1066192128u){printf("fail row 55 jacobian 1 %u expected 1066192128\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 55 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213675776u),f(1065051130u),f(1020352512u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 56 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 56 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 56 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065051130u){printf("fail row 56 jacobian 0 %u expected 1065051130\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1066192128u){printf("fail row 56 jacobian 1 %u expected 1066192128\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2147483648u){printf("fail row 56 jacobian 2 %u expected 2147483648\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213675776u),f(1065051130u),f(1020352512u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 57 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 57 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 57 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065051130u){printf("fail row 57 jacobian 0 %u expected 1065051130\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1066192128u){printf("fail row 57 jacobian 1 %u expected 1066192128\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=2147483648u){printf("fail row 57 jacobian 2 %u expected 2147483648\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066192125u),f(3212534784u),f(1020353024u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 58 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 58 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 58 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212534784u){printf("fail row 58 jacobian 0 %u expected 3212534784\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213675773u){printf("fail row 58 jacobian 1 %u expected 3213675773\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 58 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066192125u),f(3212534784u),f(1020353024u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 59 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 59 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 59 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212534784u){printf("fail row 59 jacobian 0 %u expected 3212534784\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213675773u){printf("fail row 59 jacobian 1 %u expected 3213675773\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 59 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066192080u),f(1065051136u),f(1020353024u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 60 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 60 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 60 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065051136u){printf("fail row 60 jacobian 0 %u expected 1065051136\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213675728u){printf("fail row 60 jacobian 1 %u expected 3213675728\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 60 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066192080u),f(1065051136u),f(1020353024u)}};
Vector local_axis{{f(0u),f(0u),f(1065353216u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 61 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 61 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065353216u){printf("fail row 61 prepared_axis 2 %u expected 1065353216\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065051136u){printf("fail row 61 jacobian 0 %u expected 1065051136\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=3213675728u){printf("fail row 61 jacobian 1 %u expected 3213675728\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 61 jacobian 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213675776u),f(3212534880u),f(1020352512u)}};
Vector local_axis{{f(0u),f(1065353216u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 62 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353216u){printf("fail row 62 prepared_axis 1 %u expected 1065353216\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 62 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3167836160u){printf("fail row 62 jacobian 0 %u expected 3167836160\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 62 jacobian 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3213675776u){printf("fail row 62 jacobian 2 %u expected 3213675776\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213675776u),f(3212534880u),f(1020352512u)}};
Vector local_axis{{f(1065353216u),f(0u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353216u){printf("fail row 63 prepared_axis 0 %u expected 1065353216\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 63 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 63 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=2147483648u){printf("fail row 63 jacobian 0 %u expected 2147483648\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1020352512u){printf("fail row 63 jacobian 1 %u expected 1020352512\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065051232u){printf("fail row 63 jacobian 2 %u expected 1065051232\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213675776u),f(1065051130u),f(1020352512u)}};
Vector local_axis{{f(0u),f(1065353216u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 64 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353216u){printf("fail row 64 prepared_axis 1 %u expected 1065353216\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 64 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3167836160u){printf("fail row 64 jacobian 0 %u expected 3167836160\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 64 jacobian 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3213675776u){printf("fail row 64 jacobian 2 %u expected 3213675776\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213675776u),f(1065051130u),f(1020352512u)}};
Vector local_axis{{f(1065353216u),f(0u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353216u){printf("fail row 65 prepared_axis 0 %u expected 1065353216\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 65 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 65 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=0u){printf("fail row 65 jacobian 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1020352512u){printf("fail row 65 jacobian 1 %u expected 1020352512\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3212534778u){printf("fail row 65 jacobian 2 %u expected 3212534778\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066192125u),f(3212534784u),f(1020353024u)}};
Vector local_axis{{f(0u),f(1065353216u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 66 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353216u){printf("fail row 66 prepared_axis 1 %u expected 1065353216\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 66 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3167836672u){printf("fail row 66 jacobian 0 %u expected 3167836672\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 66 jacobian 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1066192125u){printf("fail row 66 jacobian 2 %u expected 1066192125\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066192125u),f(3212534784u),f(1020353024u)}};
Vector local_axis{{f(1065353216u),f(0u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353216u){printf("fail row 67 prepared_axis 0 %u expected 1065353216\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 67 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 67 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=2147483648u){printf("fail row 67 jacobian 0 %u expected 2147483648\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1020353024u){printf("fail row 67 jacobian 1 %u expected 1020353024\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1065051136u){printf("fail row 67 jacobian 2 %u expected 1065051136\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066192080u),f(1065051136u),f(1020353024u)}};
Vector local_axis{{f(0u),f(1065353216u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=0u){printf("fail row 68 prepared_axis 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1065353216u){printf("fail row 68 prepared_axis 1 %u expected 1065353216\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 68 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3167836672u){printf("fail row 68 jacobian 0 %u expected 3167836672\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 68 jacobian 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=1066192080u){printf("fail row 68 jacobian 2 %u expected 1066192080\n",bits(got[2]));++failed;}else ++ok;
}
}
{
Quaternion quaternion{{f(0u),f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1066192080u),f(1065051136u),f(1020353024u)}};
Vector local_axis{{f(1065353216u),f(0u),f(0u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353216u){printf("fail row 69 prepared_axis 0 %u expected 1065353216\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=0u){printf("fail row 69 prepared_axis 1 %u expected 0\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=0u){printf("fail row 69 prepared_axis 2 %u expected 0\n",bits(got[2]));++failed;}else ++ok;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=0u){printf("fail row 69 jacobian 0 %u expected 0\n",bits(got[0]));++failed;}else ++ok;
if(bits(got[1])!=1020353024u){printf("fail row 69 jacobian 1 %u expected 1020353024\n",bits(got[1]));++failed;}else ++ok;
if(bits(got[2])!=3212534784u){printf("fail row 69 jacobian 2 %u expected 3212534784\n",bits(got[2]));++failed;}else ++ok;
}
}
printf("ok %u failed %u\n",ok,failed);return failed?1:0;}
