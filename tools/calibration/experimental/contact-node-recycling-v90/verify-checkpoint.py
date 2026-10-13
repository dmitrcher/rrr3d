"""Reproduce retained V90 focused histories using a supplied compiled probe."""
from pathlib import Path
import argparse, gzip, json, os, struct, subprocess, tempfile

root = Path(__file__).resolve().parent
p = argparse.ArgumentParser()
p.add_argument('--probe', type=Path, required=True)
a = p.parse_args()
names = ['settle', 'mobility/devildriver/engine3/lateral']
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
expected = load(root / 'captures/force-0.jsonl.gz')
assert expected == load(root / 'captures/force-1.jsonl.gz')
reference = load(root / 'captures/windows-reference.jsonl.gz')
assert [physical(r) for r in expected[names[1]][:58]] == [physical(r) for r in reference[names[1]][:58]]
assert physical(expected[names[1]][58]) != physical(reference[names[1]][58])
env = {k: v for k, v in os.environ.items() if not k.startswith(('RRR3D_', 'NX_'))}
env.update(json.loads((root / 'candidate-environment.json').read_text()))
env['RRR3D_CALIBRATION_PROFILE'] = str(root / 'windows-source-v14.cfg')
assert b'RRR3D_EXPERIMENT_NATIVE_STATIC_CONTACT_NODES_V90' in a.probe.read_bytes()
with tempfile.TemporaryDirectory(prefix='motorrock-v90-checkpoint-') as tmp:
    capture = Path(tmp) / 'fresh.jsonl'
    subprocess.run([str(a.probe.resolve()), str(capture), 'list:' + ','.join(names)], env=env, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    assert load(capture) == expected
print('1200 fresh Mac states exactly match verified V90 captures; stock settle600states all exact; high-load physical0 through57 exact, first remaining difference58; goal incomplete.')
