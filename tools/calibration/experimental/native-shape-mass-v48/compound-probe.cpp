#include "NativeShapeMass.h"
#include "NativePrincipalInertia.h"
#include <cstdio>
namespace s = r3d::physics::nativeshapemass;
int main()
{
    unsigned count; float mass;
    while (std::scanf("%u %f", &count, &mass) == 2)
    {
        s::Integrals combined;
        for (unsigned i = 0; i < count; ++i)
        {
            int kind; float d[3], p[3];
            if (std::scanf("%d %f %f %f %f %f %f", &kind, &d[0], &d[1], &d[2],
                &p[0], &p[1], &p[2]) != 7) return 2;
            s::accumulate(combined, kind == 0 ? s::box(d, p) : s::wheel(d[0], d[1], p));
        }
        s::scaleToMass(combined, mass);
        r3d::physics::nativeprincipal::Result result;
        if (!r3d::physics::nativeprincipal::decompose(combined.tensor, result)) return 3;
        for (auto& row : combined.tensor) for (float x : row) std::printf("%.9g ", x);
        for (float x : result.principal) std::printf("%.9g ", x);
        for (auto& row : result.axes) for (float x : row) std::printf("%.9g ", x);
        std::puts("");
    }
}
