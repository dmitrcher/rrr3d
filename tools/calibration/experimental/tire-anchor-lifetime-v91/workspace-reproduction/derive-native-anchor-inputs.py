from pathlib import Path
import gzip,json,subprocess,tempfile
root=Path.cwd();directory=root/'work/tire-anchor-v91';payloads=[];normalized=[]
for rep in range(2):
 records=[json.loads(s) for s in gzip.open(root/f'outputs/tire-anchor-v91/native-anchor-window/force-devildriver-engine3-lateral-58-{rep}-private.jsonl.gz','rt')];gates=[r for r in records if r.get('function')=='tire-anchor-gate'];rows=[];norm=[]
 for r in gates:
  tick=r['sample_tick_v71'];backend=r['ebx'];w=r['wheel_contact']['words'];ready=[z for z in records if z.get('function')=='tire-anchor-bias-ready' and z.get('ebx')==backend and z.get('sample_tick_v71')==tick];assert len(ready)==1
  updates=[z for z in records if z.get('function')=='tire-anchor-update' and z.get('ebx')==backend and z.get('sample_tick_v71')==tick]
  values=w[:3]+w[22:25]+w[6:9]+w[9:12]+r['wheel_skin']['words'][:1]+w[19:20]+[bool(updates)]+ready[0]['prepare_frame']['words'][63:65];rows.append(' '.join(str(int(v)) for v in values));norm.append({'tick':tick,'words':values})
 payload=str(len(rows))+'\n'+'\n'.join(rows)+'\n';payloads.append(payload);normalized.append(norm)
assert payloads[0]==payloads[1] and normalized[0]==normalized[1]
(directory/'native-anchor-inputs.txt').write_text(payloads[0]);include=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics');checks=[]
with tempfile.TemporaryDirectory(prefix='motorrock-v91-anchor-') as t:
 for opt in ['O0','O2']:
  exe=Path(t)/opt;subprocess.run(['clang++','-std=c++17','-'+opt,'-I'+str(include),str(directory/'verify-native-anchor-inputs.cpp'),'-o',str(exe)],check=True);result=subprocess.run([str(exe)],input=payloads[0],text=True,capture_output=True,check=True);checks.append({'optimization':opt,'result':result.stdout.strip()});print(result.stdout.strip(),opt)
(root/'outputs/tire-anchor-v91/native-same-input-proof.json').write_text(json.dumps({'scope':'Existing source anchor arithmetic on identical actual Native inputs. Does not certify Mac lifetime inputs.','records':len(normalized[0]),'two_repeats_exact':True,'checks':checks,'goal_complete':False},indent=2)+'\n')
