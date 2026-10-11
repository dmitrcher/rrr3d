#!/usr/bin/env python3
"""Check independently captured Nx suspension response, including a wrong-k control."""
import json, math, sys
from pathlib import Path


def solve(matrix, rhs):
    rows = [list(row) + [value] for row, value in zip(matrix, rhs)]
    size = len(rows)
    for index in range(size):
        pivot = max(range(index, size), key=lambda row: abs(rows[row][index]))
        rows[index], rows[pivot] = rows[pivot], rows[index]
        value = rows[index][index]
        if abs(value) < 1e-12:
            raise AssertionError('Singular spring system')
        rows[index] = [entry / value for entry in rows[index]]
        for row in range(size):
            if row != index:
                value = rows[row][index]
                rows[row] = [a - value*b for a, b in zip(rows[row], rows[index])]
    return [row[-1] for row in rows]


def velocity_response(case, stiffness_scale):
    data, actual = case['input'], case['actual']
    mass, inertia, center = actual['mass'], actual['inertia'], actual['com']
    wheels = data['fixture_parameters']['wheels']
    dt = data['dt']
    arms = [(wheel['position'][0] - center[0], wheel['position'][1] - center[1]) for wheel in wheels]
    matrix = []
    for index, wheel in enumerate(wheels):
        x, y = arms[index]
        row = [1/mass + y*other_y/inertia[0] + x*other_x/inertia[1] for other_x, other_y in arms]
        row[index] += 1 / (dt * (wheel['damper'] + dt * wheel['spring'] * stiffness_scale))
        matrix.append(row)
    return 1 - sum(solve(matrix, [1] * len(wheels))) / mass


def main():
    fixture = json.loads(Path(sys.argv[1]).read_text())
    cases = {case['name']: case for case in fixture['cases']}
    if len(cases) != 36:
        raise AssertionError('Missing isolated spring probes')
    checked, rejected = 0, 0
    for kind in range(6):
        for grip in range(2):
            neutral = cases[f'spring-probe-{kind}-0-{grip}']
            downward = cases[f'spring-probe-{kind}-1-{grip}']
            observed = neutral['first']['velocity'][2] - downward['first']['velocity'][2]
            expected = velocity_response(neutral, 1)
            wrong = velocity_response(neutral, .7)
            if abs(observed - expected) > 0.000005:
                raise AssertionError(f'{neutral["name"]}: source stiffness response {observed} vs {expected}')
            if abs(observed - wrong) < .001:
                raise AssertionError('Wrong physical stiffness escaped the negative control')
            checked += 1
            rejected += 1
            for speed in range(3):
                without = cases[f'spring-probe-{kind}-{speed}-0']['first']
                with_grip = cases[f'spring-probe-{kind}-{speed}-1']['first']
                for field in ('position', 'velocity', 'angular_momentum', 'rotation'):
                    if without[field] != with_grip[field]:
                        raise AssertionError('Tire traction affects the supposedly isolated first spring step')
            if neutral['first'] != neutral['iterations']['4']:
                raise AssertionError('Four-iteration fixture mismatch')
    print(f'{checked} independently measured Nx implicit-stiffness responses; {rejected} wrong-k controls rejected; all 36 first steps isolated from traction')


if __name__ == '__main__':
    main()
