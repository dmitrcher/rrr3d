#include <arm_neon.h>
#include <iostream>
#include <iomanip>
#include <cmath>
static float test(float x,int mode) {
 float32x2_t a=vdup_n_f32(x),r=vrsqrte_f32(a);
 if(mode==0)return vget_lane_f32(r,0);
 for(int iteration=0;iteration<(mode==4?3:mode==1?1:2);++iteration) {
  if(mode==2||mode==1||mode==4)r=vmul_f32(r,vrsqrts_f32(vmul_f32(a,r),r));
  if(mode==3)r=vmul_f32(r,vrsqrts_f32(a,vmul_f32(r,r)));
  if(mode==5)r=vmul_f32(vmul_f32(vdup_n_f32(.5f),r),vsub_f32(vdup_n_f32(3.f),vmul_f32(a,vmul_f32(r,r))));
  if(mode==6)r=vsub_f32(vmul_f32(vdup_n_f32(1.5f),r),vmul_f32(vmul_f32(vmul_f32(vdup_n_f32(.5f),a),r),vmul_f32(r,r)));
  if(mode==7)r=vmul_f32(vdup_n_f32(.5f),vmul_f32(r,vsub_f32(vdup_n_f32(3.f),vmul_f32(vmul_f32(a,r),r))));
 }
 return vget_lane_f32(r,0);
}
int main() {float x;std::cout<<std::setprecision(9);while(std::cin>>x){std::cout<<x;for(int mode=0;mode<8;++mode)std::cout<<' '<<test(x,mode);std::cout<<'\n';}}
