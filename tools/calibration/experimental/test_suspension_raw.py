#!/usr/bin/env python3
"""Independent first-step Nx spring oracle at measured SDK ray geometry.

The spring impulse follows the contact normal, not the chassis ray direction.
Serialized stiffness/damping stay unchanged when the chassis is inclined.
This does not assert that the Mac ray geometry or later tire solve is exact.
"""
import json
import math
import sys
from pathlib import Path


def solve(case, scale=1.0):
    inp, actual = case['input'], case['actual']
    dt, mass = inp['dt'], actual['mass']
    sample = case['sample']
    contact = case['geometry']['wheels'][0]
    rx, ry, _ = contact['point']
    angle = inp['initial_pitch']
    ix, iy, iz = actual['inertia']
    response = 1 / mass + rx * rx / iy + ry * ry * (
        math.cos(angle) ** 2 / ix + math.sin(angle) ** 2 / iz)
    wheel = inp['fixture_parameters']['wheels'][0]
    k, d = wheel['spring'] * scale, wheel['damper'] * scale
    gamma = 1 / (dt * (d + dt * k))
    effective = 1 / (response + gamma)
    ratio = effective * response
    displacement = sample['wheels'][0]['suspension_length'] - wheel['travel'] + .05
    bias = .7 * dt * k * gamma * displacement
    velocity = inp['gravity'] * dt
    lambdas = [0.0] * sum(w['contact'] for w in sample['wheels'])
    n = case['iterations']
    for step in range(n + 1):
        for i in reversed(range(len(lambdas))):
            old = lambdas[i]
            lambdas[i] = max(0, ratio * old - effective * (
                velocity + bias * (1 - ratio ** n if step == n else 1)))
            velocity += (lambdas[i] - old) * response
        if step == n - 1:
            biased_impulse = sum(lambdas)
    total = sum(lambdas)
    return (inp['gravity'] * dt + total / mass,
            inp['height'] + (inp['gravity'] * dt + biased_impulse / mass) * dt,
            -rx * total)


if __name__ == '__main__':
    cases = json.loads(Path(sys.argv[1]).read_text())['cases']
    assert len(cases) == 240
    maxima = [0.0, 0.0, 0.0]
    rejected = 0
    for case in cases:
        sample = case['sample']
        observed = (sample['velocity'][2], sample['position'][2], sample['angular_momentum'][1])
        predicted = solve(case)
        for i, tolerance in enumerate((1e-6, 2e-6, 1e-4)):
            error = abs(predicted[i] - observed[i])
            maxima[i] = max(maxima[i], error)
            assert error < tolerance, (case['name'], case['iterations'], i, error)
        wrong = solve(case, 1 / math.cos(case['input']['initial_pitch']))
        rejected += abs(wrong[0] - observed[0]) > 1e-4
    assert rejected == 192
    print('240 Nx first-step spring solves passed; max V/P/L', maxima,
          '; rejected cosine-scaled rows', rejected)
