"""Recheck immutable evidence; optionally rerun the tested four cases from any cwd."""
from pathlib import Path
import argparse, gzip, hashlib, json, math, struct, subprocess, sys, tempfile

p = argparse.ArgumentParser()
p.add_argument('--probe', type=Path)
args = p.parse_args()
here = Path(__file__).resolve().parent
bits = lambda v: struct.unpack('<I', struct.pack('<f', v))[0]
def physical(row):
    row = dict(row)
    for field in ['position', 'velocity', 'rotation', 'angular_momentum']:
        row[field] = list(map(bits, row[field]))
    fields = ['angular_speed', 'height', 'engine_longitudinal_slip', 'engine_lateral_slip',
              'suspension_length', 'engine_normal_force', 'longitudinal_impulse', 'lateral_impulse']
    row['wheels'] = [{k: bits(v) if k in fields else v for k, v in w.items()} for w in row['wheels']]
    return row
def load(path):
    result = {}
    with (gzip.open(path, 'rt') if path.suffix == '.gz' else path.open()) as stream:
        for r in map(json.loads, stream):
            if r['event'] == 'calibration.begin':
                assert r['detail'] not in result
                result[r['detail']] = []
            elif r['event'] == 'calibration.sample':
                rows = result[r['detail']]
                assert r['sample']['tick'] == len(rows)
                rows.append(r['sample'])
            elif r['event'] == 'calibration.end':
                assert len(result[r['detail']]) == 300
    assert len(result) == 4 and all(len(v) == 300 for v in result.values())
    return result

manifest = json.loads((here/'source-manifest.json').read_text())
for f in manifest['files']:
    assert hashlib.sha256((here/f['path']).read_bytes()).hexdigest() == f['sha256'], f['path']
reference = load(here/'captures/windows-reference.jsonl.gz')
candidate = None
for mode in ['control', 'candidate']:
    runs = [load(here/f'captures/{mode}-{i}.jsonl.gz') for i in range(2)]
    assert runs[0] == runs[1]
    for name, rows in runs[0].items():
        dif = [a['tick'] for a,b in zip(reference[name],rows) if physical(a) != physical(b)]
        expected = (None if mode == 'candidate' and name.endswith('-0') else 29) if '-pole-' in name else None
        assert (dif[0] if dif else None) == expected, (mode,name,dif[:1])
    if mode == 'candidate': candidate = runs[0]
assert candidate is not None
if args.probe:
    with tempfile.TemporaryDirectory(prefix='motorrock-v80-verification-') as d:
        output = Path(d)/'fresh.jsonl'
        subprocess.run([sys.executable, str(here/'run-probe.py'), '--probe', str(args.probe.resolve()),
                        '--out', str(output), '--cases', 'list:'+','.join(candidate)], check=True)
        assert load(output) == candidate, 'Fresh execution differs from verified checkpoint'
print('SHA256 manifest exact; 1200-state control/candidate repeats exact; pole0 trajectory exact; pole1 remaining onset29; optional fresh probe exact.')
