from pathlib import Path
import json, os, subprocess, gzip, struct, hashlib, time

root=Path.cwd(); out=root/'outputs/lost-tire-v110/focused';out.mkdir(parents=True,exist_ok=False)
probe=root/'work/frozen-v110/rrr3d_physics_calibration'
base=json.loads((root/'work/frozen-v109/manifest.json').read_text())['environment']
names=['target-half','border-front-grounded','border-grounded','border-front-air-straight','body-tilt-roll','body-wheel-contact-x','body-wheel-contact-pitch','body-wheel-contact-roll','bump-crossing','suspension-drop','spring-soft','body-tilt-pitch','body-slide-y','slope-longitudinal','slope-lateral','curb-side','settle','wheel-drive','brake','reverse-drive','inertia-light','pair-stock-pole-1','mobility/marauder/wheel1/wheel-drive','mobility/marauder/wheel1/brake-moving','mobility/manticora/wheel3/lateral','mobility/dirtdevil/wheel1/wheel-drive','mobility/devildriver/engine3/lateral','mobility/devildriverBoss/engine3/lateral']
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
saved=load(root/'outputs/contact-bias-v109/broad-final/candidate-run0.jsonl')
modes={'control':{},'candidate':{'RRR3D_EXPERIMENT_NATIVE_LOST_TIRE_DISPATCH_V110':'1'}}
report={'scope':'V109 hull-reference material frame and complete static mesh manager scene: default-off V108 and candidate; original Windows unchanged; two modes x two uninterrupted repeats.','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'environment':base,'modes':{},'goal_complete':False}
for mode,changes in modes.items():
    runs=[]
    for rep in range(2):
        env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env.update(changes)
        p=out/f'{mode}-{rep}.jsonl'; start=time.monotonic()
        with (out/f'{mode}-{rep}.log').open('wb') as f:subprocess.run([str(probe),str(p),'list:'+','.join(names)],env=env,stdout=f,stderr=f,check=True,timeout=240)
        runs.append(load(p)); print(mode,rep,'captured',sum(map(len,runs[-1].values())),round(time.monotonic()-start,2),flush=True)
    assert runs[0]==runs[1],('Nondeterminism',mode)
    if mode=='control':assert runs[0]==saved,'Default-off changed frozen V109'
    cases={}
    for n in names:
        assert len(runs[0][n])==len(reference[n])
        dif=[a['tick'] for a,b in zip(reference[n],runs[0][n]) if a!=b]
        olddif=[a['tick'] for a,b in zip(saved[n],runs[0][n]) if a!=b]
        cases[n]={'first_difference_windows':dif[0] if dif else None,'differing_ticks_windows':len(dif),'first_change_from_v109':olddif[0] if olddif else None}
        print(mode,n,cases[n],flush=True)
    report['modes'][mode]={'changes':changes,'cases':cases,'repeats_exact':True,'states_per_repeat':sum(map(len,runs[0].values()))}
    (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
report['default_off_matches_v109']=True
(out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
