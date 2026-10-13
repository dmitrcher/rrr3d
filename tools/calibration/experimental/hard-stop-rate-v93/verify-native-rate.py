"""Verify original preparation fixtures and exact rate math at O0/O2."""
from pathlib import Path
import gzip, json, struct, subprocess, tempfile
root=Path(__file__).resolve().parent
fixtures=json.loads((root/'proofs/native-rate-fixtures.json').read_text())
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
value=lambda x:struct.unpack('<f',struct.pack('<I',x))[0]
def physical(row):
    row=dict(row)
    for field in ['position','velocity','angular_momentum','rotation']:
        row[field]=list(map(bits,row[field]))
    floats={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'}
    row['wheels']=[{k:bits(v) if k in floats else v for k,v in w.items()} for w in row['wheels']]
    return row
references={}
with gzip.open(root/'captures/windows-reference.jsonl.gz','rt') as stream:
    for r in map(json.loads,stream):
        if r.get('event')=='calibration.sample':references.setdefault(r['detail'],[]).append(physical(r['sample']))
preserved=0
for folder in ['native-kernel','native-stop-prep']:
    for path in (root/'captures'/folder).glob('*-physical.jsonl.gz'):
        with gzip.open(path,'rt') as stream:
            rows=[r for r in map(json.loads,stream) if r.get('event')=='calibration.sample']
        assert len(rows)==600 and all(r['detail']==rows[0]['detail'] for r in rows)
        assert [physical(r['sample']) for r in rows]==references[rows[0]['detail']]
        preserved+=len(rows)
assert preserved==7200
for index,tick in [(0,22),(1,22),(2,23)]:
    rows=[r for r in fixtures if r['case_index']==index]
    assert len(rows)==4
    for rep in range(2):
        with gzip.open(root/f'captures/native-stop-prep/{index}-{tick}-{rep}-private.jsonl.gz','rt') as stream:
            native=list(map(json.loads,stream))
        objects=[r for r in native if r.get('function')=='spring-row-entry'][:4]
        prepared=[r for r in native if r.get('function')=='spring-row-ready'][:4]
        for fixture,o,p in zip(rows,objects,prepared):
            assert fixture['error']==o['spring_object']['words'][24]
            assert fixture['dt']==o['stack']['words'][2]
            assert fixture['rate']==p['constraint_row']['words'][7]
            assert fixture['mass']==p['constraint_row']['words'][11]
            assert fixture['position_mass']==p['constraint_row']['words'][15]
            inverse=o['spring_context']['words'][16]
            assert inverse==bits(1/value(fixture['dt']))
            assert fixture['rate']==bits(value(fixture['error'])*value(inverse))
with tempfile.TemporaryDirectory(prefix='motorrock-v93-rate-') as tmp:
    for opt in ['O0','O2']:
        binary=Path(tmp)/opt
        subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(root/'proofs'),str(root/'proofs/verify-rate.cpp'),'-o',str(binary)],check=True)
        result=json.loads(subprocess.check_output([str(binary)],text=True))
        assert result=={'exact':12,'old_division_errors':4}
        print(opt,result)
print('7200 Native observable states preserve Golden float32 words; 12 fixtures bound to both repeats; rate/position terms exact; old division differs on4')
