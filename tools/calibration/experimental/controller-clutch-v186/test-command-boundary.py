"""Clutch fields must have no effect without both V186 and real-game provenance."""
from pathlib import Path
import hashlib
import json
import os
import subprocess

root = Path.cwd()
out = root / 'outputs/controller-clutch-v186/command-boundary'
out.mkdir(exist_ok=True)
env = {k: v for k, v in os.environ.items() if not k.startswith(('RRR3D_', 'NX_'))}
env.update(json.loads((root / 'outputs/controller-clutch-v186/world-environment.json').read_text()))
env.pop('RRR3D_STABILIZE_TRACE_FILE_V113', None)
exe = root / 'work/controller-clutch-v186/SourceClutchWorldProbe'
inputs = root / 'outputs/controller-clutch-v186/world-input.words'
reports = []
for boundary, flag, modes in [
    ('flag-off', '0', ('normal', 'inject-clutch')),
    ('unmarked', '1', ('clear-unmarked', 'inject-unmarked')),
]:
    env['RRR3D_EXPERIMENT_GAMECAR_CLUTCH_V186'] = flag
    runs = []
    for mode in modes:
        path = out / f'{boundary}-{mode}.jsonl'
        args = [str(exe), str(inputs), str(path)]
        if mode != 'normal':
            args.append(mode)
        with path.with_suffix('.log').open('w') as log:
            subprocess.run(args, env=env, stdout=log, stderr=log, check=True, timeout=120)
        rows = [json.loads(s) for s in path.read_text().splitlines()]
        assert len(rows) == 720
        runs.append(rows)
    differences = [a['case'] for a, b in zip(*runs) if a != b]
    reports.append(dict(boundary=boundary, cases=720, exact=720-len(differences), differences=differences))
report = dict(runs=reports, executable_sha256=hashlib.sha256(exe.read_bytes()).hexdigest())
(out / 'comparison.json').write_text(json.dumps(report, indent=2) + '\n')
print([(r['boundary'], r['exact']) for r in reports], flush=True)
assert all(not r['differences'] for r in reports), 'Experimental or provenance boundary leaked'
