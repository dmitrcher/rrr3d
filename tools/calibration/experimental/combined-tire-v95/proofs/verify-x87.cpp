#include <cstdio>
#include <cstring>
#include <xmmintrin.h>
static const unsigned fixtures[][4] = {
#include "combined-fixtures.inc"
};
static float f(unsigned w) { float x; std::memcpy(&x,&w,4); return x; }
static unsigned bits(float x) { unsigned w; std::memcpy(&w,&x,4); return w; }
int main() {
    unsigned short saved, control;
    __asm fnstcw saved
    control = (saved & ~0x0300) | 0x0300;
    __asm fldcw control
    for(unsigned i=0;i<sizeof(fixtures)/sizeof(fixtures[0]);++i) {
        float longitudinal=f(fixtures[i][0]), lateral=f(fixtures[i][1]);
        __m128 x=_mm_div_ss(_mm_set_ss(longitudinal),_mm_set_ss(f(fixtures[i][2])));
        __m128 y=_mm_div_ss(_mm_set_ss(lateral),_mm_set_ss(f(fixtures[i][3])));
        float sigma=_mm_cvtss_f32(_mm_add_ss(_mm_mul_ss(y,y),_mm_mul_ss(x,x)));
        float outLongitudinal=longitudinal, outLateral=lateral;
        if(sigma>1.0f) {
            __asm {
                fld1
                fld sigma
                fsqrt
                fdivp st(1), st(0)
                fld longitudinal
                fmul st(0), st(1)
                fstp outLongitudinal
                fmul lateral
                fstp outLateral
            }
        }
        std::printf("%u %u %u\n",bits(outLongitudinal),bits(outLateral),bits(sigma));
    }
    __asm fldcw saved
    return 0;
}
