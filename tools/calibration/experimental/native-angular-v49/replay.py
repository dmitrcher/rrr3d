"""Fresh two-repeat controls/candidates against the bundled native and Mac histories."""
import argparse,json,os,subprocess,sys,hashlib
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--package',type=Path,required=True);p.add_argument('--out',type=Path,required=True);a=p.parse_args();root=a.package.resolve();a.out.mkdir(exist_ok=False);here=Path(__file__).resolve().parent
sys.dont_write_bytecode=True;sys.path.insert(0,str(here.parents[1]));import compare
probe=root/'bin/Mac/SpringProbe';profile=root/'source/tools/calibration/profiles/windows-source-v14.cfg';meta=json.loads((root/'measurements/gates/capture-manifest.json').read_text());base={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};base.update(meta['environment']);base['RRR3D_CALIBRATION_PROFILE']=str(profile);result={}
for scope in ['native24','long72','old6','isolation00','isolation01','isolation02','isolation03']:
 names=list(compare.load(root/'measurements/references'/f'{scope}-windows.jsonl','windows-physx-d3d9'))
 for mode in ['control','candidate']:
  env=dict(base)
  for flag in ['RRR3D_EXPERIMENT_NATIVE_ANGULAR_INTEGRATION','RRR3D_EXPERIMENT_NATIVE_PUBLIC_ANGULAR_MOMENTUM','RRR3D_EXPERIMENT_NATIVE_DYNAMIC_ACTOR_OUTPUT']:
   if mode=='candidate':env[flag]='1'
   else:env.pop(flag,None)
  runs=[]
  for repeat in range(2):
   dest=a.out/f'{scope}-{mode}-run{repeat}-mac.jsonl';subprocess.run([str(probe),str(dest.resolve()),'list:'+','.join(names)],env=env,check=True,stdout=subprocess.DEVNULL,timeout=180);got=compare.load(dest,'mac-jolt-metal');expected=compare.load(root/'measurements/gates'/dest.name,'mac-jolt-metal');
   for name in names:
    assert got[name]['meta']['profile']==str(profile)+'\n'+profile.read_text()
    assert got[name]['meta']['profile'].partition('\n')[2]==expected[name]['meta']['profile'].partition('\n')[2]
    # Only the absolute provenance path differs after archive relocation.
    got[name]['meta']['profile']=expected[name]['meta']['profile']
   assert got==expected,(scope,mode,repeat);runs.append(got)
  assert runs[0]==runs[1];result[scope+'-'+mode]={'cases':len(names),'repeats':2,'matches_original_capture':True};print(scope,mode,len(names),'matched',flush=True)
(a.out/'replay-result.json').write_text(json.dumps({'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'results':result,'goal_complete':False},indent=2)+'\n')
