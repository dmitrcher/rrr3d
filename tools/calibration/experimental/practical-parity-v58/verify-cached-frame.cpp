#include "NativeWheelFrameV57.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
float f(uint32_t w){float x;std::memcpy(&x,&w,4);return x;}
uint32_t bits(float x){uint32_t w;std::memcpy(&w,&x,4);return w;}
int main(){using namespace motorrock_native_wheel_frame_v57;unsigned checks=0;
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(3213195776u),f(3212836923u),f(1092616191u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213516622u),f(3212836908u),f(3172516204u)}};
Vector local_axis{{f(978320492u),f(899555206u),f(1065353212u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=725298206u){std::fprintf(stderr,"record 0 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),725298206u);return 1;}++checks;
if(bits(got[1])!=2845986816u){std::fprintf(stderr,"record 0 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),2845986816u);return 1;}++checks;
if(bits(got[2])!=1065353217u){std::fprintf(stderr,"record 0 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353217u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212836920u){std::fprintf(stderr,"record 0 jacobian[0] got %08x expected %08x\n",bits(got[0]),3212836920u);return 1;}++checks;
if(bits(got[1])!=1066032715u){std::fprintf(stderr,"record 0 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066032715u);return 1;}++checks;
if(bits(got[2])!=726735656u){std::fprintf(stderr,"record 0 jacobian[2] got %08x expected %08x\n",bits(got[2]),726735656u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(3213195264u),f(1065353669u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213516091u),f(1065353686u),f(3172516594u)}};
Vector local_axis{{f(978320492u),f(899555206u),f(1065353212u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=725298206u){std::fprintf(stderr,"record 1 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),725298206u);return 1;}++checks;
if(bits(got[1])!=2845986816u){std::fprintf(stderr,"record 1 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),2845986816u);return 1;}++checks;
if(bits(got[2])!=1065353217u){std::fprintf(stderr,"record 1 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353217u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065353678u){std::fprintf(stderr,"record 1 jacobian[0] got %08x expected %08x\n",bits(got[0]),1065353678u);return 1;}++checks;
if(bits(got[1])!=1066032203u){std::fprintf(stderr,"record 1 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066032203u);return 1;}++checks;
if(bits(got[2])!=2871345239u){std::fprintf(stderr,"record 1 jacobian[2] got %08x expected %08x\n",bits(got[2]),2871345239u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(1066319425u),f(3212837248u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1065998575u),f(3212837254u),f(3172975547u)}};
Vector local_axis{{f(978320492u),f(899555206u),f(1065353212u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=725298206u){std::fprintf(stderr,"record 2 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),725298206u);return 1;}++checks;
if(bits(got[1])!=2845986816u){std::fprintf(stderr,"record 2 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),2845986816u);return 1;}++checks;
if(bits(got[2])!=1065353217u){std::fprintf(stderr,"record 2 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353217u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212837245u){std::fprintf(stderr,"record 2 jacobian[0] got %08x expected %08x\n",bits(got[0]),3212837245u);return 1;}++checks;
if(bits(got[1])!=3213482491u){std::fprintf(stderr,"record 2 jacobian[1] got %08x expected %08x\n",bits(got[1]),3213482491u);return 1;}++checks;
if(bits(got[2])!=723866761u){std::fprintf(stderr,"record 2 jacobian[2] got %08x expected %08x\n",bits(got[2]),723866761u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(1066319679u),f(1065353216u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1065998848u),f(1065353208u),f(3172976186u)}};
Vector local_axis{{f(978320492u),f(899555206u),f(1065353212u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=725298206u){std::fprintf(stderr,"record 3 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),725298206u);return 1;}++checks;
if(bits(got[1])!=2845986816u){std::fprintf(stderr,"record 3 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),2845986816u);return 1;}++checks;
if(bits(got[2])!=1065353217u){std::fprintf(stderr,"record 3 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353217u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065353225u){std::fprintf(stderr,"record 3 jacobian[0] got %08x expected %08x\n",bits(got[0]),1065353225u);return 1;}++checks;
if(bits(got[1])!=3213482745u){std::fprintf(stderr,"record 3 jacobian[1] got %08x expected %08x\n",bits(got[1]),3213482745u);return 1;}++checks;
if(bits(got[2])!=2874213907u){std::fprintf(stderr,"record 3 jacobian[2] got %08x expected %08x\n",bits(got[2]),2874213907u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(3213195776u),f(3212836923u),f(1092616191u)}};
Vector world_axis{{f(3046637569u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213516622u),f(3212836908u),f(3172516204u)}};
Vector local_axis{{f(2979719805u),f(1065353217u),f(3047038827u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3046637570u){std::fprintf(stderr,"record 4 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),3046637570u);return 1;}++checks;
if(bits(got[1])!=1065353218u){std::fprintf(stderr,"record 4 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353218u);return 1;}++checks;
if(bits(got[2])!=2845986816u){std::fprintf(stderr,"record 4 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2845986816u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025263108u){std::fprintf(stderr,"record 4 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025263108u);return 1;}++checks;
if(bits(got[1])!=859411215u){std::fprintf(stderr,"record 4 jacobian[1] got %08x expected %08x\n",bits(got[1]),859411215u);return 1;}++checks;
if(bits(got[2])!=3213516374u){std::fprintf(stderr,"record 4 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213516374u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(3213195776u),f(3212836923u),f(1092616191u)}};
Vector world_axis{{f(1065353216u),f(899153921u),f(2147483648u)}};
Vector local_point{{f(3213516622u),f(3212836908u),f(3172516204u)}};
Vector local_axis{{f(1065353212u),f(829790539u),f(3125804140u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353217u){std::fprintf(stderr,"record 5 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353217u);return 1;}++checks;
if(bits(got[1])!=899153924u){std::fprintf(stderr,"record 5 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),899153924u);return 1;}++checks;
if(bits(got[2])!=725288855u){std::fprintf(stderr,"record 5 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),725288855u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=859411052u){std::fprintf(stderr,"record 5 jacobian[0] got %08x expected %08x\n",bits(got[0]),859411052u);return 1;}++checks;
if(bits(got[1])!=3172746755u){std::fprintf(stderr,"record 5 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172746755u);return 1;}++checks;
if(bits(got[2])!=1065353262u){std::fprintf(stderr,"record 5 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065353262u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(3213195264u),f(1065353669u),f(1092616192u)}};
Vector world_axis{{f(3046637569u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213516091u),f(1065353686u),f(3172516594u)}};
Vector local_axis{{f(2979719805u),f(1065353217u),f(3047038827u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3046637570u){std::fprintf(stderr,"record 6 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),3046637570u);return 1;}++checks;
if(bits(got[1])!=1065353218u){std::fprintf(stderr,"record 6 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353218u);return 1;}++checks;
if(bits(got[2])!=2845986816u){std::fprintf(stderr,"record 6 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2845986816u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025262852u){std::fprintf(stderr,"record 6 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025262852u);return 1;}++checks;
if(bits(got[1])!=859410911u){std::fprintf(stderr,"record 6 jacobian[1] got %08x expected %08x\n",bits(got[1]),859410911u);return 1;}++checks;
if(bits(got[2])!=3213515842u){std::fprintf(stderr,"record 6 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213515842u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(3213195264u),f(1065353669u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(899153921u),f(2147483648u)}};
Vector local_point{{f(3213516091u),f(1065353686u),f(3172516594u)}};
Vector local_axis{{f(1065353212u),f(829790539u),f(3125804140u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353217u){std::fprintf(stderr,"record 7 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353217u);return 1;}++checks;
if(bits(got[1])!=899153924u){std::fprintf(stderr,"record 7 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),899153924u);return 1;}++checks;
if(bits(got[2])!=725288855u){std::fprintf(stderr,"record 7 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),725288855u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=859411122u){std::fprintf(stderr,"record 7 jacobian[0] got %08x expected %08x\n",bits(got[0]),859411122u);return 1;}++checks;
if(bits(got[1])!=3172746499u){std::fprintf(stderr,"record 7 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172746499u);return 1;}++checks;
if(bits(got[2])!=3212837336u){std::fprintf(stderr,"record 7 jacobian[2] got %08x expected %08x\n",bits(got[2]),3212837336u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(1066319425u),f(3212837248u),f(1092616192u)}};
Vector world_axis{{f(3046637569u),f(1065353216u),f(0u)}};
Vector local_point{{f(1065998575u),f(3212837254u),f(3172975547u)}};
Vector local_axis{{f(2979719805u),f(1065353217u),f(3047038827u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3046637570u){std::fprintf(stderr,"record 8 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),3046637570u);return 1;}++checks;
if(bits(got[1])!=1065353218u){std::fprintf(stderr,"record 8 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353218u);return 1;}++checks;
if(bits(got[2])!=2845986816u){std::fprintf(stderr,"record 8 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2845986816u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025262853u){std::fprintf(stderr,"record 8 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025262853u);return 1;}++checks;
if(bits(got[1])!=859410956u){std::fprintf(stderr,"record 8 jacobian[1] got %08x expected %08x\n",bits(got[1]),859410956u);return 1;}++checks;
if(bits(got[2])!=1065998834u){std::fprintf(stderr,"record 8 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065998834u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(1066319425u),f(3212837248u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(899153921u),f(2147483648u)}};
Vector local_point{{f(1065998575u),f(3212837254u),f(3172975547u)}};
Vector local_axis{{f(1065353212u),f(829790539u),f(3125804140u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353217u){std::fprintf(stderr,"record 9 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353217u);return 1;}++checks;
if(bits(got[1])!=899153924u){std::fprintf(stderr,"record 9 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),899153924u);return 1;}++checks;
if(bits(got[2])!=725288855u){std::fprintf(stderr,"record 9 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),725288855u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=859410749u){std::fprintf(stderr,"record 9 jacobian[0] got %08x expected %08x\n",bits(got[0]),859410749u);return 1;}++checks;
if(bits(got[1])!=3172746500u){std::fprintf(stderr,"record 9 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172746500u);return 1;}++checks;
if(bits(got[2])!=1065353607u){std::fprintf(stderr,"record 9 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065353607u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(1066319679u),f(1065353216u),f(1092616192u)}};
Vector world_axis{{f(3046637569u),f(1065353216u),f(0u)}};
Vector local_point{{f(1065998848u),f(1065353208u),f(3172976186u)}};
Vector local_axis{{f(2979719805u),f(1065353217u),f(3047038827u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3046637570u){std::fprintf(stderr,"record 10 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),3046637570u);return 1;}++checks;
if(bits(got[1])!=1065353218u){std::fprintf(stderr,"record 10 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353218u);return 1;}++checks;
if(bits(got[2])!=2845986816u){std::fprintf(stderr,"record 10 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2845986816u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025262852u){std::fprintf(stderr,"record 10 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025262852u);return 1;}++checks;
if(bits(got[1])!=859410955u){std::fprintf(stderr,"record 10 jacobian[1] got %08x expected %08x\n",bits(got[1]),859410955u);return 1;}++checks;
if(bits(got[2])!=1065999108u){std::fprintf(stderr,"record 10 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065999108u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891170537u),f(3117415533u),f(890729766u),f(1065353215u)}};
Vector center{{f(1025280292u),f(3041193371u),f(1092656197u)}};
Vector world_point{{f(1066319679u),f(1065353216u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(899153921u),f(2147483648u)}};
Vector local_point{{f(1065998848u),f(1065353208u),f(3172976186u)}};
Vector local_axis{{f(1065353212u),f(829790539u),f(3125804140u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353217u){std::fprintf(stderr,"record 11 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353217u);return 1;}++checks;
if(bits(got[1])!=899153924u){std::fprintf(stderr,"record 11 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),899153924u);return 1;}++checks;
if(bits(got[2])!=725288855u){std::fprintf(stderr,"record 11 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),725288855u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=859411122u){std::fprintf(stderr,"record 11 jacobian[0] got %08x expected %08x\n",bits(got[0]),859411122u);return 1;}++checks;
if(bits(got[1])!=3172746499u){std::fprintf(stderr,"record 11 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172746499u);return 1;}++checks;
if(bits(got[2])!=3212836862u){std::fprintf(stderr,"record 11 jacobian[2] got %08x expected %08x\n",bits(got[2]),3212836862u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(3212270611u),f(3212836864u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213516379u),f(3212836828u),f(3171914576u)}};
Vector local_axis{{f(993109404u),f(899529873u),f(1065353156u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=729588634u){std::fprintf(stderr,"record 12 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),729588634u);return 1;}++checks;
if(bits(got[1])!=680656896u){std::fprintf(stderr,"record 12 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),680656896u);return 1;}++checks;
if(bits(got[2])!=1065353218u){std::fprintf(stderr,"record 12 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353218u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212836861u){std::fprintf(stderr,"record 12 jacobian[0] got %08x expected %08x\n",bits(got[0]),3212836861u);return 1;}++checks;
if(bits(got[1])!=1066031891u){std::fprintf(stderr,"record 12 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066031891u);return 1;}++checks;
if(bits(got[2])!=729265428u){std::fprintf(stderr,"record 12 jacobian[2] got %08x expected %08x\n",bits(got[2]),729265428u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(3212269824u),f(1065353582u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213515961u),f(1065353601u),f(3171915244u)}};
Vector local_axis{{f(993109404u),f(899529873u),f(1065353156u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=729588634u){std::fprintf(stderr,"record 13 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),729588634u);return 1;}++checks;
if(bits(got[1])!=680656896u){std::fprintf(stderr,"record 13 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),680656896u);return 1;}++checks;
if(bits(got[2])!=1065353218u){std::fprintf(stderr,"record 13 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353218u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065353591u){std::fprintf(stderr,"record 13 jacobian[0] got %08x expected %08x\n",bits(got[0]),1065353591u);return 1;}++checks;
if(bits(got[1])!=1066031499u){std::fprintf(stderr,"record 13 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066031499u);return 1;}++checks;
if(bits(got[2])!=2877344374u){std::fprintf(stderr,"record 13 jacobian[2] got %08x expected %08x\n",bits(got[2]),2877344374u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(1066961988u),f(3212836608u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1065999282u),f(3212836627u),f(3173484848u)}};
Vector local_axis{{f(993109404u),f(899529873u),f(1065353156u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=729588634u){std::fprintf(stderr,"record 14 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),729588634u);return 1;}++checks;
if(bits(got[1])!=680656896u){std::fprintf(stderr,"record 14 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),680656896u);return 1;}++checks;
if(bits(got[2])!=1065353218u){std::fprintf(stderr,"record 14 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353218u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212836605u){std::fprintf(stderr,"record 14 jacobian[0] got %08x expected %08x\n",bits(got[0]),3212836605u);return 1;}++checks;
if(bits(got[1])!=3213483843u){std::fprintf(stderr,"record 14 jacobian[1] got %08x expected %08x\n",bits(got[1]),3213483843u);return 1;}++checks;
if(bits(got[2])!=729859670u){std::fprintf(stderr,"record 14 jacobian[2] got %08x expected %08x\n",bits(got[2]),729859670u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(1066962600u),f(1065353856u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1065999919u),f(1065353848u),f(3173485535u)}};
Vector local_axis{{f(993109404u),f(899529873u),f(1065353156u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=729588634u){std::fprintf(stderr,"record 15 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),729588634u);return 1;}++checks;
if(bits(got[1])!=680656896u){std::fprintf(stderr,"record 15 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),680656896u);return 1;}++checks;
if(bits(got[2])!=1065353218u){std::fprintf(stderr,"record 15 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353218u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065353865u){std::fprintf(stderr,"record 15 jacobian[0] got %08x expected %08x\n",bits(got[0]),1065353865u);return 1;}++checks;
if(bits(got[1])!=3213484454u){std::fprintf(stderr,"record 15 jacobian[1] got %08x expected %08x\n",bits(got[1]),3213484454u);return 1;}++checks;
if(bits(got[2])!=2876751468u){std::fprintf(stderr,"record 15 jacobian[2] got %08x expected %08x\n",bits(got[2]),2876751468u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(3212270611u),f(3212836864u),f(1092616192u)}};
Vector world_axis{{f(3049783296u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213516379u),f(3212836828u),f(3171914576u)}};
Vector local_axis{{f(856555956u),f(1065353217u),f(3047014348u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3049783298u){std::fprintf(stderr,"record 16 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),3049783298u);return 1;}++checks;
if(bits(got[1])!=1065353218u){std::fprintf(stderr,"record 16 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353218u);return 1;}++checks;
if(bits(got[2])!=2849701888u){std::fprintf(stderr,"record 16 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2849701888u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025217795u){std::fprintf(stderr,"record 16 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025217795u);return 1;}++checks;
if(bits(got[1])!=863180982u){std::fprintf(stderr,"record 16 jacobian[1] got %08x expected %08x\n",bits(got[1]),863180982u);return 1;}++checks;
if(bits(got[2])!=3213515551u){std::fprintf(stderr,"record 16 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213515551u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(3212270611u),f(3212836864u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(902299648u),f(2147483648u)}};
Vector local_point{{f(3213516379u),f(3212836828u),f(3171914576u)}};
Vector local_axis{{f(1065353156u),f(3004936327u),f(3140593052u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353218u){std::fprintf(stderr,"record 17 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353218u);return 1;}++checks;
if(bits(got[1])!=902299651u){std::fprintf(stderr,"record 17 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),902299651u);return 1;}++checks;
if(bits(got[2])!=729531827u){std::fprintf(stderr,"record 17 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),729531827u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=863180761u){std::fprintf(stderr,"record 17 jacobian[0] got %08x expected %08x\n",bits(got[0]),863180761u);return 1;}++checks;
if(bits(got[1])!=3172701443u){std::fprintf(stderr,"record 17 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172701443u);return 1;}++checks;
if(bits(got[2])!=1065353186u){std::fprintf(stderr,"record 17 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065353186u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(3212269824u),f(1065353582u),f(1092616192u)}};
Vector world_axis{{f(3049783296u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213515961u),f(1065353601u),f(3171915244u)}};
Vector local_axis{{f(856555956u),f(1065353217u),f(3047014348u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3049783298u){std::fprintf(stderr,"record 18 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),3049783298u);return 1;}++checks;
if(bits(got[1])!=1065353218u){std::fprintf(stderr,"record 18 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353218u);return 1;}++checks;
if(bits(got[2])!=2849701888u){std::fprintf(stderr,"record 18 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2849701888u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025217797u){std::fprintf(stderr,"record 18 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025217797u);return 1;}++checks;
if(bits(got[1])!=863180985u){std::fprintf(stderr,"record 18 jacobian[1] got %08x expected %08x\n",bits(got[1]),863180985u);return 1;}++checks;
if(bits(got[2])!=3213515134u){std::fprintf(stderr,"record 18 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213515134u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(3212269824u),f(1065353582u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(902299648u),f(2147483648u)}};
Vector local_point{{f(3213515961u),f(1065353601u),f(3171915244u)}};
Vector local_axis{{f(1065353156u),f(3004936327u),f(3140593052u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353218u){std::fprintf(stderr,"record 19 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353218u);return 1;}++checks;
if(bits(got[1])!=902299651u){std::fprintf(stderr,"record 19 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),902299651u);return 1;}++checks;
if(bits(got[2])!=729531827u){std::fprintf(stderr,"record 19 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),729531827u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=863181268u){std::fprintf(stderr,"record 19 jacobian[0] got %08x expected %08x\n",bits(got[0]),863181268u);return 1;}++checks;
if(bits(got[1])!=3172701445u){std::fprintf(stderr,"record 19 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172701445u);return 1;}++checks;
if(bits(got[2])!=3212837253u){std::fprintf(stderr,"record 19 jacobian[2] got %08x expected %08x\n",bits(got[2]),3212837253u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(1066961988u),f(3212836608u),f(1092616192u)}};
Vector world_axis{{f(3049783296u),f(1065353216u),f(0u)}};
Vector local_point{{f(1065999282u),f(3212836627u),f(3173484848u)}};
Vector local_axis{{f(856555956u),f(1065353217u),f(3047014348u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3049783298u){std::fprintf(stderr,"record 20 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),3049783298u);return 1;}++checks;
if(bits(got[1])!=1065353218u){std::fprintf(stderr,"record 20 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353218u);return 1;}++checks;
if(bits(got[2])!=2849701888u){std::fprintf(stderr,"record 20 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2849701888u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025217795u){std::fprintf(stderr,"record 20 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025217795u);return 1;}++checks;
if(bits(got[1])!=863181041u){std::fprintf(stderr,"record 20 jacobian[1] got %08x expected %08x\n",bits(got[1]),863181041u);return 1;}++checks;
if(bits(got[2])!=1066000183u){std::fprintf(stderr,"record 20 jacobian[2] got %08x expected %08x\n",bits(got[2]),1066000183u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(1066961988u),f(3212836608u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(902299648u),f(2147483648u)}};
Vector local_point{{f(1065999282u),f(3212836627u),f(3173484848u)}};
Vector local_axis{{f(1065353156u),f(3004936327u),f(3140593052u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353218u){std::fprintf(stderr,"record 21 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353218u);return 1;}++checks;
if(bits(got[1])!=902299651u){std::fprintf(stderr,"record 21 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),902299651u);return 1;}++checks;
if(bits(got[2])!=729531827u){std::fprintf(stderr,"record 21 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),729531827u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=863180761u){std::fprintf(stderr,"record 21 jacobian[0] got %08x expected %08x\n",bits(got[0]),863180761u);return 1;}++checks;
if(bits(got[1])!=3172701443u){std::fprintf(stderr,"record 21 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172701443u);return 1;}++checks;
if(bits(got[2])!=1065352984u){std::fprintf(stderr,"record 21 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065352984u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(1066962600u),f(1065353856u),f(1092616192u)}};
Vector world_axis{{f(3049783296u),f(1065353216u),f(0u)}};
Vector local_point{{f(1065999919u),f(1065353848u),f(3173485535u)}};
Vector local_axis{{f(856555956u),f(1065353217u),f(3047014348u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=3049783298u){std::fprintf(stderr,"record 22 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),3049783298u);return 1;}++checks;
if(bits(got[1])!=1065353218u){std::fprintf(stderr,"record 22 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353218u);return 1;}++checks;
if(bits(got[2])!=2849701888u){std::fprintf(stderr,"record 22 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2849701888u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025217797u){std::fprintf(stderr,"record 22 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025217797u);return 1;}++checks;
if(bits(got[1])!=863181044u){std::fprintf(stderr,"record 22 jacobian[1] got %08x expected %08x\n",bits(got[1]),863181044u);return 1;}++checks;
if(bits(got[2])!=1066000819u){std::fprintf(stderr,"record 22 jacobian[2] got %08x expected %08x\n",bits(got[2]),1066000819u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(891159452u),f(3132204454u),f(894215870u),f(1065353201u)}};
Vector center{{f(1038798940u),f(3039469712u),f(1092656021u)}};
Vector world_point{{f(1066962600u),f(1065353856u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(902299648u),f(2147483648u)}};
Vector local_point{{f(1065999919u),f(1065353848u),f(3173485535u)}};
Vector local_axis{{f(1065353156u),f(3004936327u),f(3140593052u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353218u){std::fprintf(stderr,"record 23 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353218u);return 1;}++checks;
if(bits(got[1])!=902299651u){std::fprintf(stderr,"record 23 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),902299651u);return 1;}++checks;
if(bits(got[2])!=729531827u){std::fprintf(stderr,"record 23 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),729531827u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=863181268u){std::fprintf(stderr,"record 23 jacobian[0] got %08x expected %08x\n",bits(got[0]),863181268u);return 1;}++checks;
if(bits(got[1])!=3172701445u){std::fprintf(stderr,"record 23 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172701445u);return 1;}++checks;
if(bits(got[2])!=3212837500u){std::fprintf(stderr,"record 23 jacobian[2] got %08x expected %08x\n",bits(got[2]),3212837500u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140360468u),f(3212834048u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213515961u),f(3212836262u),f(3172276168u)}};
Vector local_axis{{f(986163444u),f(921747015u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2877590939u){std::fprintf(stderr,"record 24 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),2877590939u);return 1;}++checks;
if(bits(got[1])!=717109760u){std::fprintf(stderr,"record 24 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),717109760u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 24 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212836071u){std::fprintf(stderr,"record 24 jacobian[0] got %08x expected %08x\n",bits(got[0]),3212836071u);return 1;}++checks;
if(bits(got[1])!=1066031742u){std::fprintf(stderr,"record 24 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066031742u);return 1;}++checks;
if(bits(got[2])!=2880959384u){std::fprintf(stderr,"record 24 jacobian[2] got %08x expected %08x\n",bits(got[2]),2880959384u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140360468u),f(1065354496u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213515965u),f(1065353328u),f(3172280020u)}};
Vector local_axis{{f(986163444u),f(921747015u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2877590939u){std::fprintf(stderr,"record 25 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),2877590939u);return 1;}++checks;
if(bits(got[1])!=717109760u){std::fprintf(stderr,"record 25 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),717109760u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 25 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065353242u){std::fprintf(stderr,"record 25 jacobian[0] got %08x expected %08x\n",bits(got[0]),1065353242u);return 1;}++checks;
if(bits(got[1])!=1066031741u){std::fprintf(stderr,"record 25 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066031741u);return 1;}++checks;
if(bits(got[2])!=723668280u){std::fprintf(stderr,"record 25 jacobian[2] got %08x expected %08x\n",bits(got[2]),723668280u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140431183u),f(3212833792u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1065999857u),f(3212835998u),f(3173158603u)}};
Vector local_axis{{f(986163444u),f(921747015u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2877590939u){std::fprintf(stderr,"record 26 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),2877590939u);return 1;}++checks;
if(bits(got[1])!=717109760u){std::fprintf(stderr,"record 26 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),717109760u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 26 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212835817u){std::fprintf(stderr,"record 26 jacobian[0] got %08x expected %08x\n",bits(got[0]),3212835817u);return 1;}++checks;
if(bits(got[1])!=3213483902u){std::fprintf(stderr,"record 26 jacobian[1] got %08x expected %08x\n",bits(got[1]),3213483902u);return 1;}++checks;
if(bits(got[2])!=2871174188u){std::fprintf(stderr,"record 26 jacobian[2] got %08x expected %08x\n",bits(got[2]),2871174188u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140431182u),f(1065354624u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1065999597u),f(1065353461u),f(3173162442u)}};
Vector local_axis{{f(986163444u),f(921747015u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2877590939u){std::fprintf(stderr,"record 27 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),2877590939u);return 1;}++checks;
if(bits(got[1])!=717109760u){std::fprintf(stderr,"record 27 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),717109760u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 27 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065353370u){std::fprintf(stderr,"record 27 jacobian[0] got %08x expected %08x\n",bits(got[0]),1065353370u);return 1;}++checks;
if(bits(got[1])!=3213483647u){std::fprintf(stderr,"record 27 jacobian[1] got %08x expected %08x\n",bits(got[1]),3213483647u);return 1;}++checks;
if(bits(got[2])!=733464514u){std::fprintf(stderr,"record 27 jacobian[2] got %08x expected %08x\n",bits(got[2]),733464514u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140360468u),f(3212834048u),f(1092616192u)}};
Vector world_axis{{f(906493968u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213515961u),f(3212836262u),f(3172276168u)}};
Vector local_axis{{f(904660671u),f(1065353155u),f(3069236571u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=906493903u){std::fprintf(stderr,"record 28 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),906493903u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 28 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=717109248u){std::fprintf(stderr,"record 28 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),717109248u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025236331u){std::fprintf(stderr,"record 28 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025236331u);return 1;}++checks;
if(bits(got[1])!=3013974818u){std::fprintf(stderr,"record 28 jacobian[1] got %08x expected %08x\n",bits(got[1]),3013974818u);return 1;}++checks;
if(bits(got[2])!=3213515372u){std::fprintf(stderr,"record 28 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213515372u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140360468u),f(3212834048u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053977616u),f(0u)}};
Vector local_point{{f(3213515961u),f(3212836262u),f(3172276168u)}};
Vector local_axis{{f(1065353136u),f(3052240471u),f(3133647092u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 29 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3053977551u){std::fprintf(stderr,"record 29 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3053977551u);return 1;}++checks;
if(bits(got[2])!=2877594676u){std::fprintf(stderr,"record 29 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2877594676u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3013974736u){std::fprintf(stderr,"record 29 jacobian[0] got %08x expected %08x\n",bits(got[0]),3013974736u);return 1;}++checks;
if(bits(got[1])!=3172719979u){std::fprintf(stderr,"record 29 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172719979u);return 1;}++checks;
if(bits(got[2])!=1065352460u){std::fprintf(stderr,"record 29 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065352460u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140360468u),f(1065354496u),f(1092616192u)}};
Vector world_axis{{f(906493968u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213515965u),f(1065353328u),f(3172280020u)}};
Vector local_axis{{f(904660671u),f(1065353155u),f(3069236571u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=906493903u){std::fprintf(stderr,"record 30 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),906493903u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 30 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=717109248u){std::fprintf(stderr,"record 30 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),717109248u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025236332u){std::fprintf(stderr,"record 30 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025236332u);return 1;}++checks;
if(bits(got[1])!=3013974819u){std::fprintf(stderr,"record 30 jacobian[1] got %08x expected %08x\n",bits(got[1]),3013974819u);return 1;}++checks;
if(bits(got[2])!=3213515405u){std::fprintf(stderr,"record 30 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213515405u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140360468u),f(1065354496u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053977616u),f(0u)}};
Vector local_point{{f(3213515965u),f(1065353328u),f(3172280020u)}};
Vector local_axis{{f(1065353136u),f(3052240471u),f(3133647092u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 31 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3053977551u){std::fprintf(stderr,"record 31 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3053977551u);return 1;}++checks;
if(bits(got[2])!=2877594676u){std::fprintf(stderr,"record 31 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2877594676u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3013975003u){std::fprintf(stderr,"record 31 jacobian[0] got %08x expected %08x\n",bits(got[0]),3013975003u);return 1;}++checks;
if(bits(got[1])!=3172719980u){std::fprintf(stderr,"record 31 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172719980u);return 1;}++checks;
if(bits(got[2])!=3212836872u){std::fprintf(stderr,"record 31 jacobian[2] got %08x expected %08x\n",bits(got[2]),3212836872u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140431183u),f(3212833792u),f(1092616192u)}};
Vector world_axis{{f(906493968u),f(1065353216u),f(0u)}};
Vector local_point{{f(1065999857u),f(3212835998u),f(3173158603u)}};
Vector local_axis{{f(904660671u),f(1065353155u),f(3069236571u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=906493903u){std::fprintf(stderr,"record 32 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),906493903u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 32 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=717109248u){std::fprintf(stderr,"record 32 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),717109248u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025236332u){std::fprintf(stderr,"record 32 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025236332u);return 1;}++checks;
if(bits(got[1])!=3013974921u){std::fprintf(stderr,"record 32 jacobian[1] got %08x expected %08x\n",bits(got[1]),3013974921u);return 1;}++checks;
if(bits(got[2])!=1066000270u){std::fprintf(stderr,"record 32 jacobian[2] got %08x expected %08x\n",bits(got[2]),1066000270u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140431183u),f(3212833792u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053977616u),f(0u)}};
Vector local_point{{f(1065999857u),f(3212835998u),f(3173158603u)}};
Vector local_axis{{f(1065353136u),f(3052240471u),f(3133647092u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 33 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3053977551u){std::fprintf(stderr,"record 33 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3053977551u);return 1;}++checks;
if(bits(got[2])!=2877594676u){std::fprintf(stderr,"record 33 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2877594676u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3013974737u){std::fprintf(stderr,"record 33 jacobian[0] got %08x expected %08x\n",bits(got[0]),3013974737u);return 1;}++checks;
if(bits(got[1])!=3172719980u){std::fprintf(stderr,"record 33 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172719980u);return 1;}++checks;
if(bits(got[2])!=1065352132u){std::fprintf(stderr,"record 33 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065352132u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140431182u),f(1065354624u),f(1092616192u)}};
Vector world_axis{{f(906493968u),f(1065353216u),f(0u)}};
Vector local_point{{f(1065999597u),f(1065353461u),f(3173162442u)}};
Vector local_axis{{f(904660671u),f(1065353155u),f(3069236571u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=906493903u){std::fprintf(stderr,"record 34 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),906493903u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 34 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=717109248u){std::fprintf(stderr,"record 34 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),717109248u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025236332u){std::fprintf(stderr,"record 34 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025236332u);return 1;}++checks;
if(bits(got[1])!=3013974921u){std::fprintf(stderr,"record 34 jacobian[1] got %08x expected %08x\n",bits(got[1]),3013974921u);return 1;}++checks;
if(bits(got[2])!=1065999981u){std::fprintf(stderr,"record 34 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065999981u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(913357986u),f(3125258500u),f(3020748080u),f(1065353196u)}};
Vector center{{f(1140395887u),f(957196102u),f(1092656094u)}};
Vector world_point{{f(1140431182u),f(1065354624u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053977616u),f(0u)}};
Vector local_point{{f(1065999597u),f(1065353461u),f(3173162442u)}};
Vector local_axis{{f(1065353136u),f(3052240471u),f(3133647092u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 35 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3053977551u){std::fprintf(stderr,"record 35 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3053977551u);return 1;}++checks;
if(bits(got[2])!=2877594676u){std::fprintf(stderr,"record 35 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2877594676u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3013975003u){std::fprintf(stderr,"record 35 jacobian[0] got %08x expected %08x\n",bits(got[0]),3013975003u);return 1;}++checks;
if(bits(got[1])!=3172719980u){std::fprintf(stderr,"record 35 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172719980u);return 1;}++checks;
if(bits(got[2])!=3212837036u){std::fprintf(stderr,"record 35 jacobian[2] got %08x expected %08x\n",bits(got[2]),3212837036u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140417328u),f(3212834048u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213515193u),f(3212836215u),f(3172277188u)}};
Vector local_axis{{f(986137762u),f(920805250u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2872295512u){std::fprintf(stderr,"record 36 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),2872295512u);return 1;}++checks;
if(bits(got[1])!=2861118464u){std::fprintf(stderr,"record 36 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),2861118464u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 36 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212836025u){std::fprintf(stderr,"record 36 jacobian[0] got %08x expected %08x\n",bits(got[0]),3212836025u);return 1;}++checks;
if(bits(got[1])!=1066030974u){std::fprintf(stderr,"record 36 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066030974u);return 1;}++checks;
if(bits(got[2])!=2865967778u){std::fprintf(stderr,"record 36 jacobian[2] got %08x expected %08x\n",bits(got[2]),2865967778u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140417327u),f(1065354496u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213515452u),f(1065353352u),f(3172280798u)}};
Vector local_axis{{f(986137762u),f(920805250u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=2872295512u){std::fprintf(stderr,"record 37 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),2872295512u);return 1;}++checks;
if(bits(got[1])!=2861118464u){std::fprintf(stderr,"record 37 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),2861118464u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 37 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065353265u){std::fprintf(stderr,"record 37 jacobian[0] got %08x expected %08x\n",bits(got[0]),1065353265u);return 1;}++checks;
if(bits(got[1])!=1066031230u){std::fprintf(stderr,"record 37 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066031230u);return 1;}++checks;
if(bits(got[2])!=729671306u){std::fprintf(stderr,"record 37 jacobian[2] got %08x expected %08x\n",bits(got[2]),729671306u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140431183u),f(3212833792u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1065999857u),f(3212835998u),f(3173158603u)}};
Vector local_axis{{f(986163444u),f(921747015u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=910730237u){std::fprintf(stderr,"record 38 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),910730237u);return 1;}++checks;
if(bits(got[1])!=887483400u){std::fprintf(stderr,"record 38 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),887483400u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 38 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212835815u){std::fprintf(stderr,"record 38 jacobian[0] got %08x expected %08x\n",bits(got[0]),3212835815u);return 1;}++checks;
if(bits(got[1])!=3213483903u){std::fprintf(stderr,"record 38 jacobian[1] got %08x expected %08x\n",bits(got[1]),3213483903u);return 1;}++checks;
if(bits(got[2])!=912758327u){std::fprintf(stderr,"record 38 jacobian[2] got %08x expected %08x\n",bits(got[2]),912758327u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140431182u),f(1065354624u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(1065999597u),f(1065353461u),f(3173162442u)}};
Vector local_axis{{f(986163444u),f(921747015u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=910730237u){std::fprintf(stderr,"record 39 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),910730237u);return 1;}++checks;
if(bits(got[1])!=887483400u){std::fprintf(stderr,"record 39 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),887483400u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 39 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065353371u){std::fprintf(stderr,"record 39 jacobian[0] got %08x expected %08x\n",bits(got[0]),1065353371u);return 1;}++checks;
if(bits(got[1])!=3213483646u){std::fprintf(stderr,"record 39 jacobian[1] got %08x expected %08x\n",bits(got[1]),3213483646u);return 1;}++checks;
if(bits(got[2])!=3056185465u){std::fprintf(stderr,"record 39 jacobian[2] got %08x expected %08x\n",bits(got[2]),3056185465u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140417328u),f(3212834048u),f(1092616192u)}};
Vector world_axis{{f(905969679u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213515193u),f(3212836215u),f(3172277188u)}};
Vector local_axis{{f(904259227u),f(1065353155u),f(3068294643u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=905969572u){std::fprintf(stderr,"record 40 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),905969572u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 40 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=2873399808u){std::fprintf(stderr,"record 40 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2873399808u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025236331u){std::fprintf(stderr,"record 40 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025236331u);return 1;}++checks;
if(bits(got[1])!=3013336551u){std::fprintf(stderr,"record 40 jacobian[1] got %08x expected %08x\n",bits(got[1]),3013336551u);return 1;}++checks;
if(bits(got[2])!=3213514605u){std::fprintf(stderr,"record 40 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213514605u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140417328u),f(3212834048u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053453327u),f(0u)}};
Vector local_point{{f(3213515193u),f(3212836215u),f(3172277188u)}};
Vector local_axis{{f(1065353136u),f(3051833110u),f(3133621410u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 41 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3053453220u){std::fprintf(stderr,"record 41 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3053453220u);return 1;}++checks;
if(bits(got[2])!=2872300575u){std::fprintf(stderr,"record 41 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2872300575u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3013336355u){std::fprintf(stderr,"record 41 jacobian[0] got %08x expected %08x\n",bits(got[0]),3013336355u);return 1;}++checks;
if(bits(got[1])!=3172719979u){std::fprintf(stderr,"record 41 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172719979u);return 1;}++checks;
if(bits(got[2])!=1065352412u){std::fprintf(stderr,"record 41 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065352412u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140417327u),f(1065354496u),f(1092616192u)}};
Vector world_axis{{f(905969679u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213515452u),f(1065353352u),f(3172280798u)}};
Vector local_axis{{f(904259227u),f(1065353155u),f(3068294643u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=905969572u){std::fprintf(stderr,"record 42 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),905969572u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 42 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=2873399808u){std::fprintf(stderr,"record 42 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2873399808u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025236332u){std::fprintf(stderr,"record 42 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025236332u);return 1;}++checks;
if(bits(got[1])!=3013336552u){std::fprintf(stderr,"record 42 jacobian[1] got %08x expected %08x\n",bits(got[1]),3013336552u);return 1;}++checks;
if(bits(got[2])!=3213514893u){std::fprintf(stderr,"record 42 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213514893u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140417327u),f(1065354496u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053453327u),f(0u)}};
Vector local_point{{f(3213515452u),f(1065353352u),f(3172280798u)}};
Vector local_axis{{f(1065353136u),f(3051833110u),f(3133621410u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 43 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3053453220u){std::fprintf(stderr,"record 43 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3053453220u);return 1;}++checks;
if(bits(got[2])!=2872300575u){std::fprintf(stderr,"record 43 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),2872300575u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3013336536u){std::fprintf(stderr,"record 43 jacobian[0] got %08x expected %08x\n",bits(got[0]),3013336536u);return 1;}++checks;
if(bits(got[1])!=3172719980u){std::fprintf(stderr,"record 43 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172719980u);return 1;}++checks;
if(bits(got[2])!=3212836896u){std::fprintf(stderr,"record 43 jacobian[2] got %08x expected %08x\n",bits(got[2]),3212836896u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140431183u),f(3212833792u),f(1092616192u)}};
Vector world_axis{{f(906493968u),f(1065353216u),f(0u)}};
Vector local_point{{f(1065999857u),f(3212835998u),f(3173158603u)}};
Vector local_axis{{f(904660671u),f(1065353155u),f(3069236571u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=906173204u){std::fprintf(stderr,"record 44 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),906173204u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 44 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=3034967241u){std::fprintf(stderr,"record 44 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),3034967241u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025237196u){std::fprintf(stderr,"record 44 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025237196u);return 1;}++checks;
if(bits(got[1])!=886030113u){std::fprintf(stderr,"record 44 jacobian[1] got %08x expected %08x\n",bits(got[1]),886030113u);return 1;}++checks;
if(bits(got[2])!=1066000269u){std::fprintf(stderr,"record 44 jacobian[2] got %08x expected %08x\n",bits(got[2]),1066000269u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140431183u),f(3212833792u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053977616u),f(0u)}};
Vector local_point{{f(1065999857u),f(3212835998u),f(3173158603u)}};
Vector local_axis{{f(1065353136u),f(3052240471u),f(3133647092u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 45 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3053656857u){std::fprintf(stderr,"record 45 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3053656857u);return 1;}++checks;
if(bits(got[2])!=3058213891u){std::fprintf(stderr,"record 45 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),3058213891u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=910402533u){std::fprintf(stderr,"record 45 jacobian[0] got %08x expected %08x\n",bits(got[0]),910402533u);return 1;}++checks;
if(bits(got[1])!=3172719865u){std::fprintf(stderr,"record 45 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172719865u);return 1;}++checks;
if(bits(got[2])!=1065352132u){std::fprintf(stderr,"record 45 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065352132u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140431182u),f(1065354624u),f(1092616192u)}};
Vector world_axis{{f(906493968u),f(1065353216u),f(0u)}};
Vector local_point{{f(1065999597u),f(1065353461u),f(3173162442u)}};
Vector local_axis{{f(904660671u),f(1065353155u),f(3069236571u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=906173204u){std::fprintf(stderr,"record 46 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),906173204u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 46 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=3034967241u){std::fprintf(stderr,"record 46 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),3034967241u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025237196u){std::fprintf(stderr,"record 46 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025237196u);return 1;}++checks;
if(bits(got[1])!=886029592u){std::fprintf(stderr,"record 46 jacobian[1] got %08x expected %08x\n",bits(got[1]),886029592u);return 1;}++checks;
if(bits(got[2])!=1065999980u){std::fprintf(stderr,"record 46 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065999980u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(912416344u),f(3125232818u),f(3016443913u),f(1065353196u)}};
Vector center{{f(1140452744u),f(957006190u),f(1092656094u)}};
Vector world_point{{f(1140431182u),f(1065354624u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053977616u),f(0u)}};
Vector local_point{{f(1065999597u),f(1065353461u),f(3173162442u)}};
Vector local_axis{{f(1065353136u),f(3052240471u),f(3133647092u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 47 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3053656857u){std::fprintf(stderr,"record 47 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3053656857u);return 1;}++checks;
if(bits(got[2])!=3058213891u){std::fprintf(stderr,"record 47 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),3058213891u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3058541221u){std::fprintf(stderr,"record 47 jacobian[0] got %08x expected %08x\n",bits(got[0]),3058541221u);return 1;}++checks;
if(bits(got[1])!=3172720095u){std::fprintf(stderr,"record 47 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172720095u);return 1;}++checks;
if(bits(got[2])!=3212837037u){std::fprintf(stderr,"record 47 jacobian[2] got %08x expected %08x\n",bits(got[2]),3212837037u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(911435098u),f(3125205430u),f(3010382024u),f(1065353196u)}};
Vector center{{f(1140509666u),f(956814451u),f(1092656094u)}};
Vector world_point{{f(1140417328u),f(3212834048u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213515193u),f(3212836215u),f(3172277188u)}};
Vector local_axis{{f(986137762u),f(920805250u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=911603710u){std::fprintf(stderr,"record 48 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),911603710u);return 1;}++checks;
if(bits(got[1])!=888117161u){std::fprintf(stderr,"record 48 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),888117161u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 48 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3212836027u){std::fprintf(stderr,"record 48 jacobian[0] got %08x expected %08x\n",bits(got[0]),3212836027u);return 1;}++checks;
if(bits(got[1])!=1066030974u){std::fprintf(stderr,"record 48 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066030974u);return 1;}++checks;
if(bits(got[2])!=909481776u){std::fprintf(stderr,"record 48 jacobian[2] got %08x expected %08x\n",bits(got[2]),909481776u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(911435098u),f(3125205430u),f(3010382024u),f(1065353196u)}};
Vector center{{f(1140509666u),f(956814451u),f(1092656094u)}};
Vector world_point{{f(1140417327u),f(1065354496u),f(1092616192u)}};
Vector world_axis{{f(0u),f(0u),f(1065353216u)}};
Vector local_point{{f(3213515452u),f(1065353352u),f(3172280798u)}};
Vector local_axis{{f(986137762u),f(920805250u),f(1065353136u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=911603710u){std::fprintf(stderr,"record 49 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),911603710u);return 1;}++checks;
if(bits(got[1])!=888117161u){std::fprintf(stderr,"record 49 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),888117161u);return 1;}++checks;
if(bits(got[2])!=1065353095u){std::fprintf(stderr,"record 49 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),1065353095u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1065353264u){std::fprintf(stderr,"record 49 jacobian[0] got %08x expected %08x\n",bits(got[0]),1065353264u);return 1;}++checks;
if(bits(got[1])!=1066031231u){std::fprintf(stderr,"record 49 jacobian[1] got %08x expected %08x\n",bits(got[1]),1066031231u);return 1;}++checks;
if(bits(got[2])!=3061208936u){std::fprintf(stderr,"record 49 jacobian[2] got %08x expected %08x\n",bits(got[2]),3061208936u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(911435098u),f(3125205430u),f(3010382024u),f(1065353196u)}};
Vector center{{f(1140509666u),f(956814451u),f(1092656094u)}};
Vector world_point{{f(1140417328u),f(3212834048u),f(1092616192u)}};
Vector world_axis{{f(905969679u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213515193u),f(3212836215u),f(3172277188u)}};
Vector local_axis{{f(904259227u),f(1065353155u),f(3068294643u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=905285238u){std::fprintf(stderr,"record 50 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),905285238u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 50 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=3035601048u){std::fprintf(stderr,"record 50 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),3035601048u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025235406u){std::fprintf(stderr,"record 50 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025235406u);return 1;}++checks;
if(bits(got[1])!=3037997602u){std::fprintf(stderr,"record 50 jacobian[1] got %08x expected %08x\n",bits(got[1]),3037997602u);return 1;}++checks;
if(bits(got[2])!=3213514607u){std::fprintf(stderr,"record 50 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213514607u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(911435098u),f(3125205430u),f(3010382024u),f(1065353196u)}};
Vector center{{f(1140509666u),f(956814451u),f(1092656094u)}};
Vector world_point{{f(1140417328u),f(3212834048u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053453327u),f(0u)}};
Vector local_point{{f(3213515193u),f(3212836215u),f(3172277188u)}};
Vector local_axis{{f(1065353136u),f(3051833110u),f(3133621410u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 51 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3052768899u){std::fprintf(stderr,"record 51 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3052768899u);return 1;}++checks;
if(bits(got[2])!=3059087362u){std::fprintf(stderr,"record 51 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),3059087362u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=911296956u){std::fprintf(stderr,"record 51 jacobian[0] got %08x expected %08x\n",bits(got[0]),911296956u);return 1;}++checks;
if(bits(got[1])!=3172719859u){std::fprintf(stderr,"record 51 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172719859u);return 1;}++checks;
if(bits(got[2])!=1065352412u){std::fprintf(stderr,"record 51 jacobian[2] got %08x expected %08x\n",bits(got[2]),1065352412u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(911435098u),f(3125205430u),f(3010382024u),f(1065353196u)}};
Vector center{{f(1140509666u),f(956814451u),f(1092656094u)}};
Vector world_point{{f(1140417327u),f(1065354496u),f(1092616192u)}};
Vector world_axis{{f(905969679u),f(1065353216u),f(0u)}};
Vector local_point{{f(3213515452u),f(1065353352u),f(3172280798u)}};
Vector local_axis{{f(904259227u),f(1065353155u),f(3068294643u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=905285238u){std::fprintf(stderr,"record 52 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),905285238u);return 1;}++checks;
if(bits(got[1])!=1065353094u){std::fprintf(stderr,"record 52 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),1065353094u);return 1;}++checks;
if(bits(got[2])!=3035601048u){std::fprintf(stderr,"record 52 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),3035601048u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=1025235407u){std::fprintf(stderr,"record 52 jacobian[0] got %08x expected %08x\n",bits(got[0]),1025235407u);return 1;}++checks;
if(bits(got[1])!=3037997872u){std::fprintf(stderr,"record 52 jacobian[1] got %08x expected %08x\n",bits(got[1]),3037997872u);return 1;}++checks;
if(bits(got[2])!=3213514894u){std::fprintf(stderr,"record 52 jacobian[2] got %08x expected %08x\n",bits(got[2]),3213514894u);return 1;}++checks;
}
}
{
Quaternion quaternion{{f(911435098u),f(3125205430u),f(3010382024u),f(1065353196u)}};
Vector center{{f(1140509666u),f(956814451u),f(1092656094u)}};
Vector world_point{{f(1140417327u),f(1065354496u),f(1092616192u)}};
Vector world_axis{{f(1065353216u),f(3053453327u),f(0u)}};
Vector local_point{{f(3213515452u),f(1065353352u),f(3172280798u)}};
Vector local_axis{{f(1065353136u),f(3051833110u),f(3133621410u)}};
{auto got=forward(quaternion,local_axis,true);
if(bits(got[0])!=1065353095u){std::fprintf(stderr,"record 53 prepared_axis[0] got %08x expected %08x\n",bits(got[0]),1065353095u);return 1;}++checks;
if(bits(got[1])!=3052768899u){std::fprintf(stderr,"record 53 prepared_axis[1] got %08x expected %08x\n",bits(got[1]),3052768899u);return 1;}++checks;
if(bits(got[2])!=3059087362u){std::fprintf(stderr,"record 53 prepared_axis[2] got %08x expected %08x\n",bits(got[2]),3059087362u);return 1;}++checks;
}
{auto got=cross(forward(quaternion,local_point,false),forward(quaternion,local_axis,true));
if(bits(got[0])!=3059393712u){std::fprintf(stderr,"record 53 jacobian[0] got %08x expected %08x\n",bits(got[0]),3059393712u);return 1;}++checks;
if(bits(got[1])!=3172720100u){std::fprintf(stderr,"record 53 jacobian[1] got %08x expected %08x\n",bits(got[1]),3172720100u);return 1;}++checks;
if(bits(got[2])!=3212836895u){std::fprintf(stderr,"record 53 jacobian[2] got %08x expected %08x\n",bits(got[2]),3212836895u);return 1;}++checks;
}
}
std::printf("%u original wheel frame float32 words exact\n",checks);}
