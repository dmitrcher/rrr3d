#pragma once
#include <array>
#include <cmath>

// Experimental reconstruction of the original scalar SSE mesh query.
// Keep each store and the different axis sum orders: reassociation and FMA
// change the actual contact lever arm on large source triangles.
namespace motorrock::wheelrayv52 {
using Vector = std::array<float, 3>;
inline float add(float a, float b) { volatile float v = a + b; return v; }
inline float sub(float a, float b) { volatile float v = a - b; return v; }
inline float mul(float a, float b) { volatile float v = a * b; return v; }
inline float div(float a, float b) { volatile float v = a / b; return v; }
inline Vector subtract(Vector a, Vector b) {
    return {sub(a[0], b[0]), sub(a[1], b[1]), sub(a[2], b[2])};
}
inline Vector cross(Vector a, Vector b) {
    return {sub(mul(a[1], b[2]), mul(a[2], b[1])),
            sub(mul(a[2], b[0]), mul(a[0], b[2])),
            sub(mul(a[0], b[1]), mul(a[1], b[0]))};
}
inline float dotXZY(Vector a, Vector b) {
    return add(add(mul(a[0], b[0]), mul(a[2], b[2])), mul(a[1], b[1]));
}
inline float dotZYX(Vector a, Vector b) {
    return add(add(mul(a[2], b[2]), mul(a[1], b[1])), mul(a[0], b[0]));
}
struct Intersection { float distance, u, v; };
inline Intersection intersect(Vector origin, Vector direction,
                              Vector a, Vector b, Vector c) {
    const Vector e1 = subtract(b, a), e2 = subtract(c, a);
    const Vector p = cross(direction, e2), t = subtract(origin, a);
    const float determinant = dotXZY(p, e1);
    const Vector q = cross(t, e1);
    const float reciprocal = div(1.0F, determinant);
    return {mul(dotXZY(q, e2), reciprocal),
            mul(dotXZY(t, p), reciprocal),
            mul(dotZYX(direction, q), reciprocal)};
}
inline Vector interpolate(Vector a, Vector b, Vector c, float u, float v) {
    const float w = sub(sub(1.0F, u), v);
    return {add(add(mul(a[0], w), mul(c[0], v)), mul(b[0], u)),
            add(add(mul(b[1], u), mul(a[1], w)), mul(c[1], v)),
            add(add(mul(a[2], w), mul(b[2], u)), mul(c[2], v))};
}
} // namespace motorrock::wheelrayv52
