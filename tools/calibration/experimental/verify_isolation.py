#!/usr/bin/env python3
"""Validate isolation, repeats, and progressive parity from four native captures."""
import argparse
import hashlib
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import compare


def capture(path, platform):
    cases = compare.load(path, platform)
    semantic = hashlib.sha256()
    contacts = 0
    maximum_reported_friction_force = 0.0
    for name, case in cases.items():
        meta = case['meta']
        assert name.startswith('isolate-'), 'Unexpected scenario'
        assert meta.get('wheel_contacts_enabled') is False, name
        assert meta.get('body_contact_friction_enabled') is False, name
        assert meta.get('body_linear_damping') == 0, name
        assert meta.get('body_angular_damping') == 0, name
        p = meta['fixture_parameters']
        assert p['angular_damping'] == 0 and p['body_friction'] == 0, name
        assert not p['traction'] and not p['extra_gravity'], name
        assert p['brake_limit'] == p['tire_spring'] == 0, name
        assert p['air_pitch'] == p['clamp_roll'] == p['clamp_pitch'] == 0, name
        assert p['game_angular_damping'] == [1, 1, 1], name
        for wheel in p['wheels']:
            assert wheel['spring'] == wheel['damper'] == 0, name
            assert wheel['longitudinal_tire'][-1] == wheel['lateral_tire'][-1] == 0, name
        for sample in case['rows']:
            command = sample['command']
            assert command['wheel_torque'] == command['brake_torque'] == command['steer_radians'] == 0, name
            assert command['delta_velocity'] == [0, 0, 0], name
            for wheel in sample['wheels']:
                assert wheel['contact'] is False, name
                for field in ('angular_speed', 'engine_normal_force', 'longitudinal_impulse', 'lateral_impulse'):
                    assert wheel[field] == 0, (name, sample['tick'], field)
        # Include commands, complete state, metadata and mass properties.
        semantic.update(json.dumps([name, case], sort_keys=True, separators=(',', ':')).encode())
    with Path(path).open(encoding='utf-8-sig') as stream:
        for line in stream:
            r = json.loads(line)
            if r['event'] in ('calibration.body-points', 'calibration.body-contact'):
                contacts += 1
                s = r['sample']
                for field in ('friction0', 'friction1', 'static_friction0', 'static_friction1'):
                    if field in s:
                        assert s[field] == 0, (r['detail'], s['tick'], field)
            if r['event'] == 'calibration.body-impulse':
                maximum_reported_friction_force = max(maximum_reported_friction_force,
                    compare.norm(r['sample']['friction_force'], [0, 0, 0]))
    return cases, {
        'sha256': hashlib.sha256(Path(path).read_bytes()).hexdigest(),
        'semantic_sha256': semantic.hexdigest(),
        'cases': len(cases), 'samples': sum(len(c['rows']) for c in cases.values()),
        'contact_coefficient_records_checked': contacts,
        # PhysX reports a tiny residual here even when all callback coefficients
        # and NX_MF_DISABLE_FRICTION are zero. Keep the measured value visible.
        'maximum_reported_friction_force': maximum_reported_friction_force,
    }


def verify(windows0, windows1, mac0, mac1):
    results = []
    for p, platform in [(windows0, 'windows-physx-d3d9'), (windows1, 'windows-physx-d3d9'),
                        (mac0, 'mac-jolt-metal'), (mac1, 'mac-jolt-metal')]:
        results.append(capture(p, platform))
    for i in (0, 2):
        assert results[i][1]['semantic_sha256'] == results[i+1][1]['semantic_sha256'], 'Repeat differs'
    report = compare.compare(str(windows0), str(mac0))
    return {'captures': [r[1] for r in results], 'repeatable': True,
            'exact_parity': report['exact_parity'], 'comparison': report}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ('windows0', 'windows1', 'mac0', 'mac1'): p.add_argument(name, type=Path)
    p.add_argument('--out', type=Path, required=True)
    args = p.parse_args()
    try:
        result = verify(args.windows0, args.windows1, args.mac0, args.mac1)
        args.out.write_text(json.dumps(result, indent=2) + '\n')
        print(json.dumps({'repeatable': True, 'exact_parity': result['exact_parity']}))
        return 0 if result['exact_parity']['complete'] else 1
    except (AssertionError, OSError, KeyError, ValueError) as e:
        p.exit(2, str(e) + '\n')


if __name__ == '__main__': sys.exit(main())
