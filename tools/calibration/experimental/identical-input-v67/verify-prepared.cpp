#include "NativeWheelRowV56.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
float f(uint32_t w){float x;memcpy(&x,&w,4);return x;}
uint32_t bits(float x){uint32_t w;memcpy(&w,&x,4);return w;}
int main(){unsigned ok=0,failed=0;
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(3208688877u),f(1065002711u),f(2979011024u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3136495980u){printf("fail I row 0 c 0 %u expected 3136495980\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=980479381u){printf("fail I row 0 c 1 %u expected 980479381\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3114511243u){printf("fail I row 0 c 2 %u expected 3114511243\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(2982794752u),f(2944401408u),f(1065353243u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134544052u){printf("fail mass row 0 %u expected 1134544052\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(1061114292u),f(1065051128u),f(832483859u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988950133u){printf("fail I row 1 c 0 %u expected 988950133\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=980606696u){printf("fail I row 1 c 1 %u expected 980606696\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=966977951u){printf("fail I row 1 c 2 %u expected 966977951\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(2982794752u),f(2944401408u),f(1065353243u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134562608u){printf("fail mass row 1 %u expected 1134562608\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(3208688877u),f(1065002711u),f(2979011024u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3136495980u){printf("fail I row 2 c 0 %u expected 3136495980\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=980479381u){printf("fail I row 2 c 1 %u expected 980479381\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3114511243u){printf("fail I row 2 c 2 %u expected 3114511243\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(1061114292u),f(1065051128u),f(832483859u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988950133u){printf("fail I row 3 c 0 %u expected 988950133\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=980606696u){printf("fail I row 3 c 1 %u expected 980606696\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=966977951u){printf("fail I row 3 c 2 %u expected 966977951\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(3208283009u),f(3213687870u),f(3190910968u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3136657068u){printf("fail I row 4 c 0 %u expected 3136657068\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3129486317u){printf("fail I row 4 c 1 %u expected 3129486317\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3119069322u){printf("fail I row 4 c 2 %u expected 3119069322\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(1060837895u),f(3213663813u),f(1043432867u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989163267u){printf("fail I row 5 c 0 %u expected 989163267\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3129422674u){printf("fail I row 5 c 1 %u expected 3129422674\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=971589705u){printf("fail I row 5 c 2 %u expected 971589705\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(1059175877u),f(981424988u),f(3212505811u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=982977573u){printf("fail I row 6 c 0 %u expected 982977573\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=908537601u){printf("fail I row 6 c 1 %u expected 908537601\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3121452540u){printf("fail I row 6 c 2 %u expected 3121452540\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3133806131u),f(1065353233u),f(2955935744u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1139745346u){printf("fail mass row 6 %u expected 1139745346\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(981424709u),f(3206659524u),f(1061179885u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=967034537u){printf("fail I row 7 c 0 %u expected 967034537\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3122382998u){printf("fail I row 7 c 1 %u expected 3122382998\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=975257721u){printf("fail I row 7 c 2 %u expected 975257721\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353232u),f(986322493u),f(851988608u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142265720u){printf("fail mass row 7 %u expected 1142265720\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(1059175877u),f(981424988u),f(3212515425u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=982975529u){printf("fail I row 8 c 0 %u expected 982975529\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=908536565u){printf("fail I row 8 c 1 %u expected 908536565\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3121460598u){printf("fail I row 8 c 2 %u expected 3121460598\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3133806131u),f(1065353233u),f(2955935744u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1139740705u){printf("fail mass row 8 %u expected 1139740705\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(981425310u),f(3206659524u),f(3208623325u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3114454648u){printf("fail I row 9 c 0 %u expected 3114454648\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3122393589u){printf("fail I row 9 c 1 %u expected 3122393589\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3122702687u){printf("fail I row 9 c 2 %u expected 3122702687\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353232u),f(986322493u),f(851988608u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142285574u){printf("fail mass row 9 %u expected 1142285574\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(1051273243u),f(980764932u),f(1067107045u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=984396047u){printf("fail I row 10 c 0 %u expected 984396047\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=908186622u){printf("fail I row 10 c 1 %u expected 908186622\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=982758963u){printf("fail I row 10 c 2 %u expected 982758963\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3137115400u),f(1065353225u),f(3112282689u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1136527114u){printf("fail mass row 10 %u expected 1136527114\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(3190851525u),f(3206142700u),f(1060837442u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3103911706u){printf("fail I row 11 c 0 %u expected 3103911706\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3121894415u){printf("fail I row 11 c 1 %u expected 3121894415\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=973734115u){printf("fail I row 11 c 2 %u expected 973734115\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1064899124u),f(989674638u),f(1047306836u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142834950u){printf("fail mass row 11 %u expected 1142834950\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(1051273009u),f(980764826u),f(1067107292u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=984396004u){printf("fail I row 12 c 0 %u expected 984396004\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=908186554u){printf("fail I row 12 c 1 %u expected 908186554\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=982759146u){printf("fail I row 12 c 2 %u expected 982759146\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3137115400u),f(1065353225u),f(3112282689u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1136526887u){printf("fail mass row 12 %u expected 1136526887\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(992068198u),f(911345602u),f(970568937u),f(911345602u),f(980857626u),f(886874084u),f(970568937u),f(886874084u),f(978753563u)};motorrock_native_wheel_row_v56::Vector r{f(1043492315u),f(3206137548u),f(3208283510u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=956548044u){printf("fail I row 13 c 0 %u expected 956548044\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3121881117u){printf("fail I row 13 c 1 %u expected 3121881117\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3121181518u){printf("fail I row 13 c 2 %u expected 3121181518\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1064899124u),f(989674638u),f(1047306836u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142853511u){printf("fail mass row 13 %u expected 1142853511\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(991568557u),f(928224567u),f(974862935u),f(928224567u),f(980859186u),f(911911034u),f(974862935u),f(911911034u),f(980750565u)};motorrock_native_wheel_row_v56::Vector r{f(3208823268u),f(1063776346u),f(2997384948u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3135833764u){printf("fail I row 14 c 0 %u expected 3135833764\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=979174627u){printf("fail I row 14 c 1 %u expected 979174627\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3119162200u){printf("fail I row 14 c 2 %u expected 3119162200\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3001871360u),f(2966421504u),f(1065353241u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1135182211u){printf("fail mass row 14 %u expected 1135182211\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(991568557u),f(928224567u),f(974862935u),f(928224567u),f(980859186u),f(911911034u),f(974862935u),f(911911034u),f(980750565u)};motorrock_native_wheel_row_v56::Vector r{f(1060978610u),f(1063989628u),f(851121806u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988113069u){printf("fail I row 15 c 0 %u expected 988113069\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=979705177u){printf("fail I row 15 c 1 %u expected 979705177\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=971445034u){printf("fail I row 15 c 2 %u expected 971445034\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3001871360u),f(2966421504u),f(1065353241u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1135256386u){printf("fail mass row 15 %u expected 1135256386\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(991568557u),f(928224567u),f(974862935u),f(928224567u),f(980859186u),f(911911034u),f(974862935u),f(911911034u),f(980750565u)};motorrock_native_wheel_row_v56::Vector r{f(3208823268u),f(1063776346u),f(2997384948u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3135833764u){printf("fail I row 16 c 0 %u expected 3135833764\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=979174627u){printf("fail I row 16 c 1 %u expected 979174627\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3119162200u){printf("fail I row 16 c 2 %u expected 3119162200\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(991568557u),f(928224567u),f(974862935u),f(928224567u),f(980859186u),f(911911034u),f(974862935u),f(911911034u),f(980750565u)};motorrock_native_wheel_row_v56::Vector r{f(1060978610u),f(1063989628u),f(851121806u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988113069u){printf("fail I row 17 c 0 %u expected 988113069\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=979705177u){printf("fail I row 17 c 1 %u expected 979705177\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=971445034u){printf("fail I row 17 c 2 %u expected 971445034\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(991568557u),f(928224567u),f(974862935u),f(928224567u),f(980859186u),f(911911034u),f(974862935u),f(911911034u),f(980750565u)};motorrock_native_wheel_row_v56::Vector r{f(1060494973u),f(998379702u),f(3211331071u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=982801603u){printf("fail I row 18 c 0 %u expected 982801603\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=925055832u){printf("fail I row 18 c 1 %u expected 925055832\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3118568496u){printf("fail I row 18 c 2 %u expected 3118568496\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3149337622u),f(1065353006u),f(2973237248u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1140222026u){printf("fail mass row 18 %u expected 1140222026\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(991568557u),f(928224567u),f(974862935u),f(928224567u),f(980859186u),f(911911034u),f(974862935u),f(911911034u),f(980750565u)};motorrock_native_wheel_row_v56::Vector r{f(998379686u),f(3207978619u),f(1061254495u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=971689669u){printf("fail I row 19 c 0 %u expected 971689669\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3123617474u){printf("fail I row 19 c 1 %u expected 3123617474\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=976827516u){printf("fail I row 19 c 2 %u expected 976827516\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353004u),f(1001853984u),f(848465920u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141355563u){printf("fail mass row 19 %u expected 1141355563\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(991568557u),f(928224567u),f(974862935u),f(928224567u),f(980859186u),f(911911034u),f(974862935u),f(911911034u),f(980750565u)};motorrock_native_wheel_row_v56::Vector r{f(1060494974u),f(998379703u),f(3211403742u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=982779572u){printf("fail I row 20 c 0 %u expected 982779572\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=925040316u){printf("fail I row 20 c 1 %u expected 925040316\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3118707629u){printf("fail I row 20 c 2 %u expected 3118707629\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3149337622u),f(1065353006u),f(2973237248u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1140192275u){printf("fail mass row 20 %u expected 1140192275\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(991568557u),f(928224567u),f(974862935u),f(928224567u),f(980859186u),f(911911034u),f(974862935u),f(911911034u),f(980750565u)};motorrock_native_wheel_row_v56::Vector r{f(998379742u),f(3207978620u),f(3208548181u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3118917106u){printf("fail I row 21 c 0 %u expected 3118917106\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3123701443u){printf("fail I row 21 c 1 %u expected 3123701443\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3124128091u){printf("fail I row 21 c 2 %u expected 3124128091\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353004u),f(1001853984u),f(848465920u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141433407u){printf("fail mass row 21 %u expected 1141433407\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(3210504760u),f(1069539123u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3131341247u){printf("fail I row 22 c 0 %u expected 3131341247\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=974980275u){printf("fail I row 22 c 1 %u expected 974980275\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 22 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(0u),f(1065353216u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1137295511u){printf("fail mass row 22 %u expected 1137295511\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(3210504760u),f(1069539123u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3131341247u){printf("fail I row 23 c 0 %u expected 3131341247\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=974980275u){printf("fail I row 23 c 1 %u expected 974980275\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 23 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(1062937712u),f(1069538867u),f(2147483648u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983795365u){printf("fail I row 24 c 0 %u expected 983795365\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=974980066u){printf("fail I row 24 c 1 %u expected 974980066\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 24 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(0u),f(1065353216u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1137362411u){printf("fail mass row 24 %u expected 1137362411\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(1062937712u),f(1069538867u),f(2147483648u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983795365u){printf("fail I row 25 c 0 %u expected 983795365\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=974980066u){printf("fail I row 25 c 1 %u expected 974980066\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 25 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(3210504448u),f(3213566830u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3131341014u){printf("fail I row 26 c 0 %u expected 3131341014\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3118709331u){printf("fail I row 26 c 1 %u expected 3118709331\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 26 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(0u),f(1065353216u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1140038187u){printf("fail mass row 26 %u expected 1140038187\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(3210504448u),f(3213566830u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3131341014u){printf("fail I row 27 c 0 %u expected 3131341014\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3118709331u){printf("fail I row 27 c 1 %u expected 3118709331\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 27 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(1062936832u),f(3213566491u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983794709u){printf("fail I row 28 c 0 %u expected 983794709\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3118708777u){printf("fail I row 28 c 1 %u expected 3118708777\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 28 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(0u),f(1065353216u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1140136472u){printf("fail mass row 28 %u expected 1140136472\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(1062936832u),f(3213566491u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983794709u){printf("fail I row 29 c 0 %u expected 983794709\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3118708777u){printf("fail I row 29 c 1 %u expected 3118708777\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 29 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(3181034496u),f(0u),f(3217022771u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3102141864u){printf("fail I row 30 c 0 %u expected 3102141864\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=0u){printf("fail I row 30 c 1 %u expected 0\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3121290959u){printf("fail I row 30 c 2 %u expected 3121290959\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1145028026u){printf("fail mass row 30 %u expected 1145028026\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(2147483648u),f(1033550848u),f(1063021112u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=0u){printf("fail I row 31 c 0 %u expected 0\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=939344119u){printf("fail I row 31 c 1 %u expected 939344119\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=966775015u){printf("fail I row 31 c 2 %u expected 966775015\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(0u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1151562910u){printf("fail mass row 31 %u expected 1151562910\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(3181034496u),f(0u),f(3217022515u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3102141864u){printf("fail I row 32 c 0 %u expected 3102141864\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=0u){printf("fail I row 32 c 1 %u expected 0\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3121290773u){printf("fail I row 32 c 2 %u expected 3121290773\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1145028338u){printf("fail mass row 32 %u expected 1145028338\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(0u),f(1033550848u),f(3210421360u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=0u){printf("fail I row 33 c 0 %u expected 0\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=939344119u){printf("fail I row 33 c 1 %u expected 939344119\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3114198192u){printf("fail I row 33 c 2 %u expected 3114198192\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(0u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1151605408u){printf("fail mass row 33 %u expected 1151605408\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(3181034624u),f(0u),f(1066083182u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3102142055u){printf("fail I row 34 c 0 %u expected 3102142055\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=0u){printf("fail I row 34 c 1 %u expected 0\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=969524502u){printf("fail I row 34 c 2 %u expected 969524502\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1149691096u){printf("fail mass row 34 %u expected 1149691096\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(2147483648u),f(1033550976u),f(1063020800u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=0u){printf("fail I row 35 c 0 %u expected 0\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=939344329u){printf("fail I row 35 c 1 %u expected 939344329\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=966774788u){printf("fail I row 35 c 2 %u expected 966774788\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(0u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1151563069u){printf("fail mass row 35 %u expected 1151563069\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(3181034624u),f(0u),f(1066082843u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3102142055u){printf("fail I row 36 c 0 %u expected 3102142055\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=0u){printf("fail I row 36 c 1 %u expected 0\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=969524011u){printf("fail I row 36 c 2 %u expected 969524011\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1149691392u){printf("fail mass row 36 %u expected 1149691392\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597836u),f(0u),f(0u),f(0u),f(970030948u),f(0u),f(0u),f(0u),f(968465951u)};motorrock_native_wheel_row_v56::Vector r{f(0u),f(1033550976u),f(3210420480u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=0u){printf("fail I row 37 c 0 %u expected 0\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=939344329u){printf("fail I row 37 c 1 %u expected 939344329\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3114197554u){printf("fail I row 37 c 2 %u expected 3114197554\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(0u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1151605856u){printf("fail mass row 37 %u expected 1151605856\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3210505686u),f(1069540014u),f(773389069u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3131341778u){printf("fail I row 38 c 0 %u expected 3131341778\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=974980849u){printf("fail I row 38 c 1 %u expected 974980849\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3051153471u){printf("fail I row 38 c 2 %u expected 3051153471\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(759437056u),f(2912143360u),f(1065353214u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1137294252u){printf("fail mass row 38 %u expected 1137294252\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3210505686u),f(1069540014u),f(773389069u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3131341778u){printf("fail I row 39 c 0 %u expected 3131341778\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=974980849u){printf("fail I row 39 c 1 %u expected 974980849\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3051153471u){printf("fail I row 39 c 2 %u expected 3051153471\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(1062937030u),f(1069540014u),f(763993440u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983794967u){printf("fail I row 40 c 0 %u expected 983794967\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=974981158u){printf("fail I row 40 c 1 %u expected 974981158\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=904023175u){printf("fail I row 40 c 2 %u expected 904023175\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(759437056u),f(2912143360u),f(1065353214u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1137361944u){printf("fail mass row 40 %u expected 1137361944\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(1062937030u),f(1069540014u),f(763993440u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983794967u){printf("fail I row 41 c 0 %u expected 983794967\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=974981158u){printf("fail I row 41 c 1 %u expected 974981158\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=904023175u){printf("fail I row 41 c 2 %u expected 904023175\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3210505724u),f(3213565826u),f(2904057632u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3131342040u){printf("fail I row 42 c 0 %u expected 3131342040\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3118707998u){printf("fail I row 42 c 1 %u expected 3118707998\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3051532878u){printf("fail I row 42 c 2 %u expected 3051532878\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(759437056u),f(2912143360u),f(1065353214u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1140037376u){printf("fail mass row 42 %u expected 1140037376\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3210505724u),f(3213565826u),f(2904057632u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3131342040u){printf("fail I row 43 c 0 %u expected 3131342040\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3118707998u){printf("fail I row 43 c 1 %u expected 3118707998\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3051532878u){printf("fail I row 43 c 2 %u expected 3051532878\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(1062937084u),f(3213565723u),f(2918484982u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983794774u){printf("fail I row 44 c 0 %u expected 983794774\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3118707212u){printf("fail I row 44 c 1 %u expected 3118707212\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=903643864u){printf("fail I row 44 c 2 %u expected 903643864\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(759437056u),f(2912143360u),f(1065353214u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1140136977u){printf("fail mass row 44 %u expected 1140136977\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(1062937084u),f(3213565723u),f(2918484982u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983794774u){printf("fail I row 45 c 0 %u expected 983794774\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3118707212u){printf("fail I row 45 c 1 %u expected 3118707212\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=903643864u){printf("fail I row 45 c 2 %u expected 903643864\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3176983292u),f(3038240844u),f(3217023738u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3098211207u){printf("fail I row 46 c 0 %u expected 3098211207\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3000864532u){printf("fail I row 46 c 1 %u expected 3000864532\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3121293546u){printf("fail I row 46 c 2 %u expected 3121293546\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3073376258u),f(1065353214u),f(760749056u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1145063169u){printf("fail mass row 46 %u expected 1145063169\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3038241334u),f(1029499644u),f(1063021774u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=903887187u){printf("fail I row 47 c 0 %u expected 903887187\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=934602910u){printf("fail I row 47 c 1 %u expected 934602910\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=966775627u){printf("fail I row 47 c 2 %u expected 966775627\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353214u),f(925892608u),f(759328000u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1151578209u){printf("fail mass row 47 %u expected 1151578209\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3176983036u),f(3038240668u),f(3217023587u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3098211011u){printf("fail I row 48 c 0 %u expected 3098211011\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3000864355u){printf("fail I row 48 c 1 %u expected 3000864355\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3121293437u){printf("fail I row 48 c 2 %u expected 3121293437\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3073376258u),f(1065353214u),f(760749056u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1145063355u){printf("fail mass row 48 %u expected 1145063355\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3038240825u),f(1029499388u),f(3210420942u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3051289458u){printf("fail I row 49 c 0 %u expected 3051289458\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=934586957u){printf("fail I row 49 c 1 %u expected 934586957\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3114197959u){printf("fail I row 49 c 2 %u expected 3114197959\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353214u),f(925892608u),f(759328000u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1151621426u){printf("fail mass row 49 %u expected 1151621426\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3176983037u),f(3038241228u),f(1066082102u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3097521321u){printf("fail I row 50 c 0 %u expected 3097521321\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=848620276u){printf("fail I row 50 c 1 %u expected 848620276\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=969519469u){printf("fail I row 50 c 2 %u expected 969519469\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3073376258u),f(1065353214u),f(760749056u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1149733301u){printf("fail mass row 50 %u expected 1149733301\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3038241158u),f(1029499389u),f(1063022267u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=903887688u){printf("fail I row 51 c 0 %u expected 903887688\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=934602701u){printf("fail I row 51 c 1 %u expected 934602701\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=966775985u){printf("fail I row 51 c 2 %u expected 966775985\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353214u),f(925892608u),f(759328000u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1151577958u){printf("fail mass row 51 %u expected 1151577958\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3176983035u),f(3038241227u),f(1066082150u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3097521317u){printf("fail I row 52 c 0 %u expected 3097521317\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=848620330u){printf("fail I row 52 c 1 %u expected 848620330\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=969519538u){printf("fail I row 52 c 2 %u expected 969519538\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(3073376258u),f(1065353214u),f(760749056u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1149733258u){printf("fail mass row 52 %u expected 1149733258\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(985597807u),f(842259757u),f(906115565u),f(842259757u),f(970030948u),f(848249632u),f(906115565u),f(848249632u),f(968466069u)};motorrock_native_wheel_row_v56::Vector r{f(3038240824u),f(1029499387u),f(3210420541u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3051289050u){printf("fail I row 53 c 0 %u expected 3051289050\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=934586956u){printf("fail I row 53 c 1 %u expected 934586956\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3114197668u){printf("fail I row 53 c 2 %u expected 3114197668\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353214u),f(925892608u),f(759328000u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1151621632u){printf("fail mass row 53 %u expected 1151621632\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(3212534880u),f(1066192128u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3126426454u){printf("fail I row 54 c 0 %u expected 3126426454\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=973591955u){printf("fail I row 54 c 1 %u expected 973591955\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 54 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(0u),f(1065353216u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141155330u){printf("fail mass row 54 %u expected 1141155330\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(3212534880u),f(1066192128u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3126426454u){printf("fail I row 55 c 0 %u expected 3126426454\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=973591955u){printf("fail I row 55 c 1 %u expected 973591955\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 55 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(1065051130u),f(1066192128u),f(2147483648u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=978942717u){printf("fail I row 56 c 0 %u expected 978942717\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=973591955u){printf("fail I row 56 c 1 %u expected 973591955\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 56 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(0u),f(1065353216u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141155377u){printf("fail mass row 56 %u expected 1141155377\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(1065051130u),f(1066192128u),f(2147483648u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=978942717u){printf("fail I row 57 c 0 %u expected 978942717\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=973591955u){printf("fail I row 57 c 1 %u expected 973591955\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 57 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(3212534784u),f(3213675773u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3126426371u){printf("fail I row 58 c 0 %u expected 3126426371\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3121075600u){printf("fail I row 58 c 1 %u expected 3121075600\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 58 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(0u),f(1065353216u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141155375u){printf("fail mass row 58 %u expected 1141155375\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(3212534784u),f(3213675773u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3126426371u){printf("fail I row 59 c 0 %u expected 3126426371\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3121075600u){printf("fail I row 59 c 1 %u expected 3121075600\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 59 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(1065051136u),f(3213675728u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=978942723u){printf("fail I row 60 c 0 %u expected 978942723\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3121075557u){printf("fail I row 60 c 1 %u expected 3121075557\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 60 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(0u),f(1065353216u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141155401u){printf("fail mass row 60 %u expected 1141155401\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(1065051136u),f(3213675728u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=978942723u){printf("fail I row 61 c 0 %u expected 978942723\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=3121075557u){printf("fail I row 61 c 1 %u expected 3121075557\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=0u){printf("fail I row 61 c 2 %u expected 0\n",bits(v[2]));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(3167836160u),f(0u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3082099448u){printf("fail I row 62 c 0 %u expected 3082099448\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=0u){printf("fail I row 62 c 1 %u expected 0\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3116155670u){printf("fail I row 62 c 2 %u expected 3116155670\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1149987503u){printf("fail mass row 62 %u expected 1149987503\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(2147483648u),f(1020352512u),f(1065051232u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=0u){printf("fail I row 63 c 0 %u expected 0\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=927593891u){printf("fail I row 63 c 1 %u expected 927593891\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=967344925u){printf("fail I row 63 c 2 %u expected 967344925\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(0u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1150890718u){printf("fail mass row 63 %u expected 1150890718\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(3167836160u),f(0u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3082099448u){printf("fail I row 64 c 0 %u expected 3082099448\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=0u){printf("fail I row 64 c 1 %u expected 0\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3116155670u){printf("fail I row 64 c 2 %u expected 3116155670\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1149987503u){printf("fail mass row 64 %u expected 1149987503\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(0u),f(1020352512u),f(3212534778u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=0u){printf("fail I row 65 c 0 %u expected 0\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=927593891u){printf("fail I row 65 c 1 %u expected 927593891\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3114828505u){printf("fail I row 65 c 2 %u expected 3114828505\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(0u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1150890766u){printf("fail mass row 65 %u expected 1150890766\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(3167836672u),f(0u),f(1066192125u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3082099891u){printf("fail I row 66 c 0 %u expected 3082099891\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=0u){printf("fail I row 66 c 1 %u expected 0\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=968672018u){printf("fail I row 66 c 2 %u expected 968672018\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1149987506u){printf("fail mass row 66 %u expected 1149987506\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(2147483648u),f(1020353024u),f(1065051136u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=0u){printf("fail I row 67 c 0 %u expected 0\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=927594385u){printf("fail I row 67 c 1 %u expected 927594385\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=967344861u){printf("fail I row 67 c 2 %u expected 967344861\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(0u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1150890763u){printf("fail mass row 67 %u expected 1150890763\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(3167836672u),f(0u),f(1066192080u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3082099891u){printf("fail I row 68 c 0 %u expected 3082099891\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=0u){printf("fail I row 68 c 1 %u expected 0\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=968671958u){printf("fail I row 68 c 2 %u expected 968671958\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(0u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1149987545u){printf("fail mass row 68 %u expected 1149987545\n",bits(mass));++failed;}else ++ok;
}
{motorrock_native_wheel_row_v56::Matrix m{f(979204055u),f(0u),f(0u),f(0u),f(972486741u),f(0u),f(0u),f(0u),f(967547350u)};motorrock_native_wheel_row_v56::Vector r{f(0u),f(1020353024u),f(3212534784u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=0u){printf("fail I row 69 c 0 %u expected 0\n",bits(v[0]));++failed;}else ++ok;
if(bits(v[1])!=927594385u){printf("fail I row 69 c 1 %u expected 927594385\n",bits(v[1]));++failed;}else ++ok;
if(bits(v[2])!=3114828509u){printf("fail I row 69 c 2 %u expected 3114828509\n",bits(v[2]));++failed;}else ++ok;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(0u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(973279855u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1150890763u){printf("fail mass row 69 %u expected 1150890763\n",bits(mass));++failed;}else ++ok;
}
printf("ok %u failed %u\n",ok,failed);return failed?1:0;}
