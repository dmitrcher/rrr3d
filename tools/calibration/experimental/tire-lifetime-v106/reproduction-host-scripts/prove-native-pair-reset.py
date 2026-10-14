from pathlib import Path
import json,gzip,subprocess,hashlib
root=Path.cwd();work=root/'work/tire-lifetime-v106';source=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');out=root/'outputs/tire-lifetime-v106';subprocess.run(['clang++','-std=c++17','-O2','-ffp-contract=off','-I'+str(source/'src/Rock3dEngine/header/physics'),str(work/'pair-geometry-proof.cpp'),'-o',str(work/'pair-geometry-proof')],check=True)
rows=[r['sample'] for r in map(json.loads,open(root/'outputs/calibration-v14-source-windows.jsonl')) if r.get('event')=='calibration.sample' and r['detail']=='bump-crossing'];inputs='0 0 0 12 0 0 0 1\n'+''.join(' '.join(map(str,[r['tick']+1]+r['position']+r['rotation']))+'\n' for r in rows[:80]);run=subprocess.run([str(work/'pair-geometry-proof')],input=inputs,text=True,capture_output=True,check=True);shadow=[list(map(int,x.split())) for x in run.stdout.splitlines()];print('shadow actual geometric events',shadow,flush=True)
proof=[]
for rep in range(2):
 p=out/'native-manager-reset'/f'bump-crossing-80-{rep}-private.jsonl.gz';trace=list(map(json.loads,gzip.open(p,'rt')))
 clears=[];pairs=[]
 for r in trace:
  if r.get('function')=='all-wheel-objects-clear-entry' and r['stack']['words'][0]-r['core_base']==0x206f41:
   clears.append([r['sample_tick_v71'],(r['cleared_wheel_backend']['words'][2]&0x3ffff)+1-3])
  if r.get('function')=='contact-manager-reset-flag-set':
   ids=[(r[f'manager_shape{i}']['words'][2]&0x3ffff)+1 for i in [0,1]]
   assert r['manager_context']['words'][22]&2
   pairs.append({'tick':r['sample_tick_v71'],'shapes':ids,'processed_mask2':True})
 assert clears==shadow,(rep,clears,shadow)
 p=out/'native-bump-pair-window80'/f'endpoint-bump-crossing-80-{rep}-private.jsonl.gz';removes=[]
 for r in map(json.loads,gzip.open(p,'rt')):
  if r.get('function')=='broadphase-removed-list':
   words=r['removed_pairs']['words'];assert len(words)==r['removed_pair_count']*4
   for i in range(r['removed_pair_count']):
    ids=[(v&0x3ffff)+1 for v in words[i*4:i*4+2]]
    if ids[1]==7 and 3<=ids[0]<=6:removes.append([r['sample_tick_v71']+1,ids[0]-3])
 assert removes==clears
 proof.append({'repeat':rep,'broadphase_removed_plus_one_step':removes,'actual_all_objects_clear_call_206f41':clears,'processed_pair_contexts':pairs,'shadow_geometry_equals_actual_native_events':True})
report={'scope':'Read-only original Native manager/removed-list evidence with independent shadow of original float32 public poses. Shadow only validates pair geometry/lifecycle and is never supplied to runtime Mac simulation. Fixture radius.4/travel.5 are unadjusted original defaults. Original static floor and bump bounds.','proof':proof,'native_all_clear_entry_rva':'0x148540','native_handle_zero_instruction_rva':'0x148573','native_manager_clear_return_rva':'0x206f41','native_processed_flag_write_rva':'0x206ed5','source_pair_helper_sha256':hashlib.sha256((source/'src/Rock3dEngine/header/physics/NativeWheelPairLifetimeV106.h').read_bytes()).hexdigest(),'goal_complete':False};(out/'native-pair-reset-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Original Native two repeats: exact wheel reset events; private processed mask2 verified.')
