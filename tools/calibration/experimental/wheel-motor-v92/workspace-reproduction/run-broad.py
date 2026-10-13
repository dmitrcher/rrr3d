from pathlib import Path
import json,os,subprocess,hashlib,time
root=Path.cwd();out=root/'outputs/wheel-motor-v92/broad';out.mkdir(parents=True,exist_ok=False);build=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10');probe=root/'work/frozen-v92/rrr3d_physics_calibration';assert all(k in probe.read_bytes() for k in [b'RRR3D_EXPERIMENT_NATIVE_WHEEL_VEHICLE_QUERY_V70',b'RRR3D_EXPERIMENT_NATIVE_WHEEL_ROW_INERTIA_V56',b'RRR3D_EXPERIMENT_NATIVE_TIRE_REPORT_V56',b'RRR3D_EXPERIMENT_NATIVE_WHEEL_ROW_FRAME_V57',b'RRR3D_EXPERIMENT_NATIVE_TIRE_COLD_V57']);env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};profile=json.load(open(root/'outputs/pair-material-v76/focused-inverse/manifest.json'));env.update(profile['environment']);env.update(profile['candidate_changes']);assert all(f.encode() in probe.read_bytes() for f in profile['candidate_changes']);env.pop('RRR3D_CONTACT_POINTS',None);env['RRR3D_EXPERIMENT_NATIVE_CONTACT_ORDER_V79']='1';env['RRR3D_EXPERIMENT_NATIVE_WHEEL_BOX_QUERY_V80']='1';env['RRR3D_EXPERIMENT_NATIVE_LONGITUDINAL_SLIP_V82']='1';env['RRR3D_EXPERIMENT_NATIVE_DIAGNOSTIC_AUTOWAKE_V83']='1';env['RRR3D_EXPERIMENT_NATIVE_DORMANT_MOTION_RESTORE_V83']='1'
env['RRR3D_EXPERIMENT_NATIVE_FORCE_DOMAIN_V84']='1'
env['RRR3D_EXPERIMENT_NATIVE_TIRE_JV_V85']='1';env['RRR3D_EXPERIMENT_NATIVE_TIRE_STEP_V85']='1'
def digest(p):
 h=hashlib.sha256()
 with p.open('rb') as stream:
  for chunk in iter(lambda:stream.read(1024*1024),b''):h.update(chunk)
 return h.hexdigest()
env.update(json.loads((root/'work/frozen-v92/manifest.json').read_text())['environment'])
entries=[]
for rep in range(2):
 print('broad',rep,'start',flush=True);p=out/f'candidate-run{rep}.jsonl';t=time.monotonic()
 with (out/f'candidate-run{rep}.log').open('wb') as log:subprocess.run([str(probe),str(p),'all'],env=env,stdout=log,stderr=log,check=True,timeout=600)
 entries.append({'file':p.name,'elapsed_seconds':time.monotonic()-t,'bytes':p.stat().st_size,'sha256':digest(p)});(out/'capture-manifest.json').write_text(json.dumps({'scope':'Fresh broad 608-case matrix; not certified by prior isolation gates.','probe_sha256':digest(probe),'environment':{k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},'captures':entries,'goal_complete':False},indent=2)+'\n');print('broad',rep,'complete',entries[-1],flush=True)
