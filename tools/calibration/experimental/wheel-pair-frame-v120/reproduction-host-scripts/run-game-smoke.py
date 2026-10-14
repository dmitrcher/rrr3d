from pathlib import Path
import json,os,subprocess,hashlib,time
root=Path.cwd();out=root/'outputs/pair-pole-v120/game-smoke';out.mkdir(parents=True,exist_ok=False)
app=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/RRR3d.app');exe=app/'Contents/MacOS/RRR3d'
m=json.load(open(root/'outputs/pair-material-v76/focused-inverse/manifest.json'));env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(m['environment']);env.update(m['candidate_changes']);env['RRR3D_EXPERIMENT_NATIVE_CONTACT_ORDER_V79']='1';env['RRR3D_EXPERIMENT_NATIVE_WHEEL_BOX_QUERY_V80']='1';env['RRR3D_EXPERIMENT_NATIVE_LONGITUDINAL_SLIP_V82']='1';env['RRR3D_EXPERIMENT_NATIVE_DIAGNOSTIC_AUTOWAKE_V83']='1';env['RRR3D_EXPERIMENT_NATIVE_DORMANT_MOTION_RESTORE_V83']='1';env['RRR3D_EXPERIMENT_NATIVE_FORCE_DOMAIN_V84']='1';env['RRR3D_EXPERIMENT_NATIVE_TIRE_JV_V85']='1';env['RRR3D_EXPERIMENT_NATIVE_TIRE_STEP_V85']='1';env.update(json.loads((root/'work/frozen-v120-state-candidate/manifest.json').read_text())['environment']);env['RRR3D_TRACE_FILE']=str(out/'game-parity.jsonl')
args=[str(exe),'--solo-physics-replay','--track=0','--game-debug','--smoke-test-frames=600','--smoke-screenshot']
assert all(f.encode() in exe.read_bytes() for f in json.load(open(root/'outputs/pair-material-v76/focused-inverse/manifest.json'))['candidate_changes'])
t=time.monotonic()
with (out/'game.log').open('wb') as log:
 p=subprocess.Popen(args,env=env,stdout=log,stderr=log,cwd=app/'Contents/Resources')
 (out/'launch.json').write_text(json.dumps({'pid':p.pid,'arguments':args,'environment':{k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},'executable_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'scope':'Headless real-map solo replay, no rendering or car/car collision parity claim'},indent=2)+'\n')
 code=p.wait(timeout=240)
(out/'exit.json').write_text(json.dumps({'exit_code':code,'elapsed_seconds':time.monotonic()-t,'goal_complete':False},indent=2)+'\n');assert code==0
