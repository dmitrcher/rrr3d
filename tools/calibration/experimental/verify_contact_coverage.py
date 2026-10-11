#!/usr/bin/env python3
"""Require repeated, isolated 120-step runs with an actual contact force in every case."""
import argparse
import hashlib
import json
from pathlib import Path
from verify_isolation import capture
import compare


def inspect(path, platform):
    cases, isolation = capture(path, platform, 'frame-v25-long-')
    expected = {f'frame-v25-long-p{p}-x{x}-y{y}'
                for p in range(12) for x in range(3) for y in range(2)}
    assert set(cases) == expected, 'Incomplete long-contact matrix'
    for name, case in cases.items():
        assert case['meta']['steps'] == 120, name
        assert case['meta']['fixed_orientation'] is True, name
        assert case['meta']['gravity'] == -20 and case['meta']['ground'] is True, name
        assert all(row['command']['acceleration'] == [0, 0, 0] for row in case['rows']), name
    event, field = ('calibration.point-force', 'normal_force') if platform == 'windows-physx-d3d9' else ('calibration.body-impulse', 'normal_force_magnitude')
    coverage = {name: {'force_records': 0, 'positive_force_records': 0,
                       'maximum_normal_force': 0, 'first_positive_tick': None}
                for name in sorted(cases)}
    semantic = hashlib.sha256()
    with Path(path).open(encoding='utf-8-sig') as stream:
        for line in stream:
            r = json.loads(line)
            if r['event'] != event:
                continue
            name, sample = r['detail'], r['sample']
            assert name in coverage and 0 <= sample['tick'] < 120, name
            force = sample[field]
            assert force >= 0, (name, sample['tick'], force)
            entry = coverage[name]
            entry['force_records'] += 1
            entry['maximum_normal_force'] = max(entry['maximum_normal_force'], force)
            if force > 0:
                entry['positive_force_records'] += 1
                if entry['first_positive_tick'] is None:
                    entry['first_positive_tick'] = sample['tick']
            semantic.update(json.dumps([event, name, sample], sort_keys=True).encode())
    missing = [name for name, entry in coverage.items() if not entry['positive_force_records']]
    return cases, {'isolation': isolation, 'force_event': event,
                   'force_semantic_sha256': semantic.hexdigest(),
                   'cases_without_positive_contact_force': missing, 'coverage': coverage}


def verify(windows0, windows1, mac0, mac1):
    records = [inspect(path, platform) for path, platform in
               [(windows0, 'windows-physx-d3d9'), (windows1, 'windows-physx-d3d9'),
                (mac0, 'mac-jolt-metal'), (mac1, 'mac-jolt-metal')]]
    for index in (0, 2):
        assert records[index][0] == records[index+1][0], 'Physical repeat differs'
        assert records[index][1]['force_semantic_sha256'] == records[index+1][1]['force_semantic_sha256'], 'Contact-force repeat differs'
    report = compare.compare(windows0, mac0)
    return {'cases': 72, 'samples_per_capture': 8640, 'physical_and_force_repeats_exact': True,
            'coverage_complete': all(not record[1]['cases_without_positive_contact_force'] for record in records),
            'captures': [record[1] for record in records], 'comparison': report,
            'limitations': ['Windows records per-point forces; Mac records a manifold force magnitude. Their counts and maxima are coverage diagnostics, not equivalent force observables.',
                            'The actor rotation is frozen; this isolates translation/contact geometry, not rotational collision response.']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('windows0', 'windows1', 'mac0', 'mac1'):
        parser.add_argument(name, type=Path)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    try:
        result = verify(args.windows0, args.windows1, args.mac0, args.mac1)
        args.out.write_text(json.dumps(result, indent=2) + '\n')
        print(json.dumps({'coverage_complete': result['coverage_complete'],
                          'repeats_exact': True,
                          'remaining': len(result['comparison']['exact_parity']['cases_remaining'])}))
        return 0 if result['coverage_complete'] and result['comparison']['exact_parity']['complete'] else 1
    except (AssertionError, KeyError, OSError, ValueError) as error:
        parser.exit(2, str(error) + '\n')


if __name__ == '__main__':
    raise SystemExit(main())
