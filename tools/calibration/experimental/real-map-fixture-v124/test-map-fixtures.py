#!/usr/bin/env python3
"""Check actual game/map replay determinism, excluding wall-clock log fields.

Keeps caller's physics profile/experiment settings. Runs each map twice at seed
42 and once at 999, with RRR3D_AI_DRIVER=1 deliberately set in every run.
This is a Mac repeatability check, not a Windows/Mac parity verdict.
"""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import subprocess


def finite(value):
    if isinstance(value, float):
        assert math.isfinite(value), value
    elif isinstance(value, dict):
        for item in value.values():
            finite(item)
    elif isinstance(value, list):
        for item in value:
            finite(item)


def inspect(path):
    rows = [json.loads(line) for line in path.read_text().splitlines()]
    events = [row for row in rows if row['event'].startswith('solo.')]
    assert not any(row['event'] == 'solo.error' for row in events)
    begin = [row['sample'] for row in events if row['event'] == 'solo.begin']
    assert len(begin) == 1
    assert begin[0]['opponents'] == 0 and begin[0]['vehicles'] == 1
    assert begin[0]['human_ai'] is False and begin[0]['gameplay_updates'] is False
    samples = [row['sample'] for row in events if row['event'] == 'solo.sample']
    assert len(samples) == 900 and [s['tick'] for s in samples] == list(range(900))
    assert all(s['resets'] == begin[0]['initial_reset_count'] for s in samples)
    assert all(len(s['wheels']) == len(samples[0]['wheels']) > 0 for s in samples)
    for field in ['throttle', 'reverse', 'brake', 'steering']:
        assert any(s['input'][field] != 0 for s in samples), field
    assert samples[720]['delta_velocity'] == [0, 2, 0]
    assert samples[780]['delta_velocity'] == [0, 0, 2]
    drives = [row['sample'] for row in events if row['event'] == 'solo.drive']
    assert {s['tick'] for s in drives} == set(range(900))
    for tick in range(900):
        steps = [s for s in drives if s['tick'] == tick]
        assert [s['substep'] for s in steps] == list(range(len(steps)))
        assert all(s['dt'] > 0 for s in steps)
        assert abs(sum(s['dt'] for s in steps) - 1 / 60) < 1e-8
        assert abs(samples[tick]['time_seconds'] - (tick + 1) / 60) < 1e-7
    assert any(s['motor_torque'] != 0 for s in drives)
    assert any(s['brake_torque'] > 0 for s in drives)
    assert any(s['steering_angle'] != 0 for s in drives)
    ends = [row['sample'] for row in events if row['event'] == 'solo.end']
    assert ends == [{'ticks': 900, 'completed': True}]
    data = [{'event': row['event'], 'detail': row['detail'], 'sample': row['sample']}
            for row in events if row['event'] in ('solo.sample', 'solo.drive')]
    finite(data)
    digest = hashlib.sha256(json.dumps(data, sort_keys=True).encode()).hexdigest()
    return {'samples': len(samples), 'drive_substeps': len(drives),
            'semantic_sha256': digest, 'opponents': 0, 'human_ai': False}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--exe', required=True, type=Path)
    parser.add_argument('--output-dir', required=True, type=Path)
    parser.add_argument('--tracks', nargs='+', type=int, default=[0, 16, 48, 64])
    args = parser.parse_args()
    args.output_dir.mkdir(parents=True, exist_ok=True)
    report = {'scope': 'Mac repeatability only; original geometry and GameCar, no gameplay or renderer',
              'physics_environment': {k: v for k, v in os.environ.items()
                                      if k.startswith('RRR3D_EXPERIMENT') or k == 'RRR3D_CALIBRATION_PROFILE'},
              'tracks': {}}
    for track in args.tracks:
        runs = []
        for run, seed in enumerate([42, 42, 999]):
            log = (args.output_dir / f'track{track}-run{run}.jsonl').resolve()
            assert not log.exists(), f'use a fresh output directory: {log}'
            env = dict(os.environ, RRR3D_TRACE_FILE=str(log),
                       RRR3D_AI_DRIVER='1', RRR3D_SEED=str(seed))
            env['RRR3D_WORLD_FIXTURE_FILE_V124']=str(log.with_suffix('.words'))
            with log.with_suffix('.log').open('w') as console:
                subprocess.run([str(args.exe.resolve()), '--solo-physics-replay',
                                f'--track={track}', '--car=marauder'], env=env,
                               stdout=console, stderr=subprocess.STDOUT,
                               check=True, timeout=90)
            result = inspect(log)
            result['seed'] = seed
            result['trace'] = str(log)
            runs.append(result)
        assert len({r['semantic_sha256'] for r in runs}) == 1, f'non-repeatable track {track}'
        report['tracks'][str(track)] = {'exactly_repeatable': True, 'runs': runs}
        (args.output_dir / 'verification.json').write_text(json.dumps(report, indent=2) + '\n')
        print(f'track {track}: 3 identical runs, 900 samples each; no opponents or AI', flush=True)


if __name__ == '__main__':
    main()
