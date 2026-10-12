#pragma once
#include <array>
#include "NativeSpringSplitV53.h"
namespace motorrock_native_wheel_frame_v57 {
using namespace motorrock_native_spring_v53;
using Vector = std::array<float, 3>;
using Quaternion = std::array<float, 4>;
inline Vector cross(const Vector &a, const Vector &b) {
 return {{sub(mul(a[1], b[2]), mul(a[2], b[1])),
          sub(mul(a[2], b[0]), mul(a[0], b[2])),
          sub(mul(a[0], b[1]), mul(a[1], b[0]))}};
}
// Original actor inverse-transform: point dot XYZ, direction dot XZY.
// The factor two is applied after all three terms have been rounded.
inline Vector inverse(const Quaternion &q, const Vector &v, bool direction) {
 float dot = direction? add(add(mul(v[0], q[0]), mul(v[2], q[2])), mul(v[1], q[1]))
                      : add(add(mul(v[0], q[0]), mul(v[1], q[1])), mul(v[2], q[2]));
 float coefficient = sub(mul(q[3], q[3]), 0.5f);
 Vector xyz{{q[0], q[1], q[2]}}, perpendicular = cross(xyz, v), result;
 for (unsigned i = 0; i < 3; ++i)
  result[i] = mul(add(sub(mul(coefficient, v[i]), mul(q[3], perpendicular[i])), mul(q[i], dot)), 2.0f);
 return result;
}
inline Vector local_point(const Quaternion &q, const Vector &point, const Vector &center) {
 Vector delta; for (unsigned i = 0; i < 3; ++i) delta[i] = sub(point[i], center[i]);
 return inverse(q, delta, false);
}
// Original row preparation: point dot XYZ, direction dot ZXY.
// The doubled terms are rounded separately before the final additions.
inline Vector forward(const Quaternion &q, const Vector &v, bool direction) {
 float dot = direction? add(add(mul(v[2], q[2]), mul(v[0], q[0])), mul(v[1], q[1]))
                      : add(add(mul(v[0], q[0]), mul(v[1], q[1])), mul(v[2], q[2]));
 float coefficient = sub(mul(mul(q[3], q[3]), 2.0f), 1.0f);
 Vector xyz{{q[0], q[1], q[2]}}, perpendicular = cross(xyz, v), result;
 for (unsigned i = 0; i < 3; ++i)
  result[i] = add(add(mul(coefficient, v[i]), mul(mul(perpendicular[i], q[3]), 2.0f)), mul(mul(q[i], dot), 2.0f));
 return result;
}
inline Vector body_axis(const Quaternion &q, const Vector &world_axis) {
 return forward(q, inverse(q, world_axis, true), true);
}
inline Vector body_lever(const Quaternion &q, const Vector &world_point, const Vector &center) {
 return forward(q, local_point(q, world_point, center), false);
}
}
