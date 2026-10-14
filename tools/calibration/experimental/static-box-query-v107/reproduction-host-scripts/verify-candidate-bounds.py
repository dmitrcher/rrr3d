from pathlib import Path
import json,gzip,subprocess,hashlib
r=Path.cwd();w=r/'work/static-box-query-v107';active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');wanted={47:None,48:None}
for row in map(json.loads,open(r/'outputs/calibration-v14-source-windows.jsonl')):
 if row.get('event')=='calibration.sample' and row['detail']=='bump-crossing' and row['sample']['tick'] in wanted:wanted[row['sample']['tick']]=row['sample']
inputs=''.join(' '.join(map(str,[tick+1]+row['position']+row['rotation']))+'\n' for tick,row in wanted.items());expected=[];proof=[]
for tick in [48,49]:
 counts=[]
 for rep in range(2):
  p=r/f'outputs/static-box-query-v107/native-boundary-transform/creation-bump-crossing-{tick}-{rep}-private.jsonl.gz';calls=[row for row in map(json.loads,gzip.open(p,'rt')) if row.get('function')=='wheel-box-ray-call'];counts.append(len(calls))
 assert counts[0]==counts[1] and counts[0] in [0,2];expected.append([tick,int(counts[0]>0),int(counts[0]>0),0,0]);proof.append({'physical_tick':tick,'actual_Native_static_box_call_count_per_repeat':counts})
reports=[]
for opt in ['O0','O2']:
 exe=w/('candidate-bounds-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(active/'src/Rock3dEngine/header/physics'),str(w/'candidate-bounds-proof.cpp'),'-o',str(exe)],check=True);actual=[list(map(int,x.split())) for x in subprocess.check_output([str(exe)],input=inputs,text=True).splitlines()];assert actual==expected;reports.append({'optimization':opt,'original_pose_shadow_candidate_masks':actual,'actual_Native_query_presence_exact':True})
(w/'candidate-bounds-input.txt').write_text(inputs);report={'scope':'Actual original Native static query presence at bump48/49, two repeats. Production candidate helper independently checked under O0/O2 on original measured poses. Shadow poses validate the geometry test only, never enter runtime Mac. Source endpoint retained before unit normalization.','proof':proof,'checks':reports,'production_header_sha256':hashlib.sha256((active/'src/Rock3dEngine/header/physics/NativeStaticWheelBoxQueryV107.h').read_bytes()).hexdigest(),'goal_complete':False};(r/'outputs/physics-v107-native-candidate-bounds-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('O0/O2 original Native query presence exact: no static box query48, two front queries49')
