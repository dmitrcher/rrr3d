from pathlib import Path
import json,sys,hashlib,math,struct,argparse
from collections import Counter
sys.path.insert(0,str(Path(__file__).resolve().parents[2]))
import compare
parser=argparse.ArgumentParser(description='Verify v29 single-step interventions, normal-row diagnostics and exact repeat controls.')
parser.add_argument('--measurements',type=Path,required=True)
parser.add_argument('--contracts',type=Path,required=True)
args=parser.parse_args()
out=args.measurements
contracts=json.loads(args.contracts.read_text())
names={c['case']:c for c in contracts}
def records(path):
    result=[];case=None
    for line in path.open():
        r=json.loads(line);compare.finite(r)
        if r['event']=='calibration.begin':case=r['detail']
        result.append({'case':case,'event':r['event'],'detail':r['detail'],'sample':r.get('sample')})
    return result
def digest(r):return hashlib.sha256(json.dumps(r,sort_keys=True,separators=(',',':')).encode()).hexdigest()
def repeated(prefix):
    paths=[out/(prefix.format(r=r)+'.jsonl') for r in range(2)]
    a,b=[records(p) for p in paths]
    assert a==b, next(((i,x,y) for i,(x,y) in enumerate(zip(a,b)) if x!=y),('length',len(a),len(b)))
    return a,{'files':[p.name for p in paths], 'semantic_sha256':digest(a),'records':len(a),'repeat_identical':True}
w,wa=repeated('reference-inverse-mass-flat-run{r}-v29-windows')
native=compare.load(out/wa['files'][0],'windows-physx-d3d9')
assert native==compare.load(out/'reference-contact-force-flat-run0-v28-windows.jsonl','windows-physx-d3d9')
m,ma=repeated('calibration-inverse-normal-rows-flat-run{r}-v29-mac')
base=compare.load(out/ma['files'][0],'mac-jolt-metal')
old=compare.load(out/'calibration-raw-mass-off-unset-v28-mac.jsonl','mac-jolt-metal')
assert all(base[n]==old[n] for n in base)
inverse={r['case']:r['sample'] for r in w if r['event']=='calibration.inverse-mass'}
first_rows={}
for r in m:
    n=r['case'];s=r['sample']
    if n in names and r['event']=='physics.source-normal-row' and s['step']==names[n]['first_tick']:
        first_rows.setdefault(n,[]).append(s)
for n in names:
    assert len(first_rows[n])==16
    for row in first_rows[n]:
        for i in range(3):assert row['body1_inverse_inertia_axis'+str(i)]==inverse[n]['inverse_inertia_axis'+str(i)]
    t=names[n]['first_tick'];assert native[n]['rows'][:t]==base[n]['rows'][:t]
def errors(x,y):
    return {f:compare.norm(x[f],y[f]) for f in ['position','velocity','angular_momentum','rotation']} | {
      'wheel_speed':compare.rms([a['angular_speed']-b['angular_speed'] for a,b in zip(x['wheels'],y['wheels'])]),
      'wheel_contact':sum(a['contact']!=b['contact'] for a,b in zip(x['wheels'],y['wheels'])),
      'wheel_height':compare.rms([a['height']-b['height'] for a,b in zip(x['wheels'],y['wheels'])]),
      'velocity_z_signed':y['velocity'][2]-x['velocity'][2]}
def analyze(prefix,expected_positions,expected_separations):
    rs,a=repeated(prefix);states=compare.load(out/a['files'][0],'mac-jolt-metal');assert set(states)==set(names)
    first={};replays={};case_rows=Counter()
    for r in rs:
        n=r['case'];s=r['sample']
        if r['event']=='physics.diagnostic-contact-replay':
            assert n not in replays;assert s['step']==names[n]['first_tick'];assert s['points']==8
            assert s['positions_replayed']==expected_positions and s['separations_replayed']==expected_separations
            replays[n]=s
        if r['event']=='physics.source-normal-row':
            case_rows[n]+=1
            if s['step']==names[n]['first_tick']:first.setdefault(n,[]).append(s)
    assert set(replays)==set(names)
    a['cases']={}
    for n,c in states.items():
        t=names[n]['first_tick'];assert c['rows'][:t]==native[n]['rows'][:t]
        assert len(first[n])==16
        assert [(r['phase'],r['index']) for r in first[n]]==[(phase,i) for phase in ['biased-final','unbiased-final'] for i in range(8)]
        solver_dt=struct.unpack('f',struct.pack('f',c['meta']['dt']))[0]
        assert all(struct.unpack('f',struct.pack('f',r['dt']))[0]==solver_dt for r in first[n])
        ee=errors(native[n]['rows'][t],c['rows'][t])
        full=[errors(x,y) for x,y in zip(native[n]['rows'],c['rows'])]
        a['cases'][n]={'first_tick':t,'pre_contact_states_identical':True,'first_contact_errors':ee,
                       'first_contact_state_identical':native[n]['rows'][t]==c['rows'][t],
                       'all_states_identical':native[n]['rows']==c['rows'],
                       'maximum_errors':{k:max(abs(e[k]) for e in full) for k in ee},
                       'source_normal_rows':case_rows[n],'first_normal_rows':first[n]}
    return a,states
report={'schema':1,'scope':'Diagnostic single-step geometry intervention and independent normal-row arithmetic hypotheses. Not standalone Mac parity and not a production fix.',
        'native':wa,'mac_diagnostic':ma,'inverse_inertia_all_first_contact_rows_exact':True,
        'native_physical_states_exact_v28':True,'mac_diagnostic_physical_states_exact_v28':True,
        'interventions':{},'factorial':{},'goal_complete':False}
for mode,p,s in [('off',False,False),('points',True,False),('separation',False,True),('both',True,True)]:
    a,st=analyze('calibration-first-contact-replay-'+mode+'-run{r}-v29-mac',p,s)
    if mode=='off':assert all(st[n]==base[n] for n in names)
    report['interventions'][mode]=a
for geometry in ['points','both']:
    report['factorial'][geometry]={}
    for mask in range(8):
        a,st=analyze('calibration-row-arithmetic-'+geometry+'-mask'+str(mask)+'-run{r}-v29-mac',True,geometry=='both')
        a['flags']={'split_bias':bool(mask&1),'premultiplied_linear':bool(mask&2),'jacobian_mass':bool(mask&4)}
        if mask==0:assert st==compare.load(out/report['interventions'][geometry]['files'][0],'mac-jolt-metal')
        report['factorial'][geometry][mask]=a
        print(geometry,mask,'first Vmax',max(c['first_contact_errors']['velocity'] for c in a['cases'].values()),
              'full Pmax',max(c['maximum_errors']['position'] for c in a['cases'].values()),
              'first exact',sum(c['first_contact_state_identical'] for c in a['cases'].values()),flush=True)
(out/'physics-v29-normal-row-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print('All semantic repeats and pre-contact controls exact. Inverse inertia exact. Goal remains active.')
