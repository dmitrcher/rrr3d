"""Archive only completed task-owned numerical controls and loader logs."""
from pathlib import Path
import gzip,hashlib,json
R=Path.cwd();W=R/'work/crossover-reference-v188';O=R/'outputs/crossover-reference-v188/controls';O.mkdir(exist_ok=True)
sha=lambda b:hashlib.sha256(b).hexdigest();items=[]
for sub in ['game-debug','sdk-reference']:
 for p in sorted((W/'prefix/drive_c/MotorRock'/sub).glob('*.jsonl')):
  if p.name.startswith('controls'):continue
  raw=p.read_bytes();rows=[json.loads(s) for s in raw.splitlines()];expected=1440 if sub=='game-debug' else 900
  event='stabilize.pose' if sub=='game-debug' else 'world.sample';assert sum(x.get('event')==event for x in rows)==expected,p
  dest=O/f'{p.stem}.jsonl.gz';data=gzip.compress(raw,mtime=0);dest.write_bytes(data);assert gzip.decompress(data)==raw
  items.append(dict(source=str(p.relative_to(R)),archive=str(dest.relative_to(R)),raw_bytes=len(raw),raw_sha256=sha(raw),archive_bytes=len(data),archive_sha256=sha(data),states=expected,event=event))
for p in sorted(W.glob('*.log*')):
 if p.name.endswith('.gz'):raw=gzip.decompress(p.read_bytes())
 elif p.suffix=='.log':raw=p.read_bytes()
 else:continue
 if p.name.startswith('run-') or len(raw)<1000:continue
 dest=O/f'{p.name.removesuffix(".gz")}.gz';data=gzip.compress(raw,mtime=0);dest.write_bytes(data)
 items.append(dict(source=str(p.relative_to(R)),archive=str(dest.relative_to(R)),raw_bytes=len(raw),raw_sha256=sha(raw),archive_bytes=len(data),archive_sha256=sha(data),type='loader_or_process_log'))
(O/'manifest.json').write_text(json.dumps(dict(scope='Task-owned V188 captures only, original PE/DLL/CRT/SDK binaries excluded. Historical captures not deleted.',files=items),indent=2)+'\n');print(len(items),'files archived',sum(x['raw_bytes'] for x in items),'raw bytes;',sum(x['archive_bytes'] for x in items),'compressed bytes')
