from pathlib import Path
import shutil,json,gzip
root=Path.cwd();git=root/'work/sync-rrr3d/tools/calibration/experimental'
for name in ['cold-manager-v162','actor-reactivation-v163','plane-broadphase-v164','pair-publication-v165','pair-retirement-v166','wake-followup-v167','live-manager-v168','scene-composition-v169','controller-audit-v170']:
 src=root/'work'/name;dst=git/name;dst.mkdir(parents=True,exist_ok=True)
 for f in src.rglob('*'):
  if not f.is_file() or '__pycache__' in f.parts or f.suffix not in ['.py','.cpp','.h','.md','.json','.txt','.cmd','.log','.gz']:continue
  target=dst/f.relative_to(src);target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(f,target)
  if target.suffix=='.log':target.write_text(target.read_text())
for name,version in [('scene-composition-v169',169)]:
 dst=git/name/'evidence';dst.mkdir(exist_ok=True)
 audit=root/f'outputs/physics-v{version}-candidate-broad-audit.json'
 if audit.exists():shutil.copy2(audit,dst/'full-broad-audit.json')
 shutil.copy2(root/f'outputs/{name}/final-broad/manifest.json',dst/'full-broad-manifest.json')
for name in ['controller-audit-v170']:
 dst=git/name/'evidence';dst.mkdir(exist_ok=True)
 for f in (root/f'outputs/{name}/native').iterdir():
  if f.suffix in ['.json','.gz']:shutil.copy2(f,dst/f.name)
print('Owned Source/evidence synchronized to healthy Git checkout')
