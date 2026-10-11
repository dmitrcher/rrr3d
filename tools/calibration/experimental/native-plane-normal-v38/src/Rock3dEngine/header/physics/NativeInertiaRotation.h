#pragma once
// Experimental native global inverse-inertia transform. Keep intermediate
// products extended and store each output once; the tensor is symmetric.
// Inputs are the current independently calculated actor pose and local tensor.
#include "ContactFrameScalar.h"
namespace motorrock { namespace inertiav37 {
namespace cf=contactframe;
inline cf::Matrix rotate(const cf::Matrix& rotation,const cf::Matrix& local) {
 cf::Matrix result;
 for(int i=0;i<3;++i)for(int j=i;j<3;++j) {
  double sum=0;
  for(int k=0;k<3;++k)for(int l=0;l<3;++l)
   sum+=double(rotation.a[i][k])*local.a[k][l]*rotation.a[j][l];
  result.a[i][j]=cf::store(sum);result.a[j][i]=result.a[i][j];
 }
 return result;
}
}}
