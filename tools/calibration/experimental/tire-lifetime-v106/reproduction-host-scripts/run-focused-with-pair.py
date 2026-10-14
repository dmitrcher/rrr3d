from pathlib import Path
import json, os, subprocess, gzip, struct, hashlib, time

root=Path.cwd(); out=root/'outputs/tire-lifetime-v106/focused-with-pair';out.mkdir(parents=True,exist_ok=False)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration')
base=json.loads((root/'work/frozen-v105/manifest.json').read_text())['environment']
names=['border-front-grounded','border-grounded','border-front-air-straight','body-tilt-roll','body-wheel-contact-x','body-wheel-contact-pitch','body-wheel-contact-roll','bump-crossing','suspension-drop','spring-soft','body-tilt-pitch','body-slide-y','slope-longitudinal','slope-lateral','curb-side','settle','wheel-drive','brake','reverse-drive','inertia-light','pair-stock-pole-1','mobility/marauder/wheel1/wheel-drive','mobility/marauder/wheel1/brake-moving','mobility/manticora/wheel3/lateral','mobility/dirtdevil/wheel1/wheel-drive','mobility/devildriver/engine3/lateral','mobility/devildriverBoss/engine3/lateral']
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def physical(row):
    row=dict(row)
    for k in ['position','velocity','rotation','angular_momentum']:row[k]=list(map(bits,row[k]))
    floats={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'}
    row['wheels']=[{k:bits(v) if k in floats else v for k,v in w.items()} for w in row['wheels']]
    return row
def load(path):
    if not path.exists():path=Path(str(path)+'.gz')
    result={n:[] for n in names}
    with (gzip.open(path,'rt') if str(path).endswith('.gz') else path.open()) as f:
        for r in map(json.loads,f):
            if r.get('event')=='calibration.sample' and r['detail'] in result:result[r['detail']].append(physical(r['sample']))
    assert all(result.values());return result
reference=load(root/'outputs/calibration-v14-source-windows.jsonl')
saved=load(root/'outputs/callback-queue-v105/broad/candidate-run0.jsonl')
modes={'control':{},'lifetime':{'RRR3D_EXPERIMENT_NATIVE_TIRE_OBJECT_LIFETIME_V106':'1','RRR3D_EXPERIMENT_NATIVE_WHEEL_PAIR_RESET_V106':'1'}}
report={'scope':'V106 tire object registration order: default-off V105 and candidate; original Windows unchanged; two modes x two uninterrupted repeats.','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'environment':base,'modes':{},'goal_complete':False}
for mode,changes in modes.items():
    runs=[]
    for rep in range(2):
        env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env.update(changes)
        p=out/f'{mode}-{rep}.jsonl'; start=time.monotonic()
        with (out/f'{mode}-{rep}.log').open('wb') as f:subprocess.run([str(probe),str(p),'list:'+','.join(names)],env=env,stdout=f,stderr=f,check=True,timeout=240)
        runs.append(load(p)); print(mode,rep,'captured',sum(map(len,runs[-1].values())),round(time.monotonic()-start,2),flush=True)
    assert runs[0]==runs[1],('Nondeterminism',mode)
    if mode=='control':assert runs[0]==saved,'Default-off changed frozen V105'
    cases={}
    for n in names:
        assert len(runs[0][n])==len(reference[n])
        dif=[a['tick'] for a,b in zip(reference[n],runs[0][n]) if a!=b]
        olddif=[a['tick'] for a,b in zip(saved[n],runs[0][n]) if a!=b]
        cases[n]={'first_difference_windows':dif[0] if dif else None,'differing_ticks_windows':len(dif),'first_change_from_v104':olddif[0] if olddif else None}
        print(mode,n,cases[n],flush=True)
    report['modes'][mode]={'changes':changes,'cases':cases,'repeats_exact':True,'states_per_repeat':sum(map(len,runs[0].values()))}
    (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
report['default_off_matches_v104']=True
(out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
