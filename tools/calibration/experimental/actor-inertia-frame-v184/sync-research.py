from pathlib import Path
import gzip,json,hashlib,shutil
root=Path(__file__).resolve().parents[2];target=root/'work/sync-rrr3d/tools/calibration/experimental'
stages=['controller-steering-transform-v180','controller-steering-matrix-v181','controller-steering-normalize-v182','controller-steering-direction-v183','actor-inertia-frame-v184']
allowed={'.py','.cpp','.h','.md','.json','.txt','.cmd','.log','.cfg'}
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def copy(p,d):
 d.parent.mkdir(parents=True,exist_ok=True)
 if p.suffix in allowed: d.write_text(p.read_text().replace('\r\n','\n'))
 else: shutil.copy2(p,d)
for stage in stages:
 src=root/'work'/stage;dst=target/stage
 for p in src.rglob('*'):
  if not p.is_file() or '__pycache__' in p.parts or any(x.endswith('.dSYM') for x in p.parts) or p.name=='native-final-histories.json':continue
  if p.suffix in allowed:copy(p,dst/p.relative_to(src))
 folders=list((root/'outputs').glob(stage+'*')) if stage.endswith('v180') else [root/'outputs'/stage]
 omitted=[]
 for folder in folders:
  for p in folder.rglob('*'):
   if not p.is_file():continue
   rel=p.relative_to(folder)
   if any(x.startswith('MotorRock-Mac-') for x in rel.parts) or p.suffix=='.zip':continue
   if p.name.startswith('candidate-run') or p.stat().st_size>2000000:
    omitted.append(dict(path=str(p.relative_to(root)),bytes=p.stat().st_size,sha256=sha(p)));continue
   dest=dst/'evidence'/folder.name/rel
   if p.suffix in {'.jsonl','.words'}:
    dest=dest.with_suffix(dest.suffix+'.gz');dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(gzip.compress(p.read_bytes(),mtime=0))
   elif p.suffix in allowed|{'.gz'}:copy(p,dest)
 if omitted:(dst/'local-large-evidence.json').write_text(json.dumps(omitted,indent=2)+'\n')
for rel in ['controller-motor-v175/NEXT.md','controller-steering-deadband-v179/NEXT.md','scene-composition-v169/PLAN-RU.md']:copy(root/'work'/rel,target/rel)
print('Copied five research stages and compact evidence; excluded compiled Windows files, archives, dSYM and full-broad trace bodies.')
