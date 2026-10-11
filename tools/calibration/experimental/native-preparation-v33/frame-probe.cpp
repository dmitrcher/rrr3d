#include <cstdio>
#include "DerivedContactErrorFrame.h"
int main(){namespace cf=motorrock::contactframe;namespace d=motorrock::derivederror;cf::Matrix actor={{{1,0,0},{0,1,0},{0,0,1}}};cf::Quaternion q={{0,0,0,1}};cf::Vector normal={{0,0,1}};auto frames=d::frames(actor,q,normal);float e;
while(scanf("%f",&e)==1){cf::Vector input={{e,0,0}},world=d::rotate(frames.raw,input,false),output=d::rotate(frames.material,world,true);printf("{\"raw\":[%.9g,%.9g,%.9g,%.9g],\"material\":[%.9g,%.9g,%.9g,%.9g],\"global\":[%.9g,%.9g,%.9g],\"error\":[%.9g,%.9g,%.9g]}\n",frames.raw[0],frames.raw[1],frames.raw[2],frames.raw[3],frames.material[0],frames.material[1],frames.material[2],frames.material[3],world[0],world[1],world[2],output[0],output[1],output[2]);}}
