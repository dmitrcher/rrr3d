from pathlib import Path
import os,subprocess,json,sys,argparse
calibration=Path(__file__).resolve().parents[2];sys.path.insert(0,str(calibration));import compare
p=argparse.ArgumentParser();p.add_argument('--probe',type=Path,required=True);p.add_argument('--measurements',type=Path,required=True);p.add_argument('--contracts',type=Path,required=True);p.add_argument('--profile',type=Path,default=calibration/'profiles/windows-source-v14.cfg');p.add_argument('--mask',type=int,required=True);p.add_argument('--mode',choices=['ordinary','points','errors'],required=True);p.add_argument('--repeat',type=int,default=0);p.add_argument('--case',default='');a=p.parse_args();assert 0<=a.mask<32;out=a.measurements;out.mkdir(parents=True,exist_ok=True)
contracts=json.loads(a.contracts.read_text());exe=a.probe.resolve()
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_EXPERIMENT','RRR3D_CONTACT')) and k not in ['RRR3D_CALIBRATION_PROFILE','RRR3D_CALIBRATION_LEGACY_DIRECT_ROTATION']}
for flag in ['SPRING_BIAS','SPRING_FINAL','SPRING_LOST','SPRING_COLD','FRICTION_ZERO','SUSPENSION_STOP','MESH_CONTACTS','TIRE_FORCE_HISTORY','SUSPENSION_RAW','UPPER_FACE_WINDING','NEGATIVE_X_FACE_ORDER','PRINCIPAL_MOMENTUM','FROZEN_INITIAL_NORMALIZE','MATERIAL_FRAME_CANDIDATE']:env['RRR3D_EXPERIMENT_'+flag]='1'
env['RRR3D_CALIBRATION_PROFILE']=str(a.profile.resolve());env['RRR3D_SEED']='42';env['RRR3D_CONTACT_ROWS']=env['RRR3D_CONTACT_POINTS']=env['RRR3D_CONTACT_NATIVE_FIRST_ONLY']='1'
for i,flag in enumerate(['MASS','BIAS','POINT_VELOCITY','ORIGINAL_DELTA','FORCE_FIRST']):env['RRR3D_EXPERIMENT_NATIVE_CONTACT_'+flag]='1' if a.mask&(1<<i) else '0'
if a.mode!='ordinary':
 env['RRR3D_CONTACT_GEOMETRY_REPLAY']=env['RRR3D_CONTACT_REPLAY_POSITIONS']=env['RRR3D_CONTACT_REPLAY_SEPARATIONS']='1'
 env['RRR3D_CONTACT_REPLAY_NATIVE_ERROR']='1' if a.mode=='errors' else '0'
raw=b''
for c in contracts:
 if a.case and c['case']!=a.case:continue
 e=env.copy();e['RRR3D_CONTACT_NATIVE_TRACE_STEP']=str(c['first_tick'])
 if a.mode!='ordinary':e['RRR3D_CONTACT_GEOMETRY_REPLAY_FILE']=str((a.contracts.parent/(Path(c['file']).name if a.mode=='errors' else 'points-'+c['case']+'.txt')).resolve())
 tmp=out/f'native-arithmetic-{a.mode}-mask{a.mask}-r{a.repeat}-{c["case"]}-v32-mac.jsonl';assert not tmp.exists();subprocess.run([str(exe),str(tmp),'list:'+c['case']],env=e,check=True,stdout=subprocess.DEVNULL,timeout=45);raw+=tmp.read_bytes();tmp.unlink();print(c['case'],a.mask,a.mode,flush=True)
assert raw
suffix='-'+a.case if a.case else ''
dest=out/f'calibration-native-arithmetic-{a.mode}-mask{a.mask}-run{a.repeat}{suffix}-v32-mac.jsonl';assert not dest.exists();dest.write_bytes(raw);got=compare.load(dest,'mac-jolt-metal')
if a.mask==0 and a.mode=='ordinary':
 baseline=compare.load(out/('calibration-inverse-normal-rows-flat-run0-v30-mac.jsonl' if a.mode=='ordinary' else 'calibration-row-arithmetic-both-mask0-run0-v30-mac.jsonl'),'mac-jolt-metal');assert all(got[n]==baseline[n] for n in got)
print(dest,len(got),len(raw),'bytes',flush=True)
