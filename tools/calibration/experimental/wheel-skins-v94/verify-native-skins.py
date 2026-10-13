"""Bind actual DLL captures and reproduce the separate skin arithmetic."""
from pathlib import Path
import gzip, json, struct, subprocess, tempfile

root = Path(__file__).resolve().parent
fixtures = json.loads((root / 'proofs/skin-fixtures-bound.json').read_text())
bits = lambda x: struct.unpack('<I', struct.pack('<f', x))[0]

def physical(row):
    row = dict(row)
    for field in ['position', 'velocity', 'angular_momentum', 'rotation']:
        row[field] = list(map(bits, row[field]))
    fields = {'angular_speed', 'height', 'engine_longitudinal_slip', 'engine_lateral_slip', 'suspension_length', 'engine_normal_force', 'longitudinal_impulse', 'lateral_impulse'}
    row['wheels'] = [{k: bits(v) if k in fields else v for k, v in w.items()} for w in row['wheels']]
    return row

reference = {}
with gzip.open(root / 'captures/windows-reference.jsonl.gz', 'rt') as stream:
    for r in map(json.loads, stream):
        if r['event'] == 'calibration.sample':
            reference.setdefault(r['detail'], []).append(physical(r['sample']))
preserved = 0
for path in (root / 'captures/native-stop-prep').glob('*-physical.jsonl.gz'):
    with gzip.open(path, 'rt') as stream:
        rows = [r for r in map(json.loads, stream) if r['event'] == 'calibration.sample']
    assert len(rows) == 600
    assert [physical(r['sample']) for r in rows] == reference[rows[0]['detail']]
    preserved += len(rows)
assert preserved == 4800

assert len(fixtures) == 2072
assert sum(f['kind'] == 'actual-native' for f in fixtures) == 24
for index, tick in [(0, 22), (1, 22), (2, 23)]:
    for rep in range(2):
        with gzip.open(root / f'captures/native-stop-prep/{index}-{tick}-{rep}-private.jsonl.gz', 'rt') as stream:
            native = list(map(json.loads, stream))
        prepares = [r for r in native if r.get('function') == 'wheel-spring-prepare']
        hits = {r['hit_shape0']['address']: r for r in native if r.get('function') == 'wheel-hit-dispatch'}
        errors = [r['spring_object']['words'][24] for r in native if r.get('function') == 'spring-row-entry'][:4]
        selected = [f for f in fixtures if f['kind'] == 'actual-native' and f['binding']['case_index'] == index and f['binding']['repeat'] == rep]
        assert len(prepares) == len(selected) == len(errors) == 4
        assert sorted(f['expected_native'] for f in selected) == sorted(errors)
        for f, p in zip(selected, prepares):
            hit = hits[p['wheel_backend']['address']]
            assert hit['hit_shape1']['address'] == p['wheel_tail']['words'][1]
            inputs = [p['wheel_tail']['words'][9], p['wheel_tail']['words'][40], hit['hit_shape1']['words'][46], hit['hit_shape0']['words'][46]]
            assert inputs == f['input']
            assert f['expected_native'] == f['expected_sse_word']

expected = [f['expected_sse_word'] for f in fixtures]
for opt in ['O0', 'O2']:
    assert list(map(int, (root / f'proofs/windows-sse-{opt}.txt').read_text().split())) == expected
with tempfile.TemporaryDirectory(prefix='motorrock-v94-skins-') as tmp:
    for opt in ['O0', 'O2']:
        binary = Path(tmp) / opt
        subprocess.run(['clang++', '-std=c++17', '-' + opt, '-ffp-contract=off', '-I' + str(root / 'proofs'), str(root / 'proofs/verify-skin.cpp'), '-o', str(binary)], check=True)
        result = json.loads(subprocess.check_output([str(binary)], text=True))
        assert result == {'exact': 2072, 'grouped_errors': 469, 'actual_grouped_errors': 4}
        print(opt, result)
print('4800 actual Native states preserve Golden; 24 DLL preparations bound to both actual shape skins; 2048 additional Windows SSE arithmetic inputs exact. Goal incomplete.')
