from pathlib import Path
import shutil,json,gzip,hashlib
root=Path.cwd();git=root/'work/sync-rrr3d/tools/calibration/experimental';names=['controller-steering-v176','controller-steering-v177','controller-steering-pose-v178','controller-steering-deadband-v179']
for name in names:
 src=root/'work'/name;dst=git/name;dst.mkdir(exist_ok=True)
 for f in src.rglob('*'):
  if not f.is_file() or any(x in f.parts for x in ['__pycache__']) or f.suffix not in ['.py','.cpp','.h','.md','.json','.txt','.cmd','.log']:continue
  target=dst/f.relative_to(src);target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(f,target)
  if target.suffix=='.log':target.write_text(target.read_text())
 evidence=dst/'evidence';evidence.mkdir(exist_ok=True)
 for f in (root/'outputs'/name).rglob('*'):
  if not f.is_file() or 'final-broad' in f.parts or f.suffix not in ['.json','.jsonl','.gz','.words']:continue
  target=evidence/f.relative_to(root/'outputs'/name);target.parent.mkdir(parents=True,exist_ok=True)
  if f.suffix in ['.jsonl','.words']:
   target=Path(str(target)+'.gz');target.write_bytes(gzip.compress(f.read_bytes(),mtime=0))
  else:shutil.copy2(f,target)
 # Full608 traces stay local compressed, rather than duplicating95MB in Git.
 if name=='controller-steering-v177':
  full=root/'outputs'/name/'final-broad';m=json.loads((full/'manifest.json').read_text())
  m['local_trace_receipts']=[dict(file=f.name,bytes=f.stat().st_size,sha256=hashlib.sha256(f.read_bytes()).hexdigest()) for f in sorted(full.glob('*.gz'))];(evidence/'full-broad-manifest.json').write_text(json.dumps(m,indent=2)+'\n')
shutil.copy2(root/'work/controller-motor-v175/NEXT.md',git/'controller-motor-v175/NEXT.md');shutil.copy2(root/'work/scene-composition-v169/PLAN-RU.md',git/'scene-composition-v169/PLAN-RU.md')
print('Owned overlays, probes, compact compressed evidence and plan synchronized; no proprietary SDK or binaries included')
