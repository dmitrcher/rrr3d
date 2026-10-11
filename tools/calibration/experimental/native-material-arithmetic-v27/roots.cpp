#include <iostream>
#include <iomanip>
#include <cmath>
#include <float.h>
#include <xmmintrin.h>
static float stored(double v) { volatile float f = (float)v; return f; }
static float root(float x, int mode) {
 if(mode == 0) return stored(std::sqrt((double)x));
 __m128 a=_mm_set_ss(x),r=_mm_rsqrt_ss(a);
 if(mode == 1) return _mm_cvtss_f32(_mm_sqrt_ss(a));
 if(mode == 2) return _mm_cvtss_f32(_mm_mul_ss(a,r));
 if(mode == 6) return _mm_cvtss_f32(_mm_div_ss(_mm_set_ss(1.f),r));
 if(mode == 3) r=_mm_mul_ss(_mm_mul_ss(_mm_set_ss(.5f),r),_mm_sub_ss(_mm_set_ss(3.f),_mm_mul_ss(a,_mm_mul_ss(r,r))));
 if(mode == 4) r=_mm_sub_ss(_mm_mul_ss(_mm_set_ss(1.5f),r),_mm_mul_ss(_mm_mul_ss(_mm_mul_ss(_mm_set_ss(.5f),a),r),_mm_mul_ss(r,r)));
 if(mode == 5) r=_mm_mul_ss(_mm_set_ss(.5f),_mm_mul_ss(r,_mm_sub_ss(_mm_set_ss(3.f),_mm_mul_ss(_mm_mul_ss(a,r),r))));
 if(mode == 7) {
  r=_mm_mul_ss(_mm_mul_ss(_mm_set_ss(.5f),r),_mm_sub_ss(_mm_set_ss(3.f),_mm_mul_ss(a,_mm_mul_ss(r,r))));
  return _mm_cvtss_f32(_mm_div_ss(_mm_set_ss(1.f),r));
 }
 return _mm_cvtss_f32(_mm_mul_ss(a,r));
}
int main() {
 std::cout<<std::setprecision(9);float x;
 while(std::cin>>x) {
  std::cout<<x;
  for(int mode=0;mode<8;++mode) {
   const float s=root(x,mode),half=stored(.5*s),inv=stored(.5/s);
   std::cout<<' '<<s<<' '<<half<<' '<<inv;
  }
  const float estimated=_mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x)));
  const float singleRoot=stored(std::sqrt((double)x));
  const float scalarInverse=stored(1.0/singleRoot);
  std::cout<<' '<<estimated<<' '<<scalarInverse<<'\n';
 }
}
