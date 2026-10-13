from pathlib import Path
import gzip,json,os,struct,subprocess
root=Path.cwd();work=root/'work/creation-order-v99';bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];flt=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
def load(p):
    if not p.exists():p=Path(str(p)+'.gz')
    with (gzip.open(p,'rt') if str(p).endswith('.gz') else p.open()) as f:return list(map(json.loads,f))
fixtures=[]
for rep in range(2):
    pub=load(root/f'outputs/creation-order-v99-verified/control-inventory-{rep}.jsonl')
    pose=next(x['sample'] for x in pub if x.get('event')=='calibration.local-points' and x['detail']=='border-front-grounded' and x['sample']['tick']==49)
    native=load(root/f'outputs/solver-compose-v98/native-next-border49/v98-next-border49-border-front-grounded-{rep}-private.jsonl')
    raw=next(x for x in native if x.get('function')=='converted-create-entry')
    prepared=[x['native_solver_row']['words'] for x in native if x.get('function')=='solver-initialize-exit'][:4]
    actor=sum([pose[f'point_frame1_axis{i}'] for i in range(3)],[])+pose['point_frame1_position']
    ground=sum([pose[f'point_frame0_axis{i}'] for i in range(3)],[])+pose['point_frame0_position']
    worlds=sum([[flt(w) for w in raw[f'converter_raw_{i}']['words'][:3]] for i in range(4)],[])
    # Original raw normal is +X for static0/dynamic1, recovered independently
    # from Native local frame0 identity and callback pre-modify patch data.
    expected=sum([raw[f'converter_raw_{i}']['words'][:3]+raw[f'converter_raw_{i}']['words'][6:9] for i in range(4)],[])+sum([x[8:11] for x in prepared],[])
    fixtures.append({'repeat':rep,'actual_native_input_words':list(map(bits,actor+worlds+[1,0,0]+ground)),'actual_native_output_words':expected})
data='\n'.join(' '.join(format(flt(w),'.9g') for w in f['actual_native_input_words']) for f in fixtures)+'\n'
inc=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics')
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(RRR3D_EXPERIMENT_NATIVE_CACHE_LOCAL_POINT_ORDER='1',RRR3D_EXPERIMENT_NATIVE_CACHE_LEVER_ORDER='1');reports=[]
for opt in ['O0','O2']:
    exe=work/f'cache-{opt}'
    subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(inc),str(work/'cache-probe.cpp'),'-o',str(exe)],check=True)
    correct=[list(map(bits,map(float,l.split()))) for l in subprocess.check_output([str(exe)],input=data.encode(),env=env).decode().splitlines()]
    old=[list(map(bits,map(float,l.split()))) for l in subprocess.check_output([str(exe),'old'],input=data.encode(),env=env).decode().splitlines()]
    assert correct==[f['actual_native_output_words'] for f in fixtures]
    mismatches=sum(a!=b for row,f in zip(old,fixtures) for a,b in zip(row,f['actual_native_output_words']));assert mismatches>0
    reports.append({'optimization':opt,'actual_native_output_words_exact':72,'old_dynamic_body0_mismatching_words':mismatches})
(root/'outputs/creation-order-v99-cache-arithmetic-proof.json').write_text(json.dumps({'scope':'Production NativeContactCache.h on independently captured original Windows static0/dynamic1 matrices, positions and four world points. Exact raw canonical local positions and solver body1 levers. Two physical repeats of one input domain; no broad domain certification.','runs':reports,'fixtures':fixtures,'goal_complete':False},indent=2)+'\n')
print(reports)
