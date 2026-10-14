#include "DerivedContactErrorFrame.h"
#include <cstdio>
#include <cstring>
namespace cf=motorrock::contactframe;namespace de=motorrock::derivederror;
float rd(){unsigned w;if(std::scanf("%u",&w)!=1)throw 1;float f;std::memcpy(&f,&w,4);return f;}
unsigned word(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
int main(){try{unsigned count;if(std::scanf("%u",&count)!=1)return 2;for(unsigned record=0;record<count;++record){cf::Quaternion q;cf::Vector center,offset;for(unsigned i=0;i<4;++i)q[i]=rd();for(unsigned i=0;i<3;++i)center[i]=rd();for(unsigned i=0;i<3;++i)offset[i]=rd();
 const unsigned orders[6][3]={{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
 for(unsigned mode=0;mode<6;++mode){const auto* o=orders[mode];const auto& v=offset;
  const float dot=de::add(de::add(de::mul(q[o[0]],v[o[0]]),de::mul(q[o[1]],v[o[1]])),de::mul(q[o[2]],v[o[2]]));
  const cf::Vector cross={{de::sub(de::mul(q[1],v[2]),de::mul(q[2],v[1])),de::sub(de::mul(q[2],v[0]),de::mul(q[0],v[2])),de::sub(de::mul(q[0],v[1]),de::mul(q[1],v[0]))}};
  const float scale=de::sub(de::mul(q[3],q[3]),.5F);
  for(unsigned i=0;i<3;++i){const auto result=de::add(center[i],de::mul(de::add(de::add(de::mul(scale,v[i]),de::mul(q[3],cross[i])),de::mul(q[i],dot)),2));std::printf("%u%c",word(result),i==2?'\n':' ');}
 }
 }return 0;
}catch(int){return 2;}}
