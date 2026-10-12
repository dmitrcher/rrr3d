import argparse,hashlib,json,os,subprocess,sys
from pathlib import Path
p=argparse.ArgumentParser()
p.add_argument('--probe',type=Path,required=True);p.add_argument('--old-probe',type=Path,required=True)
p.add_argument('--environment',type=Path,required=True);p.add_argument('--reference',type=Path,required=True)
p.add_argument('--profile',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
a=p.parse_args();a.out.mkdir(exist_ok=False)
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
reference=compare.load(a.reference,'windows-physx-d3d9');names=list(reference)
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_EXPERIMENT','RRR3D_CONTACT'))}
metadata=json.loads(a.environment.read_text());env.update(metadata.get('environment',metadata.get('base_environment',{})))
env['RRR3D_CALIBRATION_PROFILE']=str(a.profile.resolve())
env['RRR3D_EXPERIMENT_NATIVE_PRINCIPAL_INERTIA']='1'
report={'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
        'old_probe_sha256':hashlib.sha256(a.old_probe.read_bytes()).hexdigest(),
        'base_environment':{k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},'captures':{}}
previous=None
for mode in ['baseline','control','ordinary','diagnostic']:
    binary=a.old_probe if mode=='baseline' else a.probe
    if mode in ['ordinary','diagnostic']:env['RRR3D_EXPERIMENT_NATIVE_SHAPE_MASS']='1'
    else:env.pop('RRR3D_EXPERIMENT_NATIVE_SHAPE_MASS',None)
    if mode=='diagnostic':env['RRR3D_CALIBRATION_ISOLATE_NEUTRAL_STABILIZATION']='1'
    else:env.pop('RRR3D_CALIBRATION_ISOLATE_NEUTRAL_STABILIZATION',None)
    runs=[]
    for r in range(2):
        dest=a.out/f'calibration-shape-{mode}-run{r}-mac.jsonl'
        subprocess.run([str(binary.resolve()),str(dest.resolve()),'list:'+','.join(names)],env=env,check=True,timeout=180,stdout=subprocess.DEVNULL)
        got=compare.load(dest,'mac-jolt-metal');assert set(got)==set(reference)
        if mode=='control':assert got==previous,'Default-off shape candidate changes previous binary'
        stats={n:{'first_difference':next((x['tick'] for x,y in zip(reference[n]['rows'],got[n]['rows']) if x!=y),None),
                  'max_position':max(compare.norm(x['position'],y['position']) for x,y in zip(reference[n]['rows'],got[n]['rows'])),
                  'max_velocity':max(compare.norm(x['velocity'],y['velocity']) for x,y in zip(reference[n]['rows'],got[n]['rows']))} for n in names}
        report['captures'][mode+str(r)]={'sha256':hashlib.sha256(dest.read_bytes()).hexdigest(),'cases':stats}
        runs.append(got);print(mode,r,'exact',sum(x['first_difference'] is None for x in stats.values()),'of',len(stats),flush=True)
    assert runs[0]==runs[1],'Repeatability failure'
    if mode=='baseline':previous=runs[0]
(a.out/'capture-manifest.json').write_text(json.dumps(report,indent=2)+'\n')
for n in names:
    old=report['captures']['control0']['cases'][n];new=report['captures']['ordinary0']['cases'][n]
    diagnostic=report['captures']['diagnostic0']['cases'][n]
    if old!=new or new!=diagnostic:print(n,'before',old,'mass',new,'diagnostic',diagnostic,flush=True)
