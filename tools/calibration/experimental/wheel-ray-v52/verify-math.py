"""Replay actual original Windows ray inputs against the independent C++ helper."""
from pathlib import Path
import json, subprocess, tempfile
here=Path(__file__).resolve().parent
cases=json.loads((here/'native-inputs.json').read_text())['cases']
assert len(cases)==24 and len({c['case'] for c in cases})==7
inputs='\n'.join(' '.join(map(str,sum(c['vertices'],[])+c['ray']+c['intersection'])) for c in cases)+'\n'
expected=[c['intersection']+c['point']+c['point'] for c in cases]
with tempfile.TemporaryDirectory() as td:
    for opt in ['-O0','-O2']:
        binary=Path(td)/'verify'
        subprocess.run(['clang++','-std=c++17','-ffp-contract=off',opt,str(here/'verify-wheel-ray.cpp'),'-o',str(binary)],check=True)
        result=subprocess.run([str(binary)],input=inputs,text=True,capture_output=True,check=True)
        actual=[list(map(int,line.split())) for line in result.stdout.splitlines()]
        assert actual==expected,(opt,[(c['case'],a,e) for c,a,e in zip(cases,actual,expected) if a!=e])
        print(opt, '24 original rays / 216 float32 words exactly matched')
