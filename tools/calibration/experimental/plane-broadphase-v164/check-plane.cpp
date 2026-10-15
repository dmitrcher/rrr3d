#include "physics/NativePlaneBoundsV164.h"
#include <iostream>
#include <cstring>
int main(){for(const auto& row:motorrock::planeboundsv164::deathPlane())for(float f:row){unsigned u;std::memcpy(&u,&f,4);std::cout<<u<<" ";}}
