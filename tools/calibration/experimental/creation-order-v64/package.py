"""Freeze research histories with streaming hashes; never duplicate full game assets."""
from pathlib import Path
import hashlib,json,zipfile,time,os
root=Path.cwd();archive=root/'outputs/MotorRock-v64-Suspension-Investigation.zip';items={}
def add(p,member):
 assert p.is_file() and member not in items
 items[member]=p
for name in ['suspension-order-v62','creation-order-v64']:
 base=root/'work/sync-rrr3d/tools/calibration/experimental'/name
 for p in sorted(base.rglob('*')):
  if p.is_file() and '__pycache__' not in p.parts:add(p,'source/'+name+'/'+str(p.relative_to(base)))
for n in ['PHYSICS_PORT_PLAN.md','PHYSICS_SUSPENSION_ORDER_V64_2026-10-12.md']:add(root/'work/sync-rrr3d/docs'/n,'docs/'+n)
folders=['suspension-order-focused-v62','suspension-order-gates-v62','suspension-order-broad-v62','native-border-wheel-v62','native-drop-wheel-v63','native-border-next-wheel-v63','lost-stop-order-focused-v63','lost-stop-order-gates-v63','lost-stop-order-broad-v63','creation-order-focused-v64','creation-order-gates-v64','creation-order-broad-v64','mac-creation-order-trace-v64','suspension-order-contact-trace-v62','mac-border-stop-order-v62','mac-border-force-v62','mac-border-wheel-v62','material-friction-focused-v62','material-friction-gates-v62','material-friction-trace-v62']
for folder in folders:
 base=root/'outputs'/folder
 if not base.is_dir():continue
 for p in sorted(base.rglob('*')):
  if p.is_file() and '__pycache__' not in p.parts:add(p,'measurements/'+folder+'/'+str(p.relative_to(base)))
for p in (root/'outputs').glob('physics-v6[234]-*'):
 if p.is_file() and p.suffix in ['.json','.log']:add(p,'reports/'+p.name)
for p in (root/'work/frozen-v62').iterdir():
 if p.is_file():add(p,'mac-probes/v62/'+p.name)
for p in (root/'work/frozen-v64').iterdir():
 if p.is_file():add(p,'mac-probes/v64/'+p.name)
for p in (root/'work/v51-package/measurements/references').glob('*-windows.jsonl'):add(p,'references/independent-gates/'+p.name)
for p in (root/'outputs/quaternion-static-frame-gates-v61').glob('*-control-run0-mac.jsonl'):add(p,'references/v61-control/'+p.name)
def sanitize(d):
 if isinstance(d,dict):
  for k,v in list(d.items()):
   if k=='environment' and isinstance(v,dict):d[k]={n:x for n,x in v.items() if n.startswith(('RRR3D_','NX_','MOTORROCK_'))}
   else:sanitize(v)
 elif isinstance(d,list):
  for v in d:sanitize(v)
entries=[];rawbytes=0
with zipfile.ZipFile(archive.with_suffix('.partial.zip'),'w',zipfile.ZIP_DEFLATED,compresslevel=3,allowZip64=True) as z:
 for member,p in items.items():
  # Manifests can contain unrelated process environment. Publish only physics controls.
  cleaned=None
  if p.suffix=='.json':
   data=json.loads(p.read_text());old=json.dumps(data,sort_keys=True);sanitize(data)
   if json.dumps(data,sort_keys=True)!=old:cleaned=(json.dumps(data,indent=2)+'\n').encode()
  h=hashlib.sha256();size=0
  with z.open(member,'w',force_zip64=True) as dst:
   if cleaned is not None:dst.write(cleaned);h.update(cleaned);size=len(cleaned)
   else:
    with p.open('rb') as src:
     for chunk in iter(lambda:src.read(1024*1024),b''):dst.write(chunk);h.update(chunk);size+=len(chunk)
  entries.append({'member':member,'original_relative_path':str(p.relative_to(root)),'sha256':h.hexdigest(),'bytes':size,'environment_sanitized':cleaned is not None});rawbytes+=size
  if len(entries)%50==0:print('archived',len(entries),'members',flush=True)
 manifest={'scope':'Default-off research checkpoint; failed V62/V63 variants retained. Full-game parity unverified.','goal_complete':False,'native_Golden_sha256':'4bcc66c8f4f28ccb77a07759175573cc56d72035636642617fae4e75addecc1f','native_Golden_source_release':'https://github.com/dmitrcher/rrr3d/releases/tag/physics-practical-v59-investigation-2026-10-12','native_Golden_included':False,'proprietary_Native_DLLs_included':False,'full_game_assets_duplicated':False,'files':entries}
 z.writestr('manifest.json',json.dumps(manifest,indent=2)+'\n')
os.replace(archive.with_suffix('.partial.zip'),archive)
with zipfile.ZipFile(archive) as z:
 assert set(z.namelist())==set(items)|{'manifest.json'}
 for e in entries:
  h=hashlib.sha256();size=0
  with z.open(e['member']) as src:
   for chunk in iter(lambda:src.read(1024*1024),b''):h.update(chunk);size+=len(chunk)
  assert size==e['bytes'] and h.hexdigest()==e['sha256'],e['member']
h=hashlib.sha256()
with archive.open('rb') as src:
 for c in iter(lambda:src.read(1024*1024),b''):h.update(c)
report={'archive':archive.name,'bytes':archive.stat().st_size,'sha256':h.hexdigest(),'hashed_members':len(entries),'member_bytes':rawbytes,'all_members_verified':True,'goal_complete':False}
(root/'outputs/physics-v64-archive-verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report,flush=True)
