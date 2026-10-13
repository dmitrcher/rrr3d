"""Reproduce retained V94 focused histories using a supplied compiled probe."""
from pathlib import Path
import argparse, gzip, json, os, struct, subprocess, tempfile

root = Path(__file__).resolve().parent
p = argparse.ArgumentParser()
p.add_argument('--probe', type=Path, required=True)
a = p.parse_args()
names = ['mobility/manticora/wheel3/lateral','wheel-drive','mobility/devildriver/engine3/lateral']
bits = lambda x: struct.unpack('<I', struct.pack('<f', x))[0]
def load(path):
    result = {n: [] for n in names}
    stream = gzip.open(path, 'rt') if str(path).endswith('.gz') else path.open()
    with stream:
        for r in map(json.loads, stream):
            if r['event'] == 'calibration.sample' and r['detail'] in result:
                result[r['detail']].append(r['sample'])
    assert all(len(rows) == 600 for rows in result.values())
    return result
def physical(row):
    row = dict(row)
    for field in ['position', 'velocity', 'angular_momentum', 'rotation']:
        row[field] = list(map(bits, row[field]))
    fields = ['angular_speed', 'height', 'engine_longitudinal_slip', 'engine_lateral_slip', 'suspension_length', 'engine_normal_force', 'longitudinal_impulse', 'lateral_impulse']
    row['wheels'] = [{k: bits(v) if k in fields else v for k, v in w.items()} for w in row['wheels']]
    return row
expected = load(root / 'captures/focused/skins-0.jsonl.gz')
assert expected == load(root / 'captures/focused/skins-1.jsonl.gz')
reference = load(root / 'captures/windows-reference.jsonl.gz')
for name in names:
    assert list(map(physical,expected[name]))==list(map(physical,reference[name]))
env = {k: v for k, v in os.environ.items() if not k.startswith(('RRR3D_', 'NX_'))}
env.update(json.loads((root / 'candidate-environment.json').read_text()))
env['RRR3D_CALIBRATION_PROFILE'] = str(root / 'windows-source-v14.cfg')
assert b'RRR3D_EXPERIMENT_NATIVE_HARD_STOP_SKINS_V94' in a.probe.read_bytes()
with tempfile.TemporaryDirectory(prefix='motorrock-v94-checkpoint-') as tmp:
    capture = Path(tmp) / 'fresh.jsonl'
    subprocess.run([str(a.probe.resolve()), str(capture), 'list:' + ','.join(names)], env=env, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    assert load(capture) == expected
print('1800fresh states exactly match saved V94 repeats; three complete600-step histories exactWindows including corrected Manticora lateral; goal incomplete.')
