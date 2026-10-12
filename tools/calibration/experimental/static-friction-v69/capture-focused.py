from pathlib import Path
import json, math, os, subprocess, hashlib

root = Path.cwd()
out = root / 'outputs/static-friction-focused-v69'
out.mkdir(exist_ok=False)
names = ['border-grounded','border-air-straight','wheel-drive','mobility/monstertruck/wheel2/wheel-drive','monstertruck/settle','monstertruck/wheel-drive','curb-side','mobility/podushka/engine2/wheel-drive','bump-crossing','xCar/suspension-drop','mobility/xCar/engine1/suspension-drop','mobility/xCar/engine2/suspension-drop','mobility/xCar/engine3/suspension-drop','guseniza/suspension-drop','mobility/guseniza/gusWheel2/suspension-drop','mobility/dirtdevil/wheel1/wheel-drive','mobility/devildriverBoss/engine2/wheel-drive']
probe = Path(os.environ['MOTORROCK_CALIBRATION_PROBE'])
assert b'RRR3D_EXPERIMENT_NATIVE_STATIC_FRICTION_V69' in probe.read_bytes(), 'Stale probe: V66 flag missing'
base = {k: v for k, v in os.environ.items() if not k.startswith(('RRR3D_', 'NX_'))}
base.update(json.load(open(root / 'outputs/local-lost-broad-v58/capture-manifest.json'))['environment'])

def load(path):
    data = {name: {'rows': [], 'contacts': [], 'metadata': None} for name in names}
    with path.open() as stream:
        for line in stream:
            row = json.loads(line)
            name = row.get('detail')
            if name not in data:
                continue
            if row['event'] == 'calibration.begin':
                data[name]['metadata'] = row['sample']
            elif row['event'] == 'calibration.sample':
                data[name]['rows'].append(row['sample'])
            elif row['event'] == 'calibration.body-impulse':
                data[name]['contacts'].append(row['sample'])
    for name, case in data.items():
        assert case['metadata'] and len(case['rows']) == case['metadata']['steps'], name
    for case in data.values():
        case['contacts'].sort(key=lambda x: (x['tick'], json.dumps(x,sort_keys=True)))
    return data

def dist(a, b):
    return math.sqrt(sum((x-y)**2 for x, y in zip(a, b)))

def angle(a, b):
    ax,ay,az,aw = a
    bx,by,bz,bw = b
    v = (aw*bx-bw*ax-ay*bz+az*by, aw*by-bw*ay-az*bx+ax*bz, aw*bz-bw*az-ax*by+ay*bx)
    return 2*math.atan2(math.sqrt(sum(x*x for x in v)), abs(sum(x*y for x,y in zip(a,b))))*180/math.pi

ref = load(root/'outputs/calibration-v14-source-windows.jsonl')
(out/'windows-selected.json').write_text(json.dumps(ref))
base.update({'RRR3D_EXPERIMENT_NATIVE_LOST_TIRES_V59':'1','RRR3D_EXPERIMENT_NATIVE_WHEEL_BOX_QUERY_V59':'1'})
v60={'RRR3D_EXPERIMENT_NATIVE_BOX_TRIANGLE_SAT_V60':'1','RRR3D_EXPERIMENT_NATIVE_PRE_SOLVE_CAP_V60':'1','RRR3D_EXPERIMENT_NATIVE_CONTACT_PAIR_ORDER_V60':'1'}
base61=dict(v60,RRR3D_EXPERIMENT_NATIVE_MESH_INVERSE_TRANSLATION_V61='1',RRR3D_EXPERIMENT_NATIVE_RAW_WORLD_FRAME_V61='1')
joint=dict(base61,RRR3D_EXPERIMENT_NATIVE_HULL_STATIC_FRAME_V61='1')
v62=dict(joint,RRR3D_EXPERIMENT_NATIVE_PRE_CALLBACK_FRICTION_V62='1',RRR3D_EXPERIMENT_NATIVE_SUSPENSION_STOP_ORDER_V62='1')
v64=dict(v62,RRR3D_EXPERIMENT_NATIVE_ROW_CREATION_ORDER_V64='1')
v65=dict(v64,RRR3D_EXPERIMENT_NATIVE_SHAPE_ORDER_FRAME_V65='1')
variants={'control_v64':v64,'static_v69':dict(v64,RRR3D_EXPERIMENT_NATIVE_STATIC_FRICTION_V69='1'),'static_and_stop_v69':dict(v64,RRR3D_EXPERIMENT_NATIVE_STATIC_FRICTION_V69='1',RRR3D_EXPERIMENT_NATIVE_HARD_STOP_SOLVE_V68='1')}
report = {'scope': 'Single-car fixtures, no AI; unchanged original parameters. V69 tests separate static/dynamic friction limits, optionally coupled with V68 arithmetic.', 'cases': {}, 'goal_complete': False}
captures = []
for mode, changes in variants.items():
    env = dict(base)
    env.update(changes)
    runs = []
    for rep in range(2):
        path = out / f'{mode}-run{rep}.jsonl'
        if not path.exists():
            with (out / f'{mode}-run{rep}.log').open('wb') as log:
                subprocess.run([str(probe), str(path), 'list:'+','.join(names)], env=env, stdout=log, stderr=log, check=True, timeout=180)
        runs.append(load(path))
        captures.append({'mode':mode, 'repeat':rep, 'changes':changes, 'file':path.name, 'sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
    assert runs[0] == runs[1], mode
    if mode=='control_v64':assert runs[0]==load(root/'outputs/creation-order-broad-v64/candidate-run0.jsonl'), 'V64 preserved'

    for name in names:
        native, mac = ref[name]['rows'], runs[0][name]['rows']
        p = [dist(a['position'], b['position']) for a,b in zip(native,mac)]
        v = [dist(a['velocity'], b['velocity']) for a,b in zip(native,mac)]
        r = [angle(a['rotation'],b['rotation']) for a,b in zip(native,mac)]
        detail = {'position_max':max(p), 'position_rms':math.sqrt(sum(x*x for x in p)/len(p)), 'position_final':p[-1], 'velocity_max':max(v), 'orientation_max_degrees':max(r),
                  'error_landmarks': {str(t): {'position':p[t], 'velocity':v[t], 'orientation_degrees':r[t]} for t in [19,59,119,120,179,239,299,399,499,599]},
                  'first_position_above_0_01':next((i for i,x in enumerate(p) if x>.01),None),
                  'first_body_contact_windows':ref[name]['contacts'][0]['tick'] if ref[name]['contacts'] else None,
                  'first_body_contact_mac':runs[0][name]['contacts'][0]['tick'] if runs[0][name]['contacts'] else None}
        report['cases'].setdefault(name,{})[mode]=detail
        print(mode, name, {k:detail[k] for k in ['position_max','velocity_max','orientation_max_degrees','first_position_above_0_01']}, flush=True)
    (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
(out/'capture-manifest.json').write_text(json.dumps({'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(), 'environment':{k:v for k,v in base.items() if k.startswith(('RRR3D_','NX_'))}, 'names':names, 'captures':captures, 'physical_samples_two_repeats_exact':True,'contact_event_multisets_two_repeats_exact':True,'contact_events_canonicalized_for_parallel_report_order':True, 'goal_complete':False},indent=2)+'\n')
