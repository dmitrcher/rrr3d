#!/usr/bin/env python3
"""Compare standalone geometry outputs with recorded native SDK contact points."""
import argparse
import json
import math
import struct
from pathlib import Path


def floats(values):
    return [struct.unpack('f', struct.pack('f', float(value)))[0] for value in values]


def compare(path, fixture):
    lines = path.read_text().splitlines()
    assert len(lines) == len(fixture['cases']), 'Missing/extra output cases'
    point_errors, separation_errors, cases = [], [], {}
    for line, expected in zip(lines, fixture['cases']):
        values = line.split()
        count = int(values[0])
        assert len(values) == 1+8*count, 'Wrong output format'
        assert count == len(expected['points']), (expected['name'], 'Contact count differs')
        errors, separations, nonexact = [], [], 0
        for index, point in enumerate(expected['points']):
            base = 1+8*index
            actual = floats(values[base:base+3])
            assert all(math.isfinite(v) for v in floats(values[base:base+7])), 'Non-finite contact'
            assert int(values[base+7]) == point['triangle'], 'Triangle order differs'
            distance = math.dist(actual, floats(point['point']))
            errors.append(distance)
            separations.append(abs(floats(values[base+6:base+7])[0]-point['separation']))
            nonexact += distance != 0
        point_errors.extend(errors)
        separation_errors.extend(separations)
        cases[expected['name']] = {'nonexact_points': nonexact,
            'maximum_point_distance': max(errors), 'maximum_separation_error': max(separations)}
    return {'scope': fixture['scope'], 'cases': len(cases), 'points': len(point_errors),
            'counts_and_triangle_order_exact': True,
            'nonexact_points': sum(c['nonexact_points'] for c in cases.values()),
            'point_rms': math.sqrt(sum(x*x for x in point_errors)/len(point_errors)),
            'maximum_point_distance': max(point_errors),
            'separation_rms': math.sqrt(sum(x*x for x in separation_errors)/len(separation_errors)),
            'maximum_separation_error': max(separation_errors), 'details': cases}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('captures', nargs='+', type=Path)
    parser.add_argument('--reference', type=Path, default=Path(__file__).with_name('expected-sdk-points.json'))
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    fixture = json.loads(args.reference.read_text())
    reports = {path.name: compare(path, fixture) for path in args.captures}
    args.out.write_text(json.dumps(reports, indent=2)+'\n')
    for name, report in reports.items():
        print(name, report['nonexact_points'], report['point_rms'], report['maximum_point_distance'])


if __name__ == '__main__':
    main()
