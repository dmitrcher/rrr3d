#pragma once

namespace r3d::physics::nativeshapemass
{
// Original SSE scalar arithmetic: box 0x1370e0, cylinder 0x136bd0,
// translation 0x135e60. Explicit float stores prevent multiply-add contraction.
inline float add(float a, float b) noexcept { volatile float v = a + b; return v; }
inline float sub(float a, float b) noexcept { volatile float v = a - b; return v; }
inline float mul(float a, float b) noexcept { volatile float v = a * b; return v; }
inline float div(float a, float b) noexcept { volatile float v = a / b; return v; }

struct Integrals
{
    float tensor[3][3]{};
    float center[3]{};
    float mass = 0.0F;
};

inline void translate(Integrals& shape, const float delta[3]) noexcept
{
    float before[3][3], after[3][3];
    auto skew = [](const float p[3], float s[3][3])
    {
        s[0][0] = s[1][1] = s[2][2] = 0.0F;
        s[0][1] = -p[2]; s[0][2] = p[1];
        s[1][0] = p[2]; s[1][2] = -p[0];
        s[2][0] = -p[1]; s[2][1] = p[0];
    };
    float center[3];
    for (int i = 0; i < 3; ++i) center[i] = add(shape.center[i], delta[i]);
    skew(shape.center, before); skew(center, after);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            const float a = add(add(mul(before[i][0], before[0][j]),
                mul(before[i][1], before[1][j])), mul(before[i][2], before[2][j]));
            const float b = add(add(mul(after[i][0], after[0][j]),
                mul(after[i][1], after[1][j])), mul(after[i][2], after[2][j]));
            shape.tensor[i][j] = add(shape.tensor[i][j], mul(sub(a, b), shape.mass));
        }
    for (int i = 0; i < 3; ++i) shape.center[i] = center[i];
}

inline void finalizeSingleShape(Integrals& shape) noexcept
{
    // Providers add the translated shape to a zero accumulator. Their center
    // is a first moment divided by mass, including both float rounding steps.
    const float reciprocal = div(1.0F, shape.mass);
    for (float& c : shape.center) c = mul(mul(c, shape.mass), reciprocal);
}

inline Integrals box(const float half[3], const float position[3]) noexcept
{
    Integrals shape;
    float mass = half[0] != 0.0F ? half[0] : 1.0F;
    if (half[1] != 0.0F) mass = mul(half[1], mass);
    if (half[2] != 0.0F) mass = mul(half[2], mass);
    shape.mass = mul(mass, 8.0F);
    const float scale = mul(shape.mass, 0.3333333432674408F);
    const float x = mul(half[0], half[0]);
    const float y = mul(half[1], half[1]);
    const float z = mul(half[2], half[2]);
    shape.tensor[0][0] = mul(add(z, y), scale);
    shape.tensor[1][1] = mul(add(z, x), scale);
    shape.tensor[2][2] = mul(add(y, x), scale);
    translate(shape, position);
    finalizeSingleShape(shape);
    return shape;
}

inline Integrals wheel(float radius, float halfLength, const float position[3]) noexcept
{
    Integrals shape;
    shape.mass = mul(mul(mul(mul(halfLength, 2.0F), radius), radius), 3.1415927410125732F);
    const float along = mul(mul(mul(shape.mass, radius), radius), 0.5F);
    const float perpendicular = mul(mul(add(mul(mul(radius, radius), 3.0F),
        mul(mul(halfLength, halfLength), 4.0F)), shape.mass), 0.0833333358168602F);
    // Original wheel local pose permutes cylinder Y to source Z.
    shape.tensor[0][0] = shape.tensor[1][1] = perpendicular;
    shape.tensor[2][2] = along;
    translate(shape, position);
    finalizeSingleShape(shape);
    return shape;
}

inline void accumulate(Integrals& combined, const Integrals& shape) noexcept
{
    combined.mass = add(combined.mass, shape.mass);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            combined.tensor[i][j] = add(combined.tensor[i][j], shape.tensor[i][j]);
}

inline void scaleToMass(Integrals& shape, float mass) noexcept
{
    const float scale = div(mass, shape.mass);
    for (auto& row : shape.tensor) for (float& value : row) value = mul(value, scale);
    shape.mass = mass;
}
}
