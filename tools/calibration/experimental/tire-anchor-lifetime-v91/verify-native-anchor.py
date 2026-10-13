"""Verify arithmetic and lifetime cause from included read-only captures."""
from pathlib import Path
import gzip,json,struct,subprocess,tempfile
base=Path(__file__).resolve().parent
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def records(path):
 with gzip.open(path,'rt') as f:return [json.loads(s) for s in f]
reference=[r['sample'] for r in records(base/'captures/windows-reference.jsonl.gz') if r['event']=='calibration.sample' and r['detail']=='mobility/devildriver/engine3/lateral']
payloads=[];normalized=[]
for rep in range(2):
 prefix=base/f'captures/native-anchor-window/force-devildriver-engine3-lateral-58-{rep}'
 native=records(Path(str(prefix)+'-private.jsonl.gz'));physical=records(Path(str(prefix)+'-physical.jsonl.gz'))
 assert [r['sample'] for r in physical if r['event']=='calibration.sample']==reference
 gates=[r for r in native if r.get('function')=='tire-anchor-gate'];wheelids={r['ebx']:i for i,r in enumerate(gates[:4])};assert len(wheelids)==4
 mac=records(base/f'captures/mac-anchor-control/control-{rep}-anchor.jsonl.gz');lookup={(r['step'],r['wheel']):r for r in mac};rows=[];lifetime=[]
 candidate=records(base/f'captures/mac-anchor-candidate/candidate-{rep}-anchor.jsonl.gz');candidate_lookup={(r['step'],r['wheel']):r for r in candidate}
 for r in gates:
  tick=r['sample_tick_v71'];backend=r['ebx'];wheel=wheelids[backend];w=r['wheel_contact']['words'];ready=[z for z in native if z.get('function')=='tire-anchor-bias-ready' and z.get('ebx')==backend and z.get('sample_tick_v71')==tick];assert len(ready)==1
  updates=[z for z in native if z.get('function')=='tire-anchor-update' and z.get('ebx')==backend and z.get('sample_tick_v71')==tick]
  values=w[:3]+w[22:25]+w[6:9]+w[9:12]+r['wheel_skin']['words'][:1]+w[19:20]+[bool(updates)]+ready[0]['prepare_frame']['words'][63:65];rows.append(' '.join(str(int(v)) for v in values))
  m=lookup[tick,wheel];assert list(map(bits,m['current']))==w[:3];lifetime.append((tick,wheel,w[22:25],m['stored']))
  c=candidate_lookup[tick,wheel];assert list(map(bits,c['current']))==w[:3];assert list(map(bits,c['stored']))==w[22:25];assert bits(c['lateral_error'])==ready[0]['prepare_frame']['words'][63];assert bits(c['longitudinal_error'])==ready[0]['prepare_frame']['words'][64]
 for wheel in range(4):
  assert not lookup[38,wheel]['has_contact'] and lookup[38,wheel]['valid_before'] and not lookup[38,wheel]['valid_after']
  assert lookup[44,wheel]['has_contact'] and not lookup[44,wheel]['valid_before'] and lookup[44,wheel]['valid_after']
  selected=[r for r in lifetime if r[1]==wheel];old=selected[0][2];assert all(r[2]==old for r in selected)
  assert list(map(bits,lookup[44,wheel]['stored']))!=old
 payloads.append(str(len(rows))+'\n'+'\n'.join(rows)+'\n');normalized.append(lifetime)
assert payloads[0]==payloads[1]==(base/'proofs/native-anchor-inputs.txt').read_text();assert normalized[0]==normalized[1]
with tempfile.TemporaryDirectory(prefix='motorrock-v91-anchor-') as t:
 for opt in ['O0','O2']:
  exe=Path(t)/opt;subprocess.run(['clang++','-std=c++17','-'+opt,'-I'+str(base/'proofs'),str(base/'proofs/verify-native-anchor-inputs.cpp'),'-o',str(exe)],check=True)
  result=subprocess.run([str(exe)],input=payloads[0],text=True,capture_output=True,check=True);assert result.stdout.strip()=='136 Native same-input anchors exact';print(opt+': '+result.stdout.strip())
print('1200 Native states unchanged; 136 worldpoints exact; Native anchor persists across loss while old Mac resets. Candidate136storedanchors/errors exactNative. Two repeats exact.')
