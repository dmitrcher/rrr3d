#!/usr/bin/env python3
"""Verify recorded frozen quaternions and optional contact actor matrices in float32."""
import argparse
import json
import struct
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import compare


def floats(values):
    return [struct.unpack('f', struct.pack('f', value))[0] for value in values]


def matrices(path, platform):
    result = {}
    current = None
    with path.open(encoding='utf-8-sig') as stream:
        for line in stream:
            row = json.loads(line)
            assert row['platform'] == platform
            sample = row.get('sample', {})
            if row['event'] == 'calibration.begin':
                current = row['detail']
            if platform == 'windows-physx-d3d9' and row['event'] == 'calibration.body-points':
                prefix = 'actor'+str(sample['car_shape'])+'_axis'
                if prefix+'0' not in sample:
                    continue
                key = (row['detail'], sample['tick'])
            elif platform == 'mac-jolt-metal' and row['event'] == 'physics.contact-manifold':
                prefix = 'source_box_axis'
                if prefix+'0' not in sample:
                    continue
                key = (current, sample['step'])
            else:
                continue
            matrix = floats([value for axis in range(3) for value in sample[prefix+str(axis)]])
            if key in result:
                assert result[key] == matrix, (key, 'Multiple actor matrices differ')
            result[key] = matrix
    return result


def verify(windows, mac, windows_basis=None, mac_basis=None):
    left = compare.load(windows, 'windows-physx-d3d9')
    right = compare.load(mac, 'mac-jolt-metal')
    assert left.keys() == right.keys(), 'Scenario sets differ'
    samples = 0
    for name, case in left.items():
        assert case['meta'].get('fixed_orientation') is True, name
        other = right[name]
        assert {k: v for k, v in case['meta'].items() if k != 'profile'} == {k: v for k, v in other['meta'].items() if k != 'profile'}, (name, 'Fixture differs')
        for a, b in zip(case['rows'], other['rows']):
            assert a['command'] == b['command'], (name, a['tick'], 'Commands differ')
            assert floats(a['rotation']) == floats(b['rotation']), (name, a['tick'], 'Quaternion differs')
            samples += 1
    result = {'cases': len(left), 'quaternion_samples': samples,
              'all_recorded_quaternions_exact_float32': True,
              'scope': 'Frozen orientation only. Position, velocity, forces and free rotational response require separate parity checks.'}
    if windows_basis is not None:
        assert mac_basis is not None
        a = matrices(windows_basis, 'windows-physx-d3d9')
        b = matrices(mac_basis, 'mac-jolt-metal')
        assert a and a.keys() == b.keys(), 'Contact matrix case/tick sets differ'
        assert a == b, 'Contact actor matrices differ'
        result['contact_actor_matrices_checked'] = len(a)
        result['all_contact_actor_matrices_exact_float32'] = True
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('windows', type=Path)
    parser.add_argument('mac', type=Path)
    parser.add_argument('--windows-basis', type=Path)
    parser.add_argument('--mac-basis', type=Path)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if bool(args.windows_basis) != bool(args.mac_basis):
        parser.error('Both basis captures are required')
    result = verify(args.windows, args.mac, args.windows_basis, args.mac_basis)
    args.out.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(result))


if __name__ == '__main__':
    main()
