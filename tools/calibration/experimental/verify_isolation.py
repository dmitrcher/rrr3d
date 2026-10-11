#!/usr/bin/env python3
"""Validate isolation, repeats, and progressive parity from four native captures."""
import argparse
import hashlib
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import compare


def capture(path, platform, expected_prefix='isolate-'):
    cases = compare.load(path, platform)
    semantic = hashlib.sha256()
    contacts = 0
    coefficient_records_by_case = {name: 0 for name in cases}
    actual_contact_points_by_case = {name: 0 for name in cases}
    positive_normal_force_records_by_case = {name: 0 for name in cases}
    response_contact_points_by_case = {name: 0 for name in cases}
    sensor_contact_points_by_case = {name: 0 for name in cases}
    contact_semantic = hashlib.sha256()
    current_case = None
    maximum_reported_friction_force = 0.0
    for name, case in cases.items():
        meta = case['meta']
        assert name.startswith(expected_prefix), 'Unexpected scenario'
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
            if r['event'] == 'calibration.begin':
                current_case = r['detail']
            if r['event'] in ('calibration.body-points', 'calibration.body-contact'):
                contacts += 1
                coefficient_records_by_case[r['detail']] += 1
                s = r['sample']
                assert 'friction0' in s and 'friction1' in s, 'Missing friction coefficients'
                for field in ('friction0', 'friction1', 'static_friction0', 'static_friction1'):
                    if field in s:
                        assert s[field] == 0, (r['detail'], s['tick'], field)
            if r['event'] in ('physics.contact-manifold', 'calibration.body-points',
                              'calibration.body-contact', 'calibration.point-force', 'calibration.body-impulse'):
                contact_semantic.update(json.dumps([current_case, r['event'], r['detail'], r['sample']],
                    sort_keys=True, separators=(',', ':')).encode())
            if r['event'] == 'physics.contact-manifold':
                assert current_case in cases, 'Contact outside active fixture'
                contacts += 1
                coefficient_records_by_case[current_case] += 1
                s = r['sample']
                actual_contact_points_by_case[current_case] += len(s['points'])
                assert 'sensor0' in s and 'sensor1' in s, 'Missing sensor classification; recapture Mac diagnostics'
                point_counts = sensor_contact_points_by_case if s['sensor0'] or s['sensor1'] else response_contact_points_by_case
                point_counts[current_case] += len(s['points'])
                for field in ('friction0', 'friction1'):
                    assert s[field] == 0, (current_case, s['step'], field)
            if r['event'] == 'calibration.point-force':
                actual_contact_points_by_case[r['detail']] += 1
                response_contact_points_by_case[r['detail']] += 1
            if r['event'] == 'calibration.body-impulse':
                name = r['detail']
                sample = r['sample']
                normal_force = (sample['normal_force_magnitude'] if 'normal_force_magnitude' in sample
                                else compare.norm(sample['normal_force'], [0, 0, 0]))
                positive_normal_force_records_by_case[name] += normal_force > 0
                maximum_reported_friction_force = max(maximum_reported_friction_force,
                    compare.norm(r['sample']['friction_force'], [0, 0, 0]))
            if r['event'] == 'calibration.end':
                current_case = None
    for name in cases:
        if name.startswith('isolate-03-contact-') and name.endswith('-f1'):
            assert coefficient_records_by_case[name] > 0, (name, 'No actual contact-coefficient evidence')
            assert response_contact_points_by_case[name] > 0, (name, 'No actual contact-point evidence')
            assert positive_normal_force_records_by_case[name] > 0, (name, 'No positive normal-force evidence')
        elif name.startswith('isolate-03-contact-') and name.endswith('-f0'):
            assert response_contact_points_by_case[name] == 0, (name, 'Ground-off fixture has response contact points')
            assert positive_normal_force_records_by_case[name] == 0, (name, 'Ground-off fixture has contact force')
    return cases, {
        'sha256': hashlib.sha256(Path(path).read_bytes()).hexdigest(),
        'semantic_sha256': semantic.hexdigest(),
        'contact_semantic_sha256': contact_semantic.hexdigest(),
        'cases': len(cases), 'samples': sum(len(c['rows']) for c in cases.values()),
        'contact_coefficient_records_checked': contacts,
        'contact_coefficient_records_by_case': coefficient_records_by_case,
        'actual_contact_points_by_case': actual_contact_points_by_case,
        'response_contact_points_by_case': response_contact_points_by_case,
        'sensor_contact_points_by_case': sensor_contact_points_by_case,
        'positive_normal_force_records_by_case': positive_normal_force_records_by_case,
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
        assert results[i][1]['contact_semantic_sha256'] == results[i+1][1]['contact_semantic_sha256'], 'Contact repeat differs'
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
