"""Portable physical reference, factorial mechanism, and fresh probe verification."""
from pathlib import Path
import argparse,gzip,hashlib,json,struct,subprocess,sys,tempfile
here=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args()
def load(path):
 result={}
 with (gzip.open(path,'rt')if path.suffix=='.gz'else path.open())as f:
  for r in map(json.loads,f):
   if r['event']=='calibration.begin':assert r['detail']not in result;result[r['detail']]=[]
   elif r['event']=='calibration.sample':a=result[r['detail']];assert r['sample']['tick']==len(a);a.append(r['sample'])
 assert set(result)=={'settle','sleep-settle'}and all(len(v)==600 for v in result.values());return result
for f in json.load(open(here/'source-manifest.json'))['files']:assert hashlib.sha256((here/f['path']).read_bytes()).hexdigest()==f['sha256'],f['path']
reference=load(here/'captures/windows-reference.jsonl.gz');runs={m:[load(here/f'captures/{m}-{i}.jsonl.gz')for i in range(2)]for m in ['control','wake','dormant','combined']};assert all(v[0]==v[1]for v in runs.values());assert reference['settle']==reference['sleep-settle']
assert runs['wake'][0]==runs['combined'][0];assert runs['wake'][0]['sleep-settle']==runs['control'][0]['settle'];assert all(v[0]['settle']==runs['control'][0]['settle']for v in runs.values())
for mode in runs:
 a=[json.loads(l)for l in gzip.open(here/f'captures/{mode}-0-sleep.jsonl.gz','rt')];b=[json.loads(l)for l in gzip.open(here/f'captures/{mode}-1-sleep.jsonl.gz','rt')];assert a==b and len(a)==3600
 asleep=[x for x in a if x['allow_sleep']and not x['active']]
 if mode in ['wake','combined']:assert not asleep
 if mode=='dormant':assert asleep and all(x['velocity']==[0,0,0]and x['angular']==[0,0,0]for x in asleep)
 if mode=='control':
  x=[x for x in a if x['allow_sleep']and x['phase']=='after-update-before-restore'];z=[x for x in a if x['allow_sleep']and x['phase']=='after-restore'];assert next(i for i,v in enumerate(x)if not v['active'])==48;assert x[48]['velocity']==[0,0,0]and z[48]['velocity'][2]==.250097811 and not z[48]['active']
if args.probe:
 with tempfile.TemporaryDirectory(prefix='motorrock-v83-checkpoint-')as d:
  out=Path(d)/'fresh.jsonl';subprocess.run([sys.executable,str(here/'run-probe.py'),'--probe',str(args.probe.resolve()),'--out',str(out),'--cases','list:settle,sleep-settle'],check=True);assert load(out)==runs['combined'][0]
print('SHA256 manifests; four isolated modes, repeats and1200-state fresh probe exact. Sleep allowed, zero-force autowake and dormant velocity correction proven; full parity incomplete.')
