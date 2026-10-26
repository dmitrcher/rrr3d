"""Bounded total-acceleration equivalence, not native GameCar runtime observation.

Recorded SDK scene gravity=-20 is compared to Source GameCar JumpProgress's
fixed extra acceleration=-20 in a zero-gravity Mac scene. All body, wheel and
time inputs match. Source semantics establish the intended command; recorded
SDK states independently establish the target total-acceleration trajectory.
"""
from pathlib import Path
import gzip
import hashlib
import importlib.util
import json
import os
import subprocess

root = Path.cwd()
out = root / 'outputs/controller-jump-v187'
probe = root / 'work/controller-jump-v187/SourceJumpGravityProbe'
base = json.loads((root / 'outputs/controller-clutch-v186/final-broad/manifest.json').read_text())['environment']
spec = importlib.util.spec_from_file_location('words', root / 'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py')
words = importlib.util.module_from_spec(spec)
spec.loader.exec_module(words)
native = []
with (root / 'outputs/calibration-v14-source-windows.jsonl').open() as stream:
    for row in map(json.loads, stream):
        if row['event'] == 'calibration.sample' and row['detail'] == 'gravity':
            native.append(row['sample'])
assert len(native) == 120
reports = []
for mode in ['off', 'on']:
    previous = None
    for rep in range(2):
        env = {k: v for k, v in os.environ.items() if not k.startswith(('RRR3D_', 'NX_'))}
        env.update(base)
        env['RRR3D_EXPERIMENT_GAMECAR_JUMP_V187'] = '1' if mode == 'on' else '0'
        path = out / f'gravity-{mode}{rep}.jsonl'
        with path.with_suffix('.log').open('w') as log:
            subprocess.run([str(probe), str(path), 'list:gravity'], env=env, stdout=log, stderr=log, check=True, timeout=90)
        rows = [r['sample'] for r in map(json.loads, path.read_text().splitlines()) if r['event'] == 'calibration.sample']
        assert len(rows) == 120
        physical = [words.physical(r) for r in rows]
        if previous is not None:
            assert physical == previous
        previous = physical
        differences = [dict(tick=i, fields=[k for k in words.physical(n) if words.physical(n)[k] != m[k]], windows=n, mac=rows[i]) for i, (n, m) in enumerate(zip(native, physical)) if words.physical(n) != m]
        reports.append(dict(mode=mode, repeat=rep, exact=120-len(differences), first_difference=differences[0] if differences else None))
        print(mode, rep, reports[-1]['exact'], '/120', flush=True)
report = dict(scope=__doc__.strip(), native_gamecar_jump_runtime_observed=False, raw_api_route_equivalence_proven=False, runs=reports, probe_sha256=hashlib.sha256(probe.read_bytes()).hexdigest(), goal_complete=False)
(out / 'gravity-comparison.json').write_text(json.dumps(report, indent=2) + '\n')
assert all(r['exact'] == 120 for r in reports if r['mode'] == 'on'), 'Fixed JumpProgress acceleration not reproduced'
