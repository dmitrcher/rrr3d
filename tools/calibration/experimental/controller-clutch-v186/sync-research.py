from pathlib import Path
import shutil,gzip,hashlib,json
root=Path.cwd();target=root/'work/sync-rrr3d/tools/calibration/experimental';allowed={'.py','.cpp','.h','.md','.json','.txt','.cmd','.log','.cfg'};sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
for stage in ['controller-clutch-v186']:
 src=root/'work'/stage;dst=target/stage
 for f in src.rglob('*'):
  if not f.is_file() or '__pycache__' in f.parts or any(p.endswith('.dSYM') for p in f.parts) or f.suffix not in allowed:continue
  d=dst/f.relative_to(src);d.parent.mkdir(parents=True,exist_ok=True);d.write_text((f.read_text().replace('\r\n','\n').expandtabs(4) if f.suffix=='.log' else f.read_text().replace('\r\n','\n')))
 out=root/'outputs'/stage;omitted=[]
 for f in out.rglob('*'):
  if not f.is_file():continue
  rel=f.relative_to(out)
  if any(p.startswith('MotorRock-Mac-') for p in rel.parts) or f.suffix=='.zip':continue
  if f.name.startswith('candidate-run') or f.stat().st_size>2000000:
   omitted.append(dict(path=str(f.relative_to(root)),bytes=f.stat().st_size,sha256=sha(f)))
   continue
  d=dst/'evidence'/rel
  if f.suffix in {'.jsonl','.words'}:d=d.with_suffix(d.suffix+'.gz');d.parent.mkdir(parents=True,exist_ok=True);d.write_bytes(gzip.compress(f.read_bytes(),mtime=0))
  elif f.suffix in allowed|{'.gz'}:
   d.parent.mkdir(parents=True,exist_ok=True)
   if f.suffix in allowed:d.write_text((f.read_text().replace('\r\n','\n').expandtabs(4) if f.suffix=='.log' else f.read_text().replace('\r\n','\n')))
   else:shutil.copy2(f,d)
 if omitted:(dst/'local-large-evidence.json').write_text(json.dumps(omitted,indent=2)+'\n')
print('Research source and compact evidence copied. Full trace bodies and binaries excluded.')
