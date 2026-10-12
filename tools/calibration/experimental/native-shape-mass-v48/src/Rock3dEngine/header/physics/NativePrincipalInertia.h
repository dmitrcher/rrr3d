#pragma once
#include <cmath>
namespace r3d::physics::nativeprincipal {
// Original PC53 double Jacobi path, observed at PhysXCore RVAs 98220/97680.
// Explicit stores prevent contraction or replacement with float operations.
inline double add(double a,double b) noexcept { volatile double x=a+b;return x; }
inline double sub(double a,double b) noexcept { volatile double x=a-b;return x; }
inline double mul(double a,double b) noexcept { volatile double x=a*b;return x; }
inline double div(double a,double b) noexcept { volatile double x=a/b;return x; }
inline double root(double a) noexcept { volatile double x=std::sqrt(a);return x; }
struct Result { float principal[3]{};float axes[3][3]{};int sweeps=0; };
inline bool decompose(const float input[3][3],Result &out) noexcept {
    double a[3][3],v[3][3];
    for(int i=0;i<3;++i)for(int j=0;j<3;++j){a[i][j]=input[i][j];v[i][j]=i==j?1:0;}
    double diagonal=0,off=0;
    for(int i=0;i<3;++i){diagonal=add(diagonal,mul(a[i][i],a[i][i]));for(int j=i+1;j<3;++j)off=add(off,mul(a[i][j],a[i][j]));}
    const double threshold=mul(add(mul(diagonal,.5),off),9.999999949504854e-13);
    for(int sweep=0;sweep<100;++sweep) {
        off=0;for(int i=0;i<3;++i)for(int j=i+1;j<3;++j)off=add(off,mul(a[i][j],a[i][j]));
        if(threshold>off) {
            for(int i=0;i<3;++i){out.principal[i]=float(a[i][i]);for(int j=0;j<3;++j)out.axes[i][j]=float(v[i][j]);}
            out.sweeps=sweep;return true;
        }
        for(int p=0;p<2;++p)for(int q=p+1;q<3;++q) {
            if(std::abs(a[p][q])<9.999999682655225e-21)continue;
            const double theta=div(sub(a[q][q],a[p][p]),add(a[p][q],a[p][q]));
            const double magnitude=root(add(mul(theta,theta),1));
            const double t=div(1,theta>=0?add(theta,magnitude):sub(theta,magnitude));
            const double c=div(1,root(add(mul(t,t),1))),s=mul(c,t),delta=mul(t,a[p][q]);
            a[p][p]=sub(a[p][p],delta);a[q][q]=add(a[q][q],delta);a[p][q]=a[q][p]=0;
            for(int k=0;k<3;++k)if(k!=p&&k!=q) {
                const double g=a[k][p],h=a[k][q];
                a[k][p]=a[p][k]=sub(mul(g,c),mul(h,s));
                a[k][q]=a[q][k]=add(mul(g,s),mul(h,c));
            }
            for(int k=0;k<3;++k) {
                const double g=v[k][p],h=v[k][q];
                v[k][p]=sub(mul(g,c),mul(h,s));v[k][q]=add(mul(g,s),mul(h,c));
            }
        }
    }
    return false;
}
}
