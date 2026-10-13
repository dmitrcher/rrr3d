"""Verify captured Native inputs and independently measured SSE/x87 kernels."""
from pathlib import Path
import gzip, json, struct, subprocess, tempfile
root = Path(__file__).resolve().parent
bits = lambda x: struct.unpack('<I', struct.pack('<f', x))[0]
fixtures = json.loads((root/'proofs/combined-fixtures-bound.json').read_text())
assert len(fixtures) == 2056
assert sum(r['kind'] == 'actual-native' for r in fixtures) == 8
expected = [r['expected_x87'] for r in fixtures]
for opt in ['O0','O2']:
    assert [list(map(int, line.split())) for line in (root/f'proofs/windows-x87-{opt}.txt').read_text().splitlines()] == expected
for rep in range(2):
    path = next((root/'captures/native-force245').glob(f'*-{rep}-private.jsonl.gz'))
    native = list(map(json.loads, gzip.open(path,'rt')))
    functions = [r for r in native if 'function' in r]
    assert all(r['sample_tick_v71'] == 245 and r['x87_control_word_v95'] == 895 and r['mxcsr_v95'] == 8064 for r in functions)
    groups = [[r for r in native if r.get('function') == name] for name in ['tire-lateral-hermite-result','tire-longitudinal-hermite-result','tire-combined-curves']]
    for wheel,(lat,lon,post) in enumerate(zip(*groups)):
        params = lon['wheel_parameters']['words']
        inputs = [lon['xmm_words'][0][0] ^ 0x80000000, lat['xmm_words'][0][0] ^ 0x80000000, params[7], params[13]]
        outputs = [post['wheel_contact']['words'][18], post['stack']['words'][26], post['stack']['words'][6]]
        item = fixtures[rep*4+wheel]
        assert item['kind'] == 'actual-native' and inputs == item['input'] and outputs == item['expected_native'] == item['expected_x87']
def physical(row):
    row = dict(row)
    for field in ['position','velocity','angular_momentum','rotation']: row[field] = list(map(bits,row[field]))
    fields = {'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'}
    row['wheels'] = [{k:bits(v) if k in fields else v for k,v in w.items()} for w in row['wheels']]
    return row
reference = {}
with gzip.open(root/'captures/windows-reference.jsonl.gz','rt') as stream:
    for r in map(json.loads,stream):
        if r['event'] == 'calibration.sample': reference.setdefault(r['detail'],[]).append(physical(r['sample']))
preserved = 0
for folder in ['native-force245','native-kernel245']:
    for path in (root/'captures'/folder).glob('*-physical.jsonl.gz'):
        with gzip.open(path,'rt') as stream: rows = [r for r in map(json.loads,stream) if r['event'] == 'calibration.sample']
        assert len(rows) == 600 and [physical(r['sample']) for r in rows] == reference[rows[0]['detail']]
        preserved += len(rows)
assert preserved == 2400
with tempfile.TemporaryDirectory(prefix='motorrock-v95-combined-') as tmp:
    for opt in ['O0','O2']:
        binary = Path(tmp)/opt
        subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(root/'proofs'),str(root/'proofs/verify-combined.cpp'),'-o',str(binary)],check=True)
        result = json.loads(subprocess.check_output([str(binary)],text=True))
        assert result == {'exact':2056,'old_independent_errors':1304,'normalized_inputs':1304}
        print(opt,result)
print('2400 actual Native states preserve Golden;8 actual coupled preparations and2048 additional Windows SSE/x87 arithmetic inputs verified. Universal unmeasured x87 precision parity is not established; goal incomplete.')
