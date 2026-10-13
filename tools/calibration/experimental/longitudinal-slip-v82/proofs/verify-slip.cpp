#include "NativeLongitudinalSlipV81.h"
#include <cstring>
#include <iostream>
float f(unsigned v){float x;std::memcpy(&x,&v,4);return x;}
unsigned bits(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
int main(){unsigned count;std::cin>>count;while(count--){unsigned a,b,c;std::cin>>a>>b>>c;auto r=motorrock::longitudinalslipv81::calculate(f(a),f(b),f(c));std::cout<<bits(r.negativeRolling)<<' '<<bits(r.rawSlip)<<' '<<bits(r.normalizedSlip)<<'\n';}}
