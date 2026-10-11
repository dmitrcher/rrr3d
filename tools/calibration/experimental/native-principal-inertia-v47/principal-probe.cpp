#include "NativePrincipalInertia.h"
#include <cstdio>
int main() {
    for(;;) {
        float input[3][3];int n=0;
        for(int i=0;i<3;++i)for(int j=0;j<3;++j)if(std::scanf("%f",&input[i][j])==1)++n;
        if(!n)return 0;if(n!=9)return 2;
        r3d::physics::nativeprincipal::Result result;
        if(!r3d::physics::nativeprincipal::decompose(input,result))return 3;
        std::printf("%d",result.sweeps);
        for(float x:result.principal)std::printf(" %.9g",x);
        for(auto &row:result.axes)for(float x:row)std::printf(" %.9g",x);
        std::printf("\n");
    }
}
