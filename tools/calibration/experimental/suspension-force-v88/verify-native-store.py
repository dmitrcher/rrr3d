from pathlib import Path
import gzip,json,struct,subprocess,tempfile,shutil
root=Path(__file__).resolve().parent
kernel=next((root/'captures/native-kernel30').glob('*0-private*.gz'))
callback=next((root/'captures/native-callback31').glob('*0-private*.gz'))
with gzip.open(kernel,'rt') as f:rows=[r for r in map(json.loads,f) if r.get('function')=='spring-solve-applied']
with gzip.open(callback,'rt') as f:forces=[r for r in map(json.loads,f) if r.get('function')=='callback-entry']
assert len(rows)==60 and len(forces)==4
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
fixtures=json.loads((root/'proofs/native-store-fixtures.json').read_text())['fixtures']
for wheel,(entry,force) in enumerate(zip(fixtures,forces)):
 assert entry['wheel']==wheel and entry['impulse']==rows[48+3-wheel]['solve_row']['words'][28]
 assert entry['force']==force['callback_input_force']['words'][0] and entry['step']==bits(.01666666753590107)
with tempfile.TemporaryDirectory(prefix='motorrock-v88-native-') as t:
 tmp=Path(t)
 for p in (root/'proofs').iterdir():shutil.copy2(p,tmp/p.name)
 for opt in ('O0','O2'):
  exe=tmp/('verify-'+opt)
  subprocess.run(['clang++','-std=c++17','-ffp-contract=off','-'+opt,'-I'+str(tmp),str(tmp/'verify-store.cpp'),'-o',str(exe)],check=True)
  subprocess.run([str(exe)],check=True)
print('Actual Native4force stores derived from immutable observer captures; O0/O2 exact. Full physics goal incomplete.')
