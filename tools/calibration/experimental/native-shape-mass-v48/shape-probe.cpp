#include "NativeShapeMass.h"
#include <cstdio>
namespace n = r3d::physics::nativeshapemass;
int main()
{
    int kind; float dimensions[3], position[3];
    while (std::scanf("%d %f %f %f %f %f %f", &kind, &dimensions[0],
        &dimensions[1], &dimensions[2], &position[0], &position[1], &position[2]) == 7)
    {
        const auto value = kind == 0 ? n::box(dimensions, position)
            : n::wheel(dimensions[0], dimensions[1], position);
        for (auto& row : value.tensor) for (float x : row) std::printf("%.9g ", x);
        for (float x : value.center) std::printf("%.9g ", x);
        std::printf("%.9g\n", value.mass);
    }
}
