from pathlib import Path
import json,gzip,hashlib
r=Path.cwd();rows=list(map(json.loads,(r/'outputs/static-pair-order-v108/quick/dispatch.jsonl').open()));blocks=[]
for row in rows:
 if row['sdk_counter']==1:blocks.append([])
 blocks[-1].append(row)
assert len(blocks)==2 and all(len(x)==600 for x in blocks);actual={row['sdk_counter']-1:row for row in blocks[1]};checks=[]
for rep in range(2):
 for tick in [19,203,220,224]:
  path=r/f'outputs/static-pair-order-v108/native-manager-slots/creation-curb-side-{tick}-{rep}-private.jsonl.gz' if tick!=224 else r/f'outputs/static-box-query-v107/native-next-queue224/creation-curb-side-{tick}-{rep}-private.jsonl.gz';native=list(map(json.loads,gzip.open(path,'rt')));mapping={x['enqueue_wheel']['address']+4:(x['enqueue_wheel']['words'][2]&0x3ffff)+1-3 for x in native if x.get('function')=='wheel-enqueue'};ranks=[0]*4
  for row in native:
   if row.get('function')=='wheel-contact-manager':
    words=row['contact_manager']['words'];wheel=mapping.get(words[9],mapping.get(words[10]));assert wheel is not None;ranks[wheel]=words[5]+1
  assert actual[tick]['supported'] and actual[tick]['dispatch_ranks']==ranks
  checks.append({'repeat':rep,'physical_tick':tick,'actual_Native_wheel_processor_ranks':ranks,'actual_Mac_dispatch_ranks':actual[tick]['dispatch_ranks'],'actual_Mac_creation_visits_including_inactive_wheels':actual[tick]['creation_visits']})
manifest=json.loads((r/'work/frozen-v108/manifest.json').read_text());current=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');assert hashlib.sha256(current.read_bytes()).hexdigest()==manifest['hashes']['rrr3d_physics_calibration'];(r/'outputs/static-pair-order-v108/live-dispatch-proof.json').write_text(json.dumps({'scope':'Actual Mac independent runtime geometry and manager lifecycle, not Native event/pose replay: observed dispatch ranks equal independently captured Native processor indices in two Native repeats. Inactive wheel indices in creation_visits allocate no current spring object.','probe_sha256':manifest['hashes']['rrr3d_physics_calibration'],'checks':checks,'goal_complete':False},indent=2)+'\n');print('Actual independent runtime dispatch ranks exact Native at19,203,220,224 in both Native repeats.')
