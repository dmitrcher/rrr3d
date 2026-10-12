#include "NativeWheelRowV56.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
float f(uint32_t w){float x;std::memcpy(&x,&w,4);return x;}
uint32_t bits(float x){uint32_t w;std::memcpy(&w,&x,4);return w;}
int main(){unsigned checked=0;
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642611u),f(1066192128u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137099274u){std::fprintf(stderr,"row 0 component 0: %08x expected %08x\n",bits(v[0]),3137099274u);return 1;}++checked;
if(bits(v[1])!=981970762u){std::fprintf(stderr,"row 0 component 1: %08x expected %08x\n",bits(v[1]),981970762u);return 1;}++checked;
if(bits(v[2])!=773807092u){std::fprintf(stderr,"row 0 component 2: %08x expected %08x\n",bits(v[2]),773807092u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061158861u),f(1066192128u),f(2147483648u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989615491u){std::fprintf(stderr,"row 1 component 0: %08x expected %08x\n",bits(v[0]),989615491u);return 1;}++checked;
if(bits(v[1])!=981970762u){std::fprintf(stderr,"row 1 component 1: %08x expected %08x\n",bits(v[1]),981970762u);return 1;}++checked;
if(bits(v[2])!=755008481u){std::fprintf(stderr,"row 1 component 2: %08x expected %08x\n",bits(v[2]),755008481u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642560u),f(3213675750u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137099206u){std::fprintf(stderr,"row 2 component 0: %08x expected %08x\n",bits(v[0]),3137099206u);return 1;}++checked;
if(bits(v[1])!=3129454385u){std::fprintf(stderr,"row 2 component 1: %08x expected %08x\n",bits(v[1]),3129454385u);return 1;}++checked;
if(bits(v[2])!=2902491990u){std::fprintf(stderr,"row 2 component 2: %08x expected %08x\n",bits(v[2]),2902491990u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061158912u),f(3213675750u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989615558u){std::fprintf(stderr,"row 3 component 0: %08x expected %08x\n",bits(v[0]),989615558u);return 1;}++checked;
if(bits(v[1])!=3129454385u){std::fprintf(stderr,"row 3 component 1: %08x expected %08x\n",bits(v[1]),3129454385u);return 1;}++checked;
if(bits(v[2])!=2921290706u){std::fprintf(stderr,"row 3 component 2: %08x expected %08x\n",bits(v[2]),2921290706u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061035664u),f(3007192721u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989453580u){std::fprintf(stderr,"row 4 component 0: %08x expected %08x\n",bits(v[0]),989453580u);return 1;}++checked;
if(bits(v[1])!=2928376384u){std::fprintf(stderr,"row 4 component 1: %08x expected %08x\n",bits(v[1]),2928376384u);return 1;}++checked;
if(bits(v[2])!=3125779802u){std::fprintf(stderr,"row 4 component 2: %08x expected %08x\n",bits(v[2]),3125779802u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134575194u){std::fprintf(stderr,"mass row 4: %08x expected %08x\n",bits(mass),1134575194u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3007192721u),f(3208519312u),f(1061158964u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2936853995u){std::fprintf(stderr,"row 5 component 0: %08x expected %08x\n",bits(v[0]),2936853995u);return 1;}++checked;
if(bits(v[1])!=3124180505u){std::fprintf(stderr,"row 5 component 1: %08x expected %08x\n",bits(v[1]),3124180505u);return 1;}++checked;
if(bits(v[2])!=973966885u){std::fprintf(stderr,"row 5 component 2: %08x expected %08x\n",bits(v[2]),973966885u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141785127u){std::fprintf(stderr,"mass row 5: %08x expected %08x\n",bits(mass),1141785127u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061035664u),f(3007192721u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989453580u){std::fprintf(stderr,"row 6 component 0: %08x expected %08x\n",bits(v[0]),989453580u);return 1;}++checked;
if(bits(v[1])!=2928376384u){std::fprintf(stderr,"row 6 component 1: %08x expected %08x\n",bits(v[1]),2928376384u);return 1;}++checked;
if(bits(v[2])!=3125779802u){std::fprintf(stderr,"row 6 component 2: %08x expected %08x\n",bits(v[2]),3125779802u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134575194u){std::fprintf(stderr,"mass row 6: %08x expected %08x\n",bits(mass),1134575194u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3007192721u),f(3208519312u),f(3208642508u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2933526104u){std::fprintf(stderr,"row 7 component 0: %08x expected %08x\n",bits(v[0]),2933526104u);return 1;}++checked;
if(bits(v[1])!=3124180505u){std::fprintf(stderr,"row 7 component 1: %08x expected %08x\n",bits(v[1]),3124180505u);return 1;}++checked;
if(bits(v[2])!=3121450456u){std::fprintf(stderr,"row 7 component 2: %08x expected %08x\n",bits(v[2]),3121450456u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141785162u){std::fprintf(stderr,"mass row 7: %08x expected %08x\n",bits(mass),1141785162u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061035664u),f(3007192721u),f(1066192102u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989453580u){std::fprintf(stderr,"row 8 component 0: %08x expected %08x\n",bits(v[0]),989453580u);return 1;}++checked;
if(bits(v[1])!=2913922957u){std::fprintf(stderr,"row 8 component 1: %08x expected %08x\n",bits(v[1]),2913922957u);return 1;}++checked;
if(bits(v[2])!=978296116u){std::fprintf(stderr,"row 8 component 2: %08x expected %08x\n",bits(v[2]),978296116u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134575211u){std::fprintf(stderr,"mass row 8: %08x expected %08x\n",bits(mass),1134575211u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3007192721u),f(3208519312u),f(1061158911u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2936853990u){std::fprintf(stderr,"row 9 component 0: %08x expected %08x\n",bits(v[0]),2936853990u);return 1;}++checked;
if(bits(v[1])!=3124180505u){std::fprintf(stderr,"row 9 component 1: %08x expected %08x\n",bits(v[1]),3124180505u);return 1;}++checked;
if(bits(v[2])!=973966846u){std::fprintf(stderr,"row 9 component 2: %08x expected %08x\n",bits(v[2]),973966846u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141785145u){std::fprintf(stderr,"mass row 9: %08x expected %08x\n",bits(mass),1141785145u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061035664u),f(3007192721u),f(1066192102u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989453580u){std::fprintf(stderr,"row 10 component 0: %08x expected %08x\n",bits(v[0]),989453580u);return 1;}++checked;
if(bits(v[1])!=2913922957u){std::fprintf(stderr,"row 10 component 1: %08x expected %08x\n",bits(v[1]),2913922957u);return 1;}++checked;
if(bits(v[2])!=978296116u){std::fprintf(stderr,"row 10 component 2: %08x expected %08x\n",bits(v[2]),978296116u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134575211u){std::fprintf(stderr,"mass row 10: %08x expected %08x\n",bits(mass),1134575211u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(562995362u),f(2913575931u),f(562995362u),f(980857529u),f(766383132u),f(2913575931u),f(766383132u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3007192721u),f(3208519312u),f(3208642561u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2933526095u){std::fprintf(stderr,"row 11 component 0: %08x expected %08x\n",bits(v[0]),2933526095u);return 1;}++checked;
if(bits(v[1])!=3124180505u){std::fprintf(stderr,"row 11 component 1: %08x expected %08x\n",bits(v[1]),3124180505u);return 1;}++checked;
if(bits(v[2])!=3121450495u){std::fprintf(stderr,"row 11 component 2: %08x expected %08x\n",bits(v[2]),3121450495u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141785144u){std::fprintf(stderr,"mass row 11: %08x expected %08x\n",bits(mass),1141785144u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642569u),f(1066192128u),f(2726666240u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137099218u){std::fprintf(stderr,"row 12 component 0: %08x expected %08x\n",bits(v[0]),3137099218u);return 1;}++checked;
if(bits(v[1])!=981970762u){std::fprintf(stderr,"row 12 component 1: %08x expected %08x\n",bits(v[1]),981970762u);return 1;}++checked;
if(bits(v[2])!=839404911u){std::fprintf(stderr,"row 12 component 2: %08x expected %08x\n",bits(v[2]),839404911u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061158903u),f(1066192128u),f(2752866498u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989615547u){std::fprintf(stderr,"row 13 component 0: %08x expected %08x\n",bits(v[0]),989615547u);return 1;}++checked;
if(bits(v[1])!=981970762u){std::fprintf(stderr,"row 13 component 1: %08x expected %08x\n",bits(v[1]),981970762u);return 1;}++checked;
if(bits(v[2])!=2981336082u){std::fprintf(stderr,"row 13 component 2: %08x expected %08x\n",bits(v[2]),2981336082u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642558u),f(3213675750u),f(605382837u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137099204u){std::fprintf(stderr,"row 14 component 0: %08x expected %08x\n",bits(v[0]),3137099204u);return 1;}++checked;
if(bits(v[1])!=3129454385u){std::fprintf(stderr,"row 14 component 1: %08x expected %08x\n",bits(v[1]),3129454385u);return 1;}++checked;
if(bits(v[2])!=833852452u){std::fprintf(stderr,"row 14 component 2: %08x expected %08x\n",bits(v[2]),833852452u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061158914u),f(3213675750u),f(579182484u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989615561u){std::fprintf(stderr,"row 15 component 0: %08x expected %08x\n",bits(v[0]),989615561u);return 1;}++checked;
if(bits(v[1])!=3129454385u){std::fprintf(stderr,"row 15 component 1: %08x expected %08x\n",bits(v[1]),3129454385u);return 1;}++checked;
if(bits(v[2])!=2986888550u){std::fprintf(stderr,"row 15 component 2: %08x expected %08x\n",bits(v[2]),2986888550u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1055758944u),f(3001916002u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983311029u){std::fprintf(stderr,"row 16 component 0: %08x expected %08x\n",bits(v[0]),983311029u);return 1;}++checked;
if(bits(v[1])!=2965217452u){std::fprintf(stderr,"row 16 component 1: %08x expected %08x\n",bits(v[1]),2965217452u);return 1;}++checked;
if(bits(v[2])!=3125779872u){std::fprintf(stderr,"row 16 component 2: %08x expected %08x\n",bits(v[2]),3125779872u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(597099183u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138589699u){std::fprintf(stderr,"mass row 16: %08x expected %08x\n",bits(mass),1138589699u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3001916002u),f(3203242592u),f(1061158922u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2984544310u){std::fprintf(stderr,"row 17 component 0: %08x expected %08x\n",bits(v[0]),2984544310u);return 1;}++checked;
if(bits(v[1])!=3118790681u){std::fprintf(stderr,"row 17 component 1: %08x expected %08x\n",bits(v[1]),3118790681u);return 1;}++checked;
if(bits(v[2])!=973966844u){std::fprintf(stderr,"row 17 component 2: %08x expected %08x\n",bits(v[2]),973966844u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(598798595u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143832006u){std::fprintf(stderr,"mass row 17: %08x expected %08x\n",bits(mass),1143832006u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1055758944u),f(3001916002u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983311029u){std::fprintf(stderr,"row 18 component 0: %08x expected %08x\n",bits(v[0]),983311029u);return 1;}++checked;
if(bits(v[1])!=2965217452u){std::fprintf(stderr,"row 18 component 1: %08x expected %08x\n",bits(v[1]),2965217452u);return 1;}++checked;
if(bits(v[2])!=3125779872u){std::fprintf(stderr,"row 18 component 2: %08x expected %08x\n",bits(v[2]),3125779872u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(597099183u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138589699u){std::fprintf(stderr,"mass row 18: %08x expected %08x\n",bits(mass),1138589699u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3001916002u),f(3203242592u),f(3208642550u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=836740794u){std::fprintf(stderr,"row 19 component 0: %08x expected %08x\n",bits(v[0]),836740794u);return 1;}++checked;
if(bits(v[1])!=3118790745u){std::fprintf(stderr,"row 19 component 1: %08x expected %08x\n",bits(v[1]),3118790745u);return 1;}++checked;
if(bits(v[2])!=3121450497u){std::fprintf(stderr,"row 19 component 2: %08x expected %08x\n",bits(v[2]),3121450497u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(598798595u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143832002u){std::fprintf(stderr,"mass row 19: %08x expected %08x\n",bits(mass),1143832002u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1055758944u),f(3001916002u),f(1066192102u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983310863u){std::fprintf(stderr,"row 20 component 0: %08x expected %08x\n",bits(v[0]),983310863u);return 1;}++checked;
if(bits(v[1])!=817265162u){std::fprintf(stderr,"row 20 component 1: %08x expected %08x\n",bits(v[1]),817265162u);return 1;}++checked;
if(bits(v[2])!=978296046u){std::fprintf(stderr,"row 20 component 2: %08x expected %08x\n",bits(v[2]),978296046u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(597099183u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138589845u){std::fprintf(stderr,"mass row 20: %08x expected %08x\n",bits(mass),1138589845u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3001916002u),f(3203242592u),f(1061158909u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2984544294u){std::fprintf(stderr,"row 21 component 0: %08x expected %08x\n",bits(v[0]),2984544294u);return 1;}++checked;
if(bits(v[1])!=3118790681u){std::fprintf(stderr,"row 21 component 1: %08x expected %08x\n",bits(v[1]),3118790681u);return 1;}++checked;
if(bits(v[2])!=973966834u){std::fprintf(stderr,"row 21 component 2: %08x expected %08x\n",bits(v[2]),973966834u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(598798595u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143832012u){std::fprintf(stderr,"mass row 21: %08x expected %08x\n",bits(mass),1143832012u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1055758944u),f(3001916002u),f(1066192102u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983310863u){std::fprintf(stderr,"row 22 component 0: %08x expected %08x\n",bits(v[0]),983310863u);return 1;}++checked;
if(bits(v[1])!=817265162u){std::fprintf(stderr,"row 22 component 1: %08x expected %08x\n",bits(v[1]),817265162u);return 1;}++checked;
if(bits(v[2])!=978296046u){std::fprintf(stderr,"row 22 component 2: %08x expected %08x\n",bits(v[2]),978296046u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(597099183u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138589845u){std::fprintf(stderr,"mass row 22: %08x expected %08x\n",bits(mass),1138589845u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(690854354u),f(2987833936u),f(690854354u),f(980857529u),f(816390972u),f(2987833936u),f(816390972u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3001916002u),f(3203242592u),f(3208642563u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=836740810u){std::fprintf(stderr,"row 23 component 0: %08x expected %08x\n",bits(v[0]),836740810u);return 1;}++checked;
if(bits(v[1])!=3118790745u){std::fprintf(stderr,"row 23 component 1: %08x expected %08x\n",bits(v[1]),3118790745u);return 1;}++checked;
if(bits(v[2])!=3121450506u){std::fprintf(stderr,"row 23 component 2: %08x expected %08x\n",bits(v[2]),3121450506u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(598798595u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143831995u){std::fprintf(stderr,"mass row 23: %08x expected %08x\n",bits(mass),1143831995u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642265u),f(1066192128u),f(726255449u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137098811u){std::fprintf(stderr,"row 24 component 0: %08x expected %08x\n",bits(v[0]),3137098811u);return 1;}++checked;
if(bits(v[1])!=981970757u){std::fprintf(stderr,"row 24 component 1: %08x expected %08x\n",bits(v[1]),981970757u);return 1;}++checked;
if(bits(v[2])!=849294223u){std::fprintf(stderr,"row 24 component 2: %08x expected %08x\n",bits(v[2]),849294223u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061159208u),f(1066192128u),f(735675519u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989615956u){std::fprintf(stderr,"row 25 component 0: %08x expected %08x\n",bits(v[0]),989615956u);return 1;}++checked;
if(bits(v[1])!=981970767u){std::fprintf(stderr,"row 25 component 1: %08x expected %08x\n",bits(v[1]),981970767u);return 1;}++checked;
if(bits(v[2])!=2989982207u){std::fprintf(stderr,"row 25 component 2: %08x expected %08x\n",bits(v[2]),2989982207u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642604u),f(3213675364u),f(2883158624u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137099272u){std::fprintf(stderr,"row 26 component 0: %08x expected %08x\n",bits(v[0]),3137099272u);return 1;}++checked;
if(bits(v[1])!=3129454018u){std::fprintf(stderr,"row 26 component 1: %08x expected %08x\n",bits(v[1]),3129454018u);return 1;}++checked;
if(bits(v[2])!=842498427u){std::fprintf(stderr,"row 26 component 2: %08x expected %08x\n",bits(v[2]),842498427u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061159380u),f(3213675619u),f(2873738278u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989616166u){std::fprintf(stderr,"row 27 component 0: %08x expected %08x\n",bits(v[0]),989616166u);return 1;}++checked;
if(bits(v[1])!=3129454254u){std::fprintf(stderr,"row 27 component 1: %08x expected %08x\n",bits(v[1]),3129454254u);return 1;}++checked;
if(bits(v[2])!=2996778332u){std::fprintf(stderr,"row 27 component 2: %08x expected %08x\n",bits(v[2]),2996778332u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1058656432u),f(883755678u),f(3213675779u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=986326870u){std::fprintf(stderr,"row 28 component 0: %08x expected %08x\n",bits(v[0]),986326870u);return 1;}++checked;
if(bits(v[1])!=2975317280u){std::fprintf(stderr,"row 28 component 1: %08x expected %08x\n",bits(v[1]),2975317280u);return 1;}++checked;
if(bits(v[2])!=3125780007u){std::fprintf(stderr,"row 28 component 2: %08x expected %08x\n",bits(v[2]),3125780007u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3037724672u),f(1065353216u),f(2878388874u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1136527012u){std::fprintf(stderr,"mass row 28: %08x expected %08x\n",bits(mass),1136527012u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(883755733u),f(3206140080u),f(1061158607u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2994014985u){std::fprintf(stderr,"row 29 component 0: %08x expected %08x\n",bits(v[0]),2994014985u);return 1;}++checked;
if(bits(v[1])!=3121887677u){std::fprintf(stderr,"row 29 component 1: %08x expected %08x\n",bits(v[1]),3121887677u);return 1;}++checked;
if(bits(v[2])!=973966584u){std::fprintf(stderr,"row 29 component 2: %08x expected %08x\n",bits(v[2]),973966584u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(890241024u),f(2870706432u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142844580u){std::fprintf(stderr,"mass row 29: %08x expected %08x\n",bits(mass),1142844580u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1058656432u),f(883755678u),f(3213675773u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=986326870u){std::fprintf(stderr,"row 30 component 0: %08x expected %08x\n",bits(v[0]),986326870u);return 1;}++checked;
if(bits(v[1])!=2975317268u){std::fprintf(stderr,"row 30 component 1: %08x expected %08x\n",bits(v[1]),2975317268u);return 1;}++checked;
if(bits(v[2])!=3125779999u){std::fprintf(stderr,"row 30 component 2: %08x expected %08x\n",bits(v[2]),3125779999u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3037724672u),f(1065353216u),f(2878388874u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1136527017u){std::fprintf(stderr,"mass row 30: %08x expected %08x\n",bits(mass),1136527017u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(883755703u),f(3206140080u),f(3208642866u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=847277833u){std::fprintf(stderr,"row 31 component 0: %08x expected %08x\n",bits(v[0]),847277833u);return 1;}++checked;
if(bits(v[1])!=3121887769u){std::fprintf(stderr,"row 31 component 1: %08x expected %08x\n",bits(v[1]),3121887769u);return 1;}++checked;
if(bits(v[2])!=3121450757u){std::fprintf(stderr,"row 31 component 2: %08x expected %08x\n",bits(v[2]),3121450757u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(890241024u),f(2870706432u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142844279u){std::fprintf(stderr,"mass row 31: %08x expected %08x\n",bits(mass),1142844279u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1058656432u),f(883755758u),f(1066191713u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=986326502u){std::fprintf(stderr,"row 32 component 0: %08x expected %08x\n",bits(v[0]),986326502u);return 1;}++checked;
if(bits(v[1])!=832696368u){std::fprintf(stderr,"row 32 component 1: %08x expected %08x\n",bits(v[1]),832696368u);return 1;}++checked;
if(bits(v[2])!=978295341u){std::fprintf(stderr,"row 32 component 2: %08x expected %08x\n",bits(v[2]),978295341u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3037724672u),f(1065353216u),f(2878388874u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1136527629u){std::fprintf(stderr,"mass row 32: %08x expected %08x\n",bits(mass),1136527629u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(883755733u),f(3206140080u),f(1061158966u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2994015455u){std::fprintf(stderr,"row 33 component 0: %08x expected %08x\n",bits(v[0]),2994015455u);return 1;}++checked;
if(bits(v[1])!=3121887677u){std::fprintf(stderr,"row 33 component 1: %08x expected %08x\n",bits(v[1]),3121887677u);return 1;}++checked;
if(bits(v[2])!=973966849u){std::fprintf(stderr,"row 33 component 2: %08x expected %08x\n",bits(v[2]),973966849u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(890241024u),f(2870706432u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142844428u){std::fprintf(stderr,"mass row 33: %08x expected %08x\n",bits(mass),1142844428u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1058656432u),f(883755758u),f(1066191974u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=986326502u){std::fprintf(stderr,"row 34 component 0: %08x expected %08x\n",bits(v[0]),986326502u);return 1;}++checked;
if(bits(v[1])!=832696618u){std::fprintf(stderr,"row 34 component 1: %08x expected %08x\n",bits(v[1]),832696618u);return 1;}++checked;
if(bits(v[2])!=978295726u){std::fprintf(stderr,"row 34 component 2: %08x expected %08x\n",bits(v[2]),978295726u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3037724672u),f(1065353216u),f(2878388874u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1136527396u){std::fprintf(stderr,"mass row 34: %08x expected %08x\n",bits(mass),1136527396u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(811416137u),f(2997308852u),f(811416137u),f(980857529u),f(829769888u),f(2997308852u),f(829769888u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(883755703u),f(3206140080u),f(3208643018u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=847277932u){std::fprintf(stderr,"row 35 component 0: %08x expected %08x\n",bits(v[0]),847277932u);return 1;}++checked;
if(bits(v[1])!=3121887769u){std::fprintf(stderr,"row 35 component 1: %08x expected %08x\n",bits(v[1]),3121887769u);return 1;}++checked;
if(bits(v[2])!=3121450869u){std::fprintf(stderr,"row 35 component 2: %08x expected %08x\n",bits(v[2]),3121450869u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(890241024u),f(2870706432u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142844215u){std::fprintf(stderr,"mass row 35: %08x expected %08x\n",bits(mass),1142844215u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642611u),f(1066192128u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137099274u){std::fprintf(stderr,"row 36 component 0: %08x expected %08x\n",bits(v[0]),3137099274u);return 1;}++checked;
if(bits(v[1])!=981970762u){std::fprintf(stderr,"row 36 component 1: %08x expected %08x\n",bits(v[1]),981970762u);return 1;}++checked;
if(bits(v[2])!=778596536u){std::fprintf(stderr,"row 36 component 2: %08x expected %08x\n",bits(v[2]),778596536u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061158861u),f(1066192128u),f(2147483648u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989615491u){std::fprintf(stderr,"row 37 component 0: %08x expected %08x\n",bits(v[0]),989615491u);return 1;}++checked;
if(bits(v[1])!=981970762u){std::fprintf(stderr,"row 37 component 1: %08x expected %08x\n",bits(v[1]),981970762u);return 1;}++checked;
if(bits(v[2])!=2903719406u){std::fprintf(stderr,"row 37 component 2: %08x expected %08x\n",bits(v[2]),2903719406u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642560u),f(3213675750u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137099206u){std::fprintf(stderr,"row 38 component 0: %08x expected %08x\n",bits(v[0]),3137099206u);return 1;}++checked;
if(bits(v[1])!=3129454385u){std::fprintf(stderr,"row 38 component 1: %08x expected %08x\n",bits(v[1]),3129454385u);return 1;}++checked;
if(bits(v[2])!=756235974u){std::fprintf(stderr,"row 38 component 2: %08x expected %08x\n",bits(v[2]),756235974u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061158912u),f(3213675750u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989615558u){std::fprintf(stderr,"row 39 component 0: %08x expected %08x\n",bits(v[0]),989615558u);return 1;}++checked;
if(bits(v[1])!=3129454385u){std::fprintf(stderr,"row 39 component 1: %08x expected %08x\n",bits(v[1]),3129454385u);return 1;}++checked;
if(bits(v[2])!=2926080130u){std::fprintf(stderr,"row 39 component 2: %08x expected %08x\n",bits(v[2]),2926080130u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1060520048u),f(3006677105u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988775935u){std::fprintf(stderr,"row 40 component 0: %08x expected %08x\n",bits(v[0]),988775935u);return 1;}++checked;
if(bits(v[1])!=2928195794u){std::fprintf(stderr,"row 40 component 1: %08x expected %08x\n",bits(v[1]),2928195794u);return 1;}++checked;
if(bits(v[2])!=3125779802u){std::fprintf(stderr,"row 40 component 2: %08x expected %08x\n",bits(v[2]),3125779802u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134976146u){std::fprintf(stderr,"mass row 40: %08x expected %08x\n",bits(mass),1134976146u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3006677105u),f(3208003696u),f(1061158964u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2937678608u){std::fprintf(stderr,"row 41 component 0: %08x expected %08x\n",bits(v[0]),2937678608u);return 1;}++checked;
if(bits(v[1])!=3123683624u){std::fprintf(stderr,"row 41 component 1: %08x expected %08x\n",bits(v[1]),3123683624u);return 1;}++checked;
if(bits(v[2])!=973966885u){std::fprintf(stderr,"row 41 component 2: %08x expected %08x\n",bits(v[2]),973966885u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142013785u){std::fprintf(stderr,"mass row 41: %08x expected %08x\n",bits(mass),1142013785u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1060520048u),f(3006677105u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988775935u){std::fprintf(stderr,"row 42 component 0: %08x expected %08x\n",bits(v[0]),988775935u);return 1;}++checked;
if(bits(v[1])!=2928195794u){std::fprintf(stderr,"row 42 component 1: %08x expected %08x\n",bits(v[1]),2928195794u);return 1;}++checked;
if(bits(v[2])!=3125779802u){std::fprintf(stderr,"row 42 component 2: %08x expected %08x\n",bits(v[2]),3125779802u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134976146u){std::fprintf(stderr,"mass row 42: %08x expected %08x\n",bits(mass),1134976146u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3006677105u),f(3208003696u),f(3208642508u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2930521605u){std::fprintf(stderr,"row 43 component 0: %08x expected %08x\n",bits(v[0]),2930521605u);return 1;}++checked;
if(bits(v[1])!=3123683624u){std::fprintf(stderr,"row 43 component 1: %08x expected %08x\n",bits(v[1]),3123683624u);return 1;}++checked;
if(bits(v[2])!=3121450456u){std::fprintf(stderr,"row 43 component 2: %08x expected %08x\n",bits(v[2]),3121450456u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142013822u){std::fprintf(stderr,"mass row 43: %08x expected %08x\n",bits(mass),1142013822u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1060520048u),f(3006677105u),f(1066192102u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988775935u){std::fprintf(stderr,"row 44 component 0: %08x expected %08x\n",bits(v[0]),988775935u);return 1;}++checked;
if(bits(v[1])!=2912657793u){std::fprintf(stderr,"row 44 component 1: %08x expected %08x\n",bits(v[1]),2912657793u);return 1;}++checked;
if(bits(v[2])!=978296116u){std::fprintf(stderr,"row 44 component 2: %08x expected %08x\n",bits(v[2]),978296116u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134976164u){std::fprintf(stderr,"mass row 44: %08x expected %08x\n",bits(mass),1134976164u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3006677105u),f(3208003696u),f(1061158911u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2937678598u){std::fprintf(stderr,"row 45 component 0: %08x expected %08x\n",bits(v[0]),2937678598u);return 1;}++checked;
if(bits(v[1])!=3123683624u){std::fprintf(stderr,"row 45 component 1: %08x expected %08x\n",bits(v[1]),3123683624u);return 1;}++checked;
if(bits(v[2])!=973966846u){std::fprintf(stderr,"row 45 component 2: %08x expected %08x\n",bits(v[2]),973966846u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142013804u){std::fprintf(stderr,"mass row 45: %08x expected %08x\n",bits(mass),1142013804u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1060520048u),f(3006677105u),f(1066192102u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988775935u){std::fprintf(stderr,"row 46 component 0: %08x expected %08x\n",bits(v[0]),988775935u);return 1;}++checked;
if(bits(v[1])!=2912657793u){std::fprintf(stderr,"row 46 component 1: %08x expected %08x\n",bits(v[1]),2912657793u);return 1;}++checked;
if(bits(v[2])!=978296116u){std::fprintf(stderr,"row 46 component 2: %08x expected %08x\n",bits(v[2]),978296116u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864026625u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134976164u){std::fprintf(stderr,"mass row 46: %08x expected %08x\n",bits(mass),1134976164u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(572141158u),f(2922610723u),f(572141158u),f(980857529u),f(766629860u),f(2922610723u),f(766629860u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3006677105u),f(3208003696u),f(3208642561u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2930521586u){std::fprintf(stderr,"row 47 component 0: %08x expected %08x\n",bits(v[0]),2930521586u);return 1;}++checked;
if(bits(v[1])!=3123683624u){std::fprintf(stderr,"row 47 component 1: %08x expected %08x\n",bits(v[1]),3123683624u);return 1;}++checked;
if(bits(v[2])!=3121450495u){std::fprintf(stderr,"row 47 component 2: %08x expected %08x\n",bits(v[2]),3121450495u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(3011510273u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1142013803u){std::fprintf(stderr,"mass row 47: %08x expected %08x\n",bits(mass),1142013803u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642610u),f(1066192128u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137099272u){std::fprintf(stderr,"row 48 component 0: %08x expected %08x\n",bits(v[0]),3137099272u);return 1;}++checked;
if(bits(v[1])!=981970762u){std::fprintf(stderr,"row 48 component 1: %08x expected %08x\n",bits(v[1]),981970762u);return 1;}++checked;
if(bits(v[2])!=780909460u){std::fprintf(stderr,"row 48 component 2: %08x expected %08x\n",bits(v[2]),780909460u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061158863u),f(1066192128u),f(2147483648u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989615494u){std::fprintf(stderr,"row 49 component 0: %08x expected %08x\n",bits(v[0]),989615494u);return 1;}++checked;
if(bits(v[1])!=981970762u){std::fprintf(stderr,"row 49 component 1: %08x expected %08x\n",bits(v[1]),981970762u);return 1;}++checked;
if(bits(v[2])!=764281004u){std::fprintf(stderr,"row 49 component 2: %08x expected %08x\n",bits(v[2]),764281004u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(3208642560u),f(3213675750u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137099206u){std::fprintf(stderr,"row 50 component 0: %08x expected %08x\n",bits(v[0]),3137099206u);return 1;}++checked;
if(bits(v[1])!=3129454385u){std::fprintf(stderr,"row 50 component 1: %08x expected %08x\n",bits(v[1]),3129454385u);return 1;}++checked;
if(bits(v[2])!=2911764534u){std::fprintf(stderr,"row 50 component 2: %08x expected %08x\n",bits(v[2]),2911764534u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1061158912u),f(3213675750u),f(0u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989615558u){std::fprintf(stderr,"row 51 component 0: %08x expected %08x\n",bits(v[0]),989615558u);return 1;}++checked;
if(bits(v[1])!=3129454385u){std::fprintf(stderr,"row 51 component 1: %08x expected %08x\n",bits(v[1]),3129454385u);return 1;}++checked;
if(bits(v[2])!=2928393078u){std::fprintf(stderr,"row 51 component 2: %08x expected %08x\n",bits(v[2]),2928393078u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1060551440u),f(859224848u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988817191u){std::fprintf(stderr,"row 52 component 0: %08x expected %08x\n",bits(v[0]),988817191u);return 1;}++checked;
if(bits(v[1])!=710651904u){std::fprintf(stderr,"row 52 component 1: %08x expected %08x\n",bits(v[1]),710651904u);return 1;}++checked;
if(bits(v[2])!=3125779802u){std::fprintf(stderr,"row 52 component 2: %08x expected %08x\n",bits(v[2]),3125779802u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3011510272u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134951363u){std::fprintf(stderr,"mass row 52: %08x expected %08x\n",bits(mass),1134951363u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(859224847u),f(3208035088u),f(1061158961u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=784075108u){std::fprintf(stderr,"row 53 component 0: %08x expected %08x\n",bits(v[0]),784075108u);return 1;}++checked;
if(bits(v[1])!=3123713875u){std::fprintf(stderr,"row 53 component 1: %08x expected %08x\n",bits(v[1]),3123713875u);return 1;}++checked;
if(bits(v[2])!=973966882u){std::fprintf(stderr,"row 53 component 2: %08x expected %08x\n",bits(v[2]),973966882u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(864026623u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141999827u){std::fprintf(stderr,"mass row 53: %08x expected %08x\n",bits(mass),1141999827u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1060551440u),f(859224848u),f(3213675776u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988817191u){std::fprintf(stderr,"row 54 component 0: %08x expected %08x\n",bits(v[0]),988817191u);return 1;}++checked;
if(bits(v[1])!=710651904u){std::fprintf(stderr,"row 54 component 1: %08x expected %08x\n",bits(v[1]),710651904u);return 1;}++checked;
if(bits(v[2])!=3125779802u){std::fprintf(stderr,"row 54 component 2: %08x expected %08x\n",bits(v[2]),3125779802u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3011510272u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134951363u){std::fprintf(stderr,"mass row 54: %08x expected %08x\n",bits(mass),1134951363u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(859224847u),f(3208035088u),f(3208642512u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=789717607u){std::fprintf(stderr,"row 55 component 0: %08x expected %08x\n",bits(v[0]),789717607u);return 1;}++checked;
if(bits(v[1])!=3123713875u){std::fprintf(stderr,"row 55 component 1: %08x expected %08x\n",bits(v[1]),3123713875u);return 1;}++checked;
if(bits(v[2])!=3121450459u){std::fprintf(stderr,"row 55 component 2: %08x expected %08x\n",bits(v[2]),3121450459u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(864026623u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141999862u){std::fprintf(stderr,"mass row 55: %08x expected %08x\n",bits(mass),1141999862u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1060551440u),f(859224848u),f(1066192102u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988817191u){std::fprintf(stderr,"row 56 component 0: %08x expected %08x\n",bits(v[0]),988817191u);return 1;}++checked;
if(bits(v[1])!=783264108u){std::fprintf(stderr,"row 56 component 1: %08x expected %08x\n",bits(v[1]),783264108u);return 1;}++checked;
if(bits(v[2])!=978296116u){std::fprintf(stderr,"row 56 component 2: %08x expected %08x\n",bits(v[2]),978296116u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3011510272u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134951381u){std::fprintf(stderr,"mass row 56: %08x expected %08x\n",bits(mass),1134951381u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(859224847u),f(3208035088u),f(1061158913u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=784075121u){std::fprintf(stderr,"row 57 component 0: %08x expected %08x\n",bits(v[0]),784075121u);return 1;}++checked;
if(bits(v[1])!=3123713875u){std::fprintf(stderr,"row 57 component 1: %08x expected %08x\n",bits(v[1]),3123713875u);return 1;}++checked;
if(bits(v[2])!=973966847u){std::fprintf(stderr,"row 57 component 2: %08x expected %08x\n",bits(v[2]),973966847u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(864026623u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141999844u){std::fprintf(stderr,"mass row 57: %08x expected %08x\n",bits(mass),1141999844u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(1060551440u),f(859224848u),f(1066192102u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=988817191u){std::fprintf(stderr,"row 58 component 0: %08x expected %08x\n",bits(v[0]),988817191u);return 1;}++checked;
if(bits(v[1])!=783264108u){std::fprintf(stderr,"row 58 component 1: %08x expected %08x\n",bits(v[1]),783264108u);return 1;}++checked;
if(bits(v[2])!=978296116u){std::fprintf(stderr,"row 58 component 2: %08x expected %08x\n",bits(v[2]),978296116u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3011510272u),f(1065353216u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1134951381u){std::fprintf(stderr,"mass row 58: %08x expected %08x\n",bits(mass),1134951381u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(576051923u),f(2919954913u),f(576051923u),f(980857529u),f(773803332u),f(2919954913u),f(773803332u),f(977059155u)}; motorrock_native_wheel_row_v56::Vector r{f(859224847u),f(3208035088u),f(3208642559u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=789717613u){std::fprintf(stderr,"row 59 component 0: %08x expected %08x\n",bits(v[0]),789717613u);return 1;}++checked;
if(bits(v[1])!=3123713875u){std::fprintf(stderr,"row 59 component 1: %08x expected %08x\n",bits(v[1]),3123713875u);return 1;}++checked;
if(bits(v[2])!=3121450494u){std::fprintf(stderr,"row 59 component 2: %08x expected %08x\n",bits(v[2]),3121450494u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353216u),f(864026623u),f(0u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1141999845u){std::fprintf(stderr,"mass row 59: %08x expected %08x\n",bits(mass),1141999845u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(3208580638u),f(1066199865u),f(3180910617u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137019891u){std::fprintf(stderr,"row 60 component 0: %08x expected %08x\n",bits(v[0]),3137019891u);return 1;}++checked;
if(bits(v[1])!=981978218u){std::fprintf(stderr,"row 60 component 1: %08x expected %08x\n",bits(v[1]),981978218u);return 1;}++checked;
if(bits(v[2])!=3108392538u){std::fprintf(stderr,"row 60 component 2: %08x expected %08x\n",bits(v[2]),3108392538u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(3208580868u),f(3213667808u),f(3180910801u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3137020197u){std::fprintf(stderr,"row 61 component 0: %08x expected %08x\n",bits(v[0]),3137020197u);return 1;}++checked;
if(bits(v[1])!=3129446732u){std::fprintf(stderr,"row 61 component 1: %08x expected %08x\n",bits(v[1]),3129446732u);return 1;}++checked;
if(bits(v[2])!=3108392645u){std::fprintf(stderr,"row 61 component 2: %08x expected %08x\n",bits(v[2]),3108392645u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(1061095694u),f(3213667850u),f(1033425932u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989534539u){std::fprintf(stderr,"row 62 component 0: %08x expected %08x\n",bits(v[0]),989534539u);return 1;}++checked;
if(bits(v[1])!=3129446772u){std::fprintf(stderr,"row 62 component 1: %08x expected %08x\n",bits(v[1]),3129446772u);return 1;}++checked;
if(bits(v[2])!=960907544u){std::fprintf(stderr,"row 62 component 2: %08x expected %08x\n",bits(v[2]),960907544u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(1061095930u),f(1066199866u),f(1033426121u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989534853u){std::fprintf(stderr,"row 63 component 0: %08x expected %08x\n",bits(v[0]),989534853u);return 1;}++checked;
if(bits(v[1])!=981978219u){std::fprintf(stderr,"row 63 component 1: %08x expected %08x\n",bits(v[1]),981978219u);return 1;}++checked;
if(bits(v[2])!=960907658u){std::fprintf(stderr,"row 63 component 2: %08x expected %08x\n",bits(v[2]),960907658u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(1058828263u),f(851482738u),f(3213216962u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=984846874u){std::fprintf(stderr,"row 64 component 0: %08x expected %08x\n",bits(v[0]),984846874u);return 1;}++checked;
if(bits(v[1])!=815002837u){std::fprintf(stderr,"row 64 component 1: %08x expected %08x\n",bits(v[1]),815002837u);return 1;}++checked;
if(bits(v[2])!=3123534142u){std::fprintf(stderr,"row 64 component 2: %08x expected %08x\n",bits(v[2]),3123534142u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3007191400u),f(1065353210u),f(2979532567u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1137947865u){std::fprintf(stderr,"mass row 64: %08x expected %08x\n",bits(mass),1137947865u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(3180910613u),f(3204515993u),f(1061096989u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3094062792u){std::fprintf(stderr,"row 65 component 0: %08x expected %08x\n",bits(v[0]),3094062792u);return 1;}++checked;
if(bits(v[1])!=3120083143u){std::fprintf(stderr,"row 65 component 1: %08x expected %08x\n",bits(v[1]),3120083143u);return 1;}++checked;
if(bits(v[2])!=973917065u){std::fprintf(stderr,"row 65 component 2: %08x expected %08x\n",bits(v[2]),973917065u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065269948u),f(859769876u),f(1036765336u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143551485u){std::fprintf(stderr,"mass row 65: %08x expected %08x\n",bits(mass),1143551485u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(1058828233u),f(851482692u),f(3213216964u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=984846834u){std::fprintf(stderr,"row 66 component 0: %08x expected %08x\n",bits(v[0]),984846834u);return 1;}++checked;
if(bits(v[1])!=815002835u){std::fprintf(stderr,"row 66 component 1: %08x expected %08x\n",bits(v[1]),815002835u);return 1;}++checked;
if(bits(v[2])!=3123534150u){std::fprintf(stderr,"row 66 component 2: %08x expected %08x\n",bits(v[2]),3123534150u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3007191400u),f(1065353210u),f(2979532567u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1137947893u){std::fprintf(stderr,"mass row 66: %08x expected %08x\n",bits(mass),1137947893u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(1033426125u),f(3204515964u),f(3208579579u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=946577808u){std::fprintf(stderr,"row 67 component 0: %08x expected %08x\n",bits(v[0]),946577808u);return 1;}++checked;
if(bits(v[1])!=3120083042u){std::fprintf(stderr,"row 67 component 1: %08x expected %08x\n",bits(v[1]),3120083042u);return 1;}++checked;
if(bits(v[2])!=3121399918u){std::fprintf(stderr,"row 67 component 2: %08x expected %08x\n",bits(v[2]),3121399918u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065269948u),f(859769876u),f(1036765336u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143552020u){std::fprintf(stderr,"mass row 67: %08x expected %08x\n",bits(mass),1143552020u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(1053346615u),f(851481994u),f(1066559124u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983431266u){std::fprintf(stderr,"row 68 component 0: %08x expected %08x\n",bits(v[0]),983431266u);return 1;}++checked;
if(bits(v[1])!=2960433436u){std::fprintf(stderr,"row 68 component 1: %08x expected %08x\n",bits(v[1]),2960433436u);return 1;}++checked;
if(bits(v[2])!=980394297u){std::fprintf(stderr,"row 68 component 2: %08x expected %08x\n",bits(v[2]),980394297u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3007191400u),f(1065353210u),f(2979532567u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138008328u){std::fprintf(stderr,"mass row 68: %08x expected %08x\n",bits(mass),1138008328u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(3180910799u),f(3204515987u),f(1061097221u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3094063080u){std::fprintf(stderr,"row 69 component 0: %08x expected %08x\n",bits(v[0]),3094063080u);return 1;}++checked;
if(bits(v[1])!=3120083132u){std::fprintf(stderr,"row 69 component 1: %08x expected %08x\n",bits(v[1]),3120083132u);return 1;}++checked;
if(bits(v[2])!=973917236u){std::fprintf(stderr,"row 69 component 2: %08x expected %08x\n",bits(v[2]),973917236u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065269948u),f(859769876u),f(1036765336u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143551375u){std::fprintf(stderr,"mass row 69: %08x expected %08x\n",bits(mass),1143551375u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(1053346615u),f(851482006u),f(1066559166u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=983431274u){std::fprintf(stderr,"row 70 component 0: %08x expected %08x\n",bits(v[0]),983431274u);return 1;}++checked;
if(bits(v[1])!=2960433516u){std::fprintf(stderr,"row 70 component 1: %08x expected %08x\n",bits(v[1]),2960433516u);return 1;}++checked;
if(bits(v[2])!=980394360u){std::fprintf(stderr,"row 70 component 2: %08x expected %08x\n",bits(v[2]),980394360u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(3007191400u),f(1065353210u),f(2979532567u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138008274u){std::fprintf(stderr,"mass row 70: %08x expected %08x\n",bits(mass),1138008274u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992416052u),f(796698854u),f(960257632u),f(796698854u),f(980857529u),f(2960170120u),f(960257632u),f(2960170120u),f(977362238u)}; motorrock_native_wheel_row_v56::Vector r{f(1033425934u),f(3204515995u),f(3208579341u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=946577512u){std::fprintf(stderr,"row 71 component 0: %08x expected %08x\n",bits(v[0]),946577512u);return 1;}++checked;
if(bits(v[1])!=3120083101u){std::fprintf(stderr,"row 71 component 1: %08x expected %08x\n",bits(v[1]),3120083101u);return 1;}++checked;
if(bits(v[2])!=3121399743u){std::fprintf(stderr,"row 71 component 2: %08x expected %08x\n",bits(v[2]),3121399743u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065269948u),f(859769876u),f(1036765336u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143552124u){std::fprintf(stderr,"mass row 71: %08x expected %08x\n",bits(mass),1143552124u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(3208285661u),f(1066146706u),f(1038101533u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3136630151u){std::fprintf(stderr,"row 72 component 0: %08x expected %08x\n",bits(v[0]),3136630151u);return 1;}++checked;
if(bits(v[1])!=981913780u){std::fprintf(stderr,"row 72 component 1: %08x expected %08x\n",bits(v[1]),981913780u);return 1;}++checked;
if(bits(v[2])!=955141400u){std::fprintf(stderr,"row 72 component 2: %08x expected %08x\n",bits(v[2]),955141400u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(3208285338u),f(3213629773u),f(3185584251u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=3136629732u){std::fprintf(stderr,"row 73 component 0: %08x expected %08x\n",bits(v[0]),3136629732u);return 1;}++checked;
if(bits(v[1])!=3129396881u){std::fprintf(stderr,"row 73 component 1: %08x expected %08x\n",bits(v[1]),3129396881u);return 1;}++checked;
if(bits(v[2])!=3102622836u){std::fprintf(stderr,"row 73 component 2: %08x expected %08x\n",bits(v[2]),3102622836u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(1061541026u),f(3213630065u),f(3185584717u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989986745u){std::fprintf(stderr,"row 74 component 0: %08x expected %08x\n",bits(v[0]),989986745u);return 1;}++checked;
if(bits(v[1])!=3129397150u){std::fprintf(stderr,"row 74 component 1: %08x expected %08x\n",bits(v[1]),3129397150u);return 1;}++checked;
if(bits(v[2])!=3102624592u){std::fprintf(stderr,"row 74 component 2: %08x expected %08x\n",bits(v[2]),3102624592u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(1061539948u),f(1066146706u),f(1038101533u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=989986041u){std::fprintf(stderr,"row 75 component 0: %08x expected %08x\n",bits(v[0]),989986041u);return 1;}++checked;
if(bits(v[1])!=981913792u){std::fprintf(stderr,"row 75 component 1: %08x expected %08x\n",bits(v[1]),981913792u);return 1;}++checked;
if(bits(v[2])!=955140140u){std::fprintf(stderr,"row 75 component 2: %08x expected %08x\n",bits(v[2]),955140140u);return 1;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(1056934828u),f(1038101528u),f(3213630354u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=984083704u){std::fprintf(stderr,"row 76 component 0: %08x expected %08x\n",bits(v[0]),984083704u);return 1;}++checked;
if(bits(v[1])!=948896412u){std::fprintf(stderr,"row 76 component 1: %08x expected %08x\n",bits(v[1]),948896412u);return 1;}++checked;
if(bits(v[2])!=3125739288u){std::fprintf(stderr,"row 76 component 2: %08x expected %08x\n",bits(v[2]),3125739288u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864034780u),f(1065269945u),f(1036765332u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138049618u){std::fprintf(stderr,"mass row 76: %08x expected %08x\n",bits(mass),1138049618u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(3003108271u),f(3205608342u),f(1059908122u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2980262255u){std::fprintf(stderr,"row 77 component 0: %08x expected %08x\n",bits(v[0]),2980262255u);return 1;}++checked;
if(bits(v[1])!=3120967073u){std::fprintf(stderr,"row 77 component 1: %08x expected %08x\n",bits(v[1]),3120967073u);return 1;}++checked;
if(bits(v[2])!=972503038u){std::fprintf(stderr,"row 77 component 2: %08x expected %08x\n",bits(v[2]),972503038u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353207u),f(3011443296u),f(2982935184u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143815064u){std::fprintf(stderr,"mass row 77: %08x expected %08x\n",bits(mass),1143815064u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(1056934830u),f(3185584256u),f(1066146125u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=984083589u){std::fprintf(stderr,"row 78 component 0: %08x expected %08x\n",bits(v[0]),984083589u);return 1;}++checked;
if(bits(v[1])!=3096379344u){std::fprintf(stderr,"row 78 component 1: %08x expected %08x\n",bits(v[1]),3096379344u);return 1;}++checked;
if(bits(v[2])!=978254676u){std::fprintf(stderr,"row 78 component 2: %08x expected %08x\n",bits(v[2]),978254676u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864034780u),f(1065269945u),f(1036765332u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138050357u){std::fprintf(stderr,"mass row 78: %08x expected %08x\n",bits(mass),1138050357u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(3003108273u),f(3205608311u),f(1059907799u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=2980261986u){std::fprintf(stderr,"row 79 component 0: %08x expected %08x\n",bits(v[0]),2980261986u);return 1;}++checked;
if(bits(v[1])!=3120967054u){std::fprintf(stderr,"row 79 component 1: %08x expected %08x\n",bits(v[1]),3120967054u);return 1;}++checked;
if(bits(v[2])!=972502560u){std::fprintf(stderr,"row 79 component 2: %08x expected %08x\n",bits(v[2]),972502560u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353207u),f(3011443296u),f(2982935184u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143815221u){std::fprintf(stderr,"mass row 79: %08x expected %08x\n",bits(mass),1143815221u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(1056934918u),f(3185584720u),f(1066146416u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=984083647u){std::fprintf(stderr,"row 80 component 0: %08x expected %08x\n",bits(v[0]),984083647u);return 1;}++checked;
if(bits(v[1])!=3096379640u){std::fprintf(stderr,"row 80 component 1: %08x expected %08x\n",bits(v[1]),3096379640u);return 1;}++checked;
if(bits(v[2])!=978255106u){std::fprintf(stderr,"row 80 component 2: %08x expected %08x\n",bits(v[2]),978255106u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864034780u),f(1065269945u),f(1036765332u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138049986u){std::fprintf(stderr,"mass row 80: %08x expected %08x\n",bits(mass),1138049986u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(3003108272u),f(3201755314u),f(3209793552u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=832256362u){std::fprintf(stderr,"row 81 component 0: %08x expected %08x\n",bits(v[0]),832256362u);return 1;}++checked;
if(bits(v[1])!=3118173916u){std::fprintf(stderr,"row 81 component 1: %08x expected %08x\n",bits(v[1]),3118173916u);return 1;}++checked;
if(bits(v[2])!=3122589029u){std::fprintf(stderr,"row 81 component 2: %08x expected %08x\n",bits(v[2]),3122589029u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353207u),f(3011443296u),f(2982935184u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143345587u){std::fprintf(stderr,"mass row 81: %08x expected %08x\n",bits(mass),1143345587u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(1056934894u),f(1038101530u),f(3213630354u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=984083747u){std::fprintf(stderr,"row 82 component 0: %08x expected %08x\n",bits(v[0]),984083747u);return 1;}++checked;
if(bits(v[1])!=948896414u){std::fprintf(stderr,"row 82 component 1: %08x expected %08x\n",bits(v[1]),948896414u);return 1;}++checked;
if(bits(v[2])!=3125739288u){std::fprintf(stderr,"row 82 component 2: %08x expected %08x\n",bits(v[2]),3125739288u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(864034780u),f(1065269945u),f(1036765332u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1138049588u){std::fprintf(stderr,"mass row 82: %08x expected %08x\n",bits(mass),1138049588u);return 2;}++checked;
}
{ motorrock_native_wheel_row_v56::Matrix m{f(992491823u),f(813988587u),f(2983317437u),f(813988587u),f(980779535u),f(939754208u),f(2983317437u),f(939754208u),f(977137149u)}; motorrock_native_wheel_row_v56::Vector r{f(3003108248u),f(3201755505u),f(3209792476u)};auto v=motorrock_native_wheel_row_v56::multiply(m,r);
if(bits(v[0])!=832255468u){std::fprintf(stderr,"row 83 component 0: %08x expected %08x\n",bits(v[0]),832255468u);return 1;}++checked;
if(bits(v[1])!=3118174030u){std::fprintf(stderr,"row 83 component 1: %08x expected %08x\n",bits(v[1]),3118174030u);return 1;}++checked;
if(bits(v[2])!=3122588234u){std::fprintf(stderr,"row 83 component 2: %08x expected %08x\n",bits(v[2]),3122588234u);return 1;}++checked;
motorrock_native_wheel_row_v56::Vector n{f(1065353207u),f(3011443296u),f(2982935184u)};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f(979006478u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!=1143346115u){std::fprintf(stderr,"mass row 83: %08x expected %08x\n",bits(mass),1143346115u);return 2;}++checked;
}
std::printf("%u original inverse-jacobian/hard-mass float32 words exact\n",checked);}
