#pragma once
// Research reconstruction of original BaseSceneNode SetDir/SetUp and D3DX9_43.
// Uses the VM's independently audited x86 reciprocal-square-root emulation.
// Parent-free, nonzero horizontal startup directions only. No reference words or per-track branches.
#include "SourceMaterialFrameCandidate.h"
namespace motorrock { namespace startupv129 {
namespace cf=contactframe;
inline float f(double x){return cf::store(x);}
inline float add(float a,float b){return f(double(a)+b);}
inline float sub(float a,float b){return f(double(a)-b);}
inline float mul(float a,float b){return f(double(a)*b);}
inline cf::Vector crossOriginal(cf::Vector a,cf::Vector b){return {{sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))}};}
inline cf::Vector normalize(cf::Vector a){float s=add(add(mul(a[0],a[0]),mul(a[1],a[1])),mul(a[2],a[2]));if(s==0)return {{0,0,0}};float r=materialcandidate::inverseSquareRootEstimate(s);float refined=mul(sub(3,mul(mul(r,s),r)),mul(r,.5f));for(int i=0;i<3;++i)a[i]=mul(a[i],refined);return a;}
struct Result {cf::Vector direction,up;float matrix[16];cf::Quaternion quaternion;};
inline Result computeHorizontalStartup(cf::Vector value){Result o{};auto dir=normalize(value);auto up=cf::Vector{{0,0,1}};auto right=normalize(crossOriginal(up,dir));up=crossOriginal(dir,right);up=normalize({{0,0,1}});right=normalize(crossOriginal(up,dir));dir=crossOriginal(right,up);o.direction=dir;o.up=up;right=crossOriginal(up,dir);for(int i=0;i<3;++i){o.matrix[i]=dir[i];o.matrix[4+i]=right[i];o.matrix[8+i]=up[i];}o.matrix[15]=1;
const float *m=o.matrix;float trace=add(add(m[0],m[5]),m[10]);auto&q=o.quaternion;
if(trace>0){float root=f(std::sqrt(double(add(trace,1))));q[3]=mul(root,.5f);float scale=f(.25/double(q[3]));q[0]=mul(sub(m[6],m[9]),scale);q[1]=mul(sub(m[8],m[2]),scale);q[2]=mul(sub(m[1],m[4]),scale);}else{float d[]={sub(sub(m[0],m[5]),m[10]),sub(sub(m[5],m[0]),m[10]),sub(m[10],add(m[0],m[5]))};int i=d[1]>d[0]?1:0;if(d[2]>d[i])i=2;int j=(i+1)%3,k=(j+1)%3;float root=f(std::sqrt(double(add(d[i],1))));q[i]=mul(root,.5f);float scale=f(.25/double(q[i]));q[j]=mul(add(m[4*j+i],m[4*i+j]),scale);q[k]=mul(add(m[4*k+i],m[4*i+k]),scale);q[3]=mul(sub(m[4*j+k],m[4*k+j]),scale);}return o;}
}}
