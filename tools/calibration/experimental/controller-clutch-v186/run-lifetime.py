"""Persistent controller; reseed physical body to independently recorded inputs each tick.

This proves command consumption, not an uninterrupted Windows race or native timer duration.
"""
from pathlib import Path
import collections
import gzip
import hashlib
import json
import os
import subprocess

root = Path.cwd()
out = root / 'outputs/controller-clutch-v186/lifetime-reseed'
out.mkdir(exist_ok=True)
native = {r['case']: r for r in map(json.loads, gzip.open(root / 'outputs/controller-stabilize-v185/accepted-native0.jsonl.gz', 'rt')) if r['event'] == 'stabilize.pose' and not r['contact']}
env = {k: v for k, v in os.environ.items() if not k.startswith(('RRR3D_', 'NX_'))}
env.update(json.loads((root / 'outputs/controller-clutch-v186/world-environment.json').read_text()))
exe = root / 'work/controller-clutch-v186/SourceClutchLifetimeProbe'
previous = None
reports = []
for rep in range(2):
    trace = out / f'run{rep}.stabilize.jsonl'
    trace.unlink(missing_ok=True)
    env['RRR3D_STABILIZE_TRACE_FILE_V113'] = str(trace)
    path = out / f'run{rep}.jsonl'
    with path.with_suffix('.log').open('w') as log:
        subprocess.run([str(exe), str(root / 'outputs/controller-clutch-v186/world-input.words'), str(path)], env=env, stdout=log, stderr=log, check=True, timeout=180)
    rows = list(map(json.loads, path.read_text().splitlines()))
    traces = list(map(json.loads, trace.read_text().splitlines()))
    assert len(rows) == 720 * 26 and len(traces) == len(rows) * 4
    differences = []
    cz = lambda a: [0 if x == 0x80000000 else x for x in a]
    release_ticks = collections.Counter()
    for i, row in enumerate(rows):
        seed = native[row['case']]
        tick = row['tick']
        assert tick == i % 26 and row['callbacks'] == tick + 1
        # Independent original rows: request on first tick, zero strength while
        # locked afterwards, unlocked row after the Source timer releases.
        mode = seed['clutch_case'] if tick == 0 else (1 if row['clutch_locked_after'] else 0)
        expect = native[(row['case'] // 4) * 4 + mode]
        applied = traces[i * 4 + 3]
        incoming = traces[i * 4 + 1]
        assert incoming['phase'] == 'game-input-v185' and applied['phase'] == 'applied'
        observed = {k: row[k] for k in ('rotation_after_step', 'momentum_after_step', 'position_after_step')}
        observed.update(rotation_after=applied['words'][:4], omega_after=applied['words'][7:10], momentum_before=incoming['words'][10:13])
        fields = [k for k, v in observed.items() if cz(v) != cz(expect[k])]
        if fields:
            differences.append(dict(case=row['case'], tick=tick, fields=fields))
        if seed['clutch_case'] and not row['clutch_locked_after']:
            release_ticks[tick] += 1
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    if previous is not None:
        assert digest == previous, 'Persistent-controller repetitions differ'
    previous = digest
    reports.append(dict(repeat=rep, states=len(rows), exact=len(rows)-len(differences), differences=differences, source_unlocked_ticks=dict(release_ticks), sha256=digest))
    print(rep, reports[-1]['exact'], '/', len(rows), flush=True)
report = dict(scope=__doc__.strip(), runs=reports, repeats_identical=True, native_timer_duration_proven=False, uninterrupted_native_sequence_proven=False, executable_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(), goal_complete=False)
(out / 'comparison.json').write_text(json.dumps(report, indent=2) + '\n')
assert all(not r['differences'] for r in reports), 'Persistent controller differs from recorded per-tick stabilization oracle'
