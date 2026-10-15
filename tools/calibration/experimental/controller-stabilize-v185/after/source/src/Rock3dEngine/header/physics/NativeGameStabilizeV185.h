#pragma once
#include "NativeGameAngularV113.h"
// Measured original GameCar arithmetic under PC24 in Windows/Parallels.
// Products round to float, while the register sums retain double precision.
// Keep separate from the PC53 standalone SDK fixture path in V113.
namespace motorrock::stabilizev185 {
namespace G = gameangularv113;
namespace P = nativepublicangular;
namespace C = contactframe;
using Matrix = P::Matrix;
using Vector = G::Vector;
using Quaternion = G::Quaternion;
inline Matrix inverse(const Matrix &m) {
    Matrix r{};
    for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < 3; ++j) {
            unsigned a = (j + 1) % 3, b = (j + 2) % 3, c = (i + 1) % 3, d = (i + 2) % 3;
            r[i][j] = P::sub(P::mul(m[a][c], m[b][d]), P::mul(m[a][d], m[b][c]));
        }
    float det = motorrock::contactframe::store(
        double(P::mul(r[0][0], m[0][0])) + P::mul(r[0][1], m[1][0]) + P::mul(r[0][2], m[2][0]));
    volatile float scale = 1.f / det;
    for (auto &row : r)
        for (auto &v : row)
            v = P::mul(v, scale);
    return r;
}
inline Vector product(const Matrix &m, const Vector &v) {
    Vector r{};
    for (unsigned i = 0; i < 3; ++i)
        r[i] = motorrock::contactframe::store(double(P::mul(m[i][0], v[0])) +
                                              P::mul(m[i][1], v[1]) + P::mul(m[i][2], v[2]));
    return r;
}
inline Vector stabilize(const Quaternion &q, const Vector &inv, const Vector &omega,
                        const Vector &damping, double roll, double pitch, bool contact) {
    Vector inertia{};
    for (unsigned i = 0; i < 3; ++i) {
        volatile float v = 1.f / inv[i];
        inertia[i] = v;
    }
    auto mat = P::orientation(q);
    auto world = P::momentum(P::inertia(mat, inertia), omega);
    if (damping == Vector{{-1, -1, -1}} && roll <= 0 && pitch <= 0)
        return world;
    auto local = product(inverse(mat), world);
    if (!contact) {
        if (roll > 0) {
            float limit = P::mul(P::mul(inertia[0], float(roll)), 2.f);
            local[0] = std::max(-limit, std::min(limit, local[0]));
        }
        if (pitch > 0) {
            float limit = P::mul(P::mul(inertia[1], float(pitch)), 2.f);
            local[1] = std::max(-limit, std::min(limit, local[1]));
        }
    }
    for (unsigned i = 0; i < 3; ++i)
        local[i] = P::mul(local[i], damping[i]);
    return product(mat, local);
}
inline Quaternion clampOrientation(Quaternion q, double roll, double pitch,
                                   Vector *angles = nullptr) {
    const double x = q[0], y = q[1], z = q[2], w = q[3];
    const double n = double(P::mul(q[0], q[0])) + P::mul(q[1], q[1]) + P::mul(q[2], q[2]) +
                     P::mul(q[3], q[3]),
                 s = n > 0 ? 2 / n : 0;
    const double xs = C::store(x * s), ys = C::store(y * s), zs = C::store(z * s);
    const float r00 = C::store(1 - (double(C::store(y * ys)) + C::store(z * zs))),
                r10 = C::store(double(C::store(x * ys)) + C::store(w * zs)),
                r20 = C::store(double(C::store(x * zs)) - C::store(w * ys)),
                r21 = C::store(double(C::store(y * zs)) + C::store(w * xs)),
                r22 = C::store(1 - (double(C::store(x * xs)) + C::store(y * ys)));
    const float cy =
        C::store(std::sqrt(double(C::store(double(P::mul(r00, r00)) + P::mul(r10, r10)))));
    float ax, ay, az;
    if (cy > 16 * FLT_EPSILON) {
        ax = C::store(std::atan2(double(r21), double(r22)));
        ay = C::store(std::atan2(-double(r20), double(cy)));
        az = C::store(std::atan2(double(r10), double(r00)));
    } else {
        const float r12 = C::store(double(C::store(y * zs)) - C::store(w * xs)),
                    r11 = C::store(1 - (double(C::store(x * xs)) + C::store(z * zs)));
        ax = C::store(std::atan2(-double(r12), double(r11)));
        ay = C::store(std::atan2(-double(r20), double(cy)));
        az = 0;
    }
    if (angles)
        *angles = {{ax, ay, az}};
    if (roll > 0) {
        const float limit = float(roll);
        ax = std::max(-limit, std::min(limit, ax));
    }
    if (pitch > 0) {
        const float limit = float(pitch);
        ay = std::max(-limit, std::min(limit, ay));
    }
    const double ci = std::cos(double(ax) * .5), cj = std::cos(double(ay) * .5),
                 ch = std::cos(double(az) * .5), si = std::sin(double(ax) * .5),
                 sj = std::sin(double(ay) * .5), sh = std::sin(double(az) * .5);
    const double cc = C::store(ci * ch), cs = C::store(ci * sh), sc = C::store(si * ch),
                 ss = C::store(si * sh);
    return {{C::store(double(C::store(cj * sc)) - C::store(sj * cs)),
             C::store(double(C::store(cj * ss)) + C::store(sj * cc)),
             C::store(double(C::store(cj * cs)) - C::store(sj * sc)),
             C::store(double(C::store(cj * cc)) + C::store(sj * ss))}};
}
} // namespace motorrock::stabilizev185
