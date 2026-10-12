from pathlib import Path
import hashlib,importlib.util,json,os,subprocess,sys
root=Path.cwd();out=root/'outputs/triangle-edge-mask-gates-v60';out.mkdir(exist_ok=False)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_spring_probe')
base={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};base.update(json.loads((root/'outputs/wheel-row-broad-v56/capture-manifest.json').read_text())['environment'])
sys.dont_write_bytecode=True;sys.path.insert(0,str(root/'work/sync-rrr3d/tools/calibration'));import compare
spec=importlib.util.spec_from_file_location('words',root/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
pkg=root/'work/v51-package/measurements';summary={}
for scope in ['native24']:
    reference=pkg/'references'/f'{scope}-windows.jsonl';names=list(compare.load(reference,'windows-physx-d3d9'));old=compare.load(root/'outputs/wheel-row-gates-v56'/f'{scope}-candidate-run0-mac.jsonl','mac-jolt-metal');summary[scope]={}
    for mode in ['sat','cap']:
        env=dict(base)
        if mode in ['sat','cap']:env.update({f:'1' for f in ['RRR3D_EXPERIMENT_NATIVE_WHEEL_ROW_FRAME_V57','RRR3D_EXPERIMENT_NATIVE_TIRE_COLD_V57','RRR3D_EXPERIMENT_NATIVE_WHEEL_ANCHOR_BIAS_V57','RRR3D_EXPERIMENT_NATIVE_TIRE_VELOCITY_LIMIT_V58','RRR3D_EXPERIMENT_NATIVE_LOST_LOCAL_FRAME_V58','RRR3D_EXPERIMENT_NATIVE_LOST_TIRES_V59','RRR3D_EXPERIMENT_NATIVE_WHEEL_BOX_QUERY_V59',('RRR3D_EXPERIMENT_NATIVE_BOX_TRIANGLE_SAT_V60' if mode=='sat' else 'RRR3D_EXPERIMENT_NATIVE_PRE_SOLVE_CAP_V60')]})
        runs=[]
        for rep in range(2):
            path=out/f'{scope}-{mode}-run{rep}-mac.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(names)],env=env,capture_output=True,check=True,timeout=180);runs.append(compare.load(path,'mac-jolt-metal'))
        assert runs[0]==runs[1]
        if mode=='control':assert runs[0]==old,(scope,'defaultoff_changed')
        report=words.audit(reference,out/f'{scope}-{mode}-run0-mac.jsonl');summary[scope][mode]=report
        print(scope,mode,'remaining',report['cases_remaining'],flush=True)
        print(scope,mode,len(names),'exact, repeats exact',flush=True)
(out/'capture-manifest.json').write_text(json.dumps({'scope':'Retained 291 independent physics gates, fresh control/candidate two repeats each.','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'environment':{k:v for k,v in base.items() if k.startswith(('RRR3D_','NX_'))},'results':summary,'default_off_preserves_v56':'not tested by this ablation','goal_complete':False},indent=2)+'\n')
