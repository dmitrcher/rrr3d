"""Explicit publication allowlist: never recurse into prefix/installers/toolchain."""
from pathlib import Path
import gzip,hashlib,json,shutil
R=Path.cwd();W=R/'work/crossover-reference-v188';O=R/'outputs/crossover-reference-v188';D=R/'work/sync-rrr3d/tools/calibration/experimental/crossover-reference-v188';D.mkdir(exist_ok=True)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();copied=[];omitted=[]
# Only root-level owned scripts, documentation and provenance JSON.
for p in sorted(W.iterdir()):
 if p.is_file() and p.suffix in {'.py','.md','.json','.manifest'}:
  assert p.stat().st_size<2000000,p
  dst=D/p.name;shutil.copy2(p,dst);copied.append(str(dst.relative_to(D)))
# Numerical evidence and archived loader logs only; omit large full SDK trajectories.
for p in sorted(O.rglob('*')):
 if not p.is_file():continue
 rel=p.relative_to(O)
 if p.suffix not in {'.json','.gz','.log'}:raise RuntimeError(f'Unexpected output type: {p}')
 if p.stat().st_size>2000000:
  omitted.append(dict(path=str(p.relative_to(R)),bytes=p.stat().st_size,sha256=sha(p)));continue
 dst=D/'evidence'/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,dst);copied.append(str(dst.relative_to(D)))
# Installer extraction metadata only; no binary/manifest payload from Microsoft.
p=W/'installer-crt/provenance.json'
if p.exists():dst=D/'dependency-extraction-provenance.json';shutil.copy2(p,dst);copied.append(dst.name)
(D/'local-large-evidence.json').write_text(json.dumps(omitted,indent=2)+'\n')
assert not any(x.suffix.lower() in {'.dll','.exe','.pdb','.cab','.iso','.lib'} for x in D.rglob('*') if x.is_file())
print(len(copied),'owned source/provenance/evidence files;',len(omitted),'large local captures indexed')
