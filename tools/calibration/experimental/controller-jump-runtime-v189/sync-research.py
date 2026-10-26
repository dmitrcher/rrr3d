from pathlib import Path
import gzip,hashlib,json,shutil
R=Path.cwd();W=R/'work/controller-jump-runtime-v189';O=R/'outputs/controller-jump-runtime-v189';D=R/'work/sync-rrr3d/tools/calibration/experimental/controller-jump-runtime-v189';D.mkdir(exist_ok=True);allowed={'.py','.cpp','.h','.md','.json','.cfg','.txt','.cmd'};large=[]
for p in W.rglob('*'):
 if not p.is_file() or any(x.endswith('.dSYM') or x=='__pycache__' for x in p.parts):continue
 if p.suffix not in allowed|{'.log'}:continue
 dest=D/p.relative_to(W)
 if p.suffix=='.log':dest=dest.with_suffix('.log.gz')
 dest.parent.mkdir(parents=True,exist_ok=True)
 if p.suffix=='.log':dest.write_bytes(gzip.compress(p.read_bytes(),mtime=0))
 else:shutil.copy2(p,dest)
for p in O.rglob('*'):
 if not p.is_file():continue
 rel=p.relative_to(O)
 if any(x.startswith('MotorRock-Mac-') for x in rel.parts) or p.suffix=='.zip':continue
 if 'package-verification' in rel.parts and p.name!='report.json':continue
 if p.suffix=='.jsonl':continue # Existing lossless .gz captures are the publication form.
 if p.suffix not in allowed|{'.gz','.words'}:continue
 if p.stat().st_size>2000000:large.append(dict(path=str(p.relative_to(R)),bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()));continue
 dest=D/'evidence'/rel;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,dest)
(D/'local-large-evidence.json').write_text(json.dumps(large,indent=2)+'\n');assert not any(p.suffix.lower() in {'.dll','.exe','.pdb','.lib','.cab','.iso'} for p in D.rglob('*') if p.is_file());print('Owned source and numerical evidence copied; proprietary files/build artifacts excluded')
