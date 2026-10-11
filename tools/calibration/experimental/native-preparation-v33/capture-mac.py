from pathlib import Path
import json,subprocess,os,sys,argparse
calibration=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser();p.add_argument('--probe',type=Path,required=True);p.add_argument('--measurements',type=Path,required=True);p.add_argument('--contracts',type=Path,required=True);p.add_argument('--profile',type=Path,default=calibration/'profiles/windows-source-v14.cfg');p.add_argument('--repeat',type=int,choices=[0,1]);p.add_argument('--mode',choices=['points','ordinary','errors','control'],required=True);a=p.parse_args();out=a.measurements;out.mkdir(parents=True,exist_ok=True);exe=str(a.probe.resolve());contracts=json.loads(a.contracts.read_text());sys.path.insert(0,str(calibration));import compare
base={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_EXPERIMENT','RRR3D_CONTACT')) and k not in ['RRR3D_CALIBRATION_PROFILE','RRR3D_CALIBRATION_LEGACY_DIRECT_ROTATION']}
for flag in ['SPRING_BIAS','SPRING_FINAL','SPRING_LOST','SPRING_COLD','FRICTION_ZERO','SUSPENSION_STOP','MESH_CONTACTS','TIRE_FORCE_HISTORY','SUSPENSION_RAW','UPPER_FACE_WINDING','NEGATIVE_X_FACE_ORDER','PRINCIPAL_MOMENTUM','FROZEN_INITIAL_NORMALIZE','MATERIAL_FRAME_CANDIDATE']:base['RRR3D_EXPERIMENT_'+flag]='1'
for flag in ['MASS','BIAS','POINT_VELOCITY','ORIGINAL_DELTA','FORCE_FIRST']:base['RRR3D_EXPERIMENT_NATIVE_CONTACT_'+flag]='0' if a.mode=='control' else '1'
base['RRR3D_EXPERIMENT_NATIVE_CONTACT_ERROR_FRAME']='0' if a.mode=='control' else '1';base['RRR3D_CALIBRATION_PROFILE']=str(a.profile.resolve());base['RRR3D_SEED']='42';base['RRR3D_CONTACT_ROWS']=base['RRR3D_CONTACT_POINTS']=base['RRR3D_CONTACT_NATIVE_FIRST_ONLY']='1'
if a.mode in ['points','errors']:base['RRR3D_CONTACT_GEOMETRY_REPLAY']=base['RRR3D_CONTACT_REPLAY_POSITIONS']=base['RRR3D_CONTACT_REPLAY_SEPARATIONS']='1';base['RRR3D_CONTACT_REPLAY_NATIVE_ERROR']='1' if a.mode=='errors' else '0'
for repeat in ([a.repeat] if a.repeat is not None else range(2)):
 raw=b''
 for c in contracts:
  e=base.copy();e['RRR3D_CONTACT_NATIVE_TRACE_STEP']=str(c['first_tick'])
  if a.mode=='points':e['RRR3D_CONTACT_GEOMETRY_REPLAY_FILE']=str((a.contracts.parent/('points-'+c['case']+'.txt')).resolve())
  elif a.mode=='errors':e['RRR3D_CONTACT_GEOMETRY_REPLAY_FILE']=str((a.contracts.parent/Path(c['file']).name).resolve())
  tmp=out/f'derived-{a.mode}-{c["case"]}-r{repeat}.jsonl';assert not tmp.exists();subprocess.run([exe,str(tmp),'list:'+c['case']],env=e,check=True,stdout=subprocess.DEVNULL,timeout=45);raw+=tmp.read_bytes();tmp.unlink();print(a.mode,repeat,c['case'],flush=True)
 dest=out/f'calibration-derived-error-{a.mode}-run{repeat}-v33-mac.jsonl';assert not dest.exists();dest.write_bytes(raw)
 if a.mode=='control':
  got=compare.load(dest,'mac-jolt-metal');old=compare.load(out/'calibration-native-arithmetic-ordinary-mask0-run0-v32-mac.jsonl','mac-jolt-metal');assert got==old
 print(dest,len(raw),'bytes',flush=True)
