"""Verify original mass arithmetic and retained parity; does not promote the candidate."""
import argparse, hashlib, json, struct, subprocess, sys, tempfile
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--package',type=Path,required=True);a=p.parse_args()
root=a.package.resolve();here=Path(__file__).resolve().parent;sys.dont_write_bytecode=True
sys.path.insert(0,str(here.parents[1]));import compare
sys.path.insert(0,str(here.parent));from verify_isolation import capture,verify
bits=lambda v:struct.unpack('<I',struct.pack('<f',float(v)))[0]
def events(path):return [json.loads(line) for line in path.read_text().splitlines()]
def protocol(path,platform):return compare.load(path,platform)
manifest=json.loads((root/'manifest.json').read_text())
for name,sha in manifest['files'].items():assert hashlib.sha256((root/name).read_bytes()).hexdigest()==sha,name
m=root/'measurements';native=[];shape=[];compound=[]
for repeat in range(2):
    n=protocol(m/'shapes'/f'untraced-run{repeat}.jsonl','windows-physx-d3d9')
    assert protocol(m/'shapes'/f'physical-run{repeat}.jsonl','windows-physx-d3d9')==n
    assert protocol(m/'native24'/f'untraced-run{repeat}.jsonl','windows-physx-d3d9')==n
    assert protocol(m/'native24'/f'physical-run{repeat}.jsonl','windows-physx-d3d9')==n
    native.append(n)
    shape.append([r['shape_integrals']['words'] for r in events(m/'shapes'/f'mass-run{repeat}.jsonl') if 'shape_integrals' in r])
    ev=events(m/'native24'/f'mass-run{repeat}.jsonl')
    before=[r['dense_inertia']['words'] for r in ev if 'dense_inertia' in r]
    after=[r['principal_inertia']['words']+r['principal_axes']['words'] for r in ev if 'principal_inertia' in r]
    assert len(before)==len(after)==24
    compound.append([b+c for b,c in zip(before,after)])
assert native[0]==native[1] and shape[0]==shape[1] and compound[0]==compound[1]
assert len(shape[0])==120
begins=[r for r in events(m/'shapes'/'untraced-run0.jsonl') if r.get('event')=='calibration.begin']
shape_input=[];compound_input=[]
for r in begins:
    fp=r['sample']['fixture_parameters'];lines=['0 '+' '.join(map(str,fp['half_extents']+fp['shape_position']))]
    for w in fp['wheels']:
        assert w['flags']==64,'This oracle covers the ordinary radius-high wheel mass provider'
        lines.append('1 '+' '.join(map(str,[w['radius'],w['radius']*.5,0]+w['position'])))
    shape_input.extend(lines);compound_input.append(f"{len(lines)} {r['sample']['mass']}");compound_input.extend(lines)
assert '\n'.join(shape_input)+'\n'==(here/'shape-inputs.txt').read_text()
assert '\n'.join(compound_input)+'\n'==(here/'compound-inputs.txt').read_text()
summary={'actual_shape_records_exact':120,'actual_shape_fields_exact':1560,
         'compound_and_principal_cases_exact':24,'compound_and_principal_fields_exact':504,
         'repeats':2,'runtime_correction_accepted':False,'goal_complete':False,'scopes':{}}
with tempfile.TemporaryDirectory() as d:
    for opt in ['O0','O2']:
        for name,expected in [('shape',shape[0]),('compound',compound[0])]:
            binary=Path(d)/(name+'-'+opt)
            subprocess.run(['xcrun','clang++','-std=c++20','-'+opt,'-I'+str(here/'src/Rock3dEngine/header/physics'),str(here/(name+'-probe.cpp')),'-o',str(binary)],check=True)
            result=subprocess.check_output([str(binary)],input=(here/(name+'-inputs.txt')).read_bytes()).decode()
            rows=[[bits(v) for v in line.split()] for line in result.splitlines()]
            assert rows==expected,(name,opt)
for scope,reference,expected in [('mac24',native[0],6),('long72',protocol(m/'reference-long72.jsonl','windows-physx-d3d9'),72),('old6',protocol(m/'reference-old6.jsonl','windows-physx-d3d9'),6)]:
    loaded={};capture_manifest=json.loads((m/scope/'capture-manifest.json').read_text())
    assert capture_manifest['probe_sha256']==hashlib.sha256((root/'bin/Mac/SpringProbe').read_bytes()).hexdigest()
    for mode in ['baseline','control','ordinary','diagnostic']:
        runs=[protocol(m/scope/f'calibration-shape-{mode}-run{r}-mac.jsonl','mac-jolt-metal') for r in range(2)]
        assert runs[0]==runs[1];loaded[mode]=runs[0]
    assert loaded['baseline']==loaded['control']
    for mode in ['ordinary','diagnostic']:
        exact=[n for n in reference if reference[n]['rows']==loaded[mode][n]['rows']]
        assert len(exact)==expected,(scope,mode)
        summary['scopes'][scope+'-'+mode]={'exact':len(exact),'total':len(reference),'remaining':[n for n in reference if n not in exact]}
    if scope=='mac24':
        for i in range(6):assert reference[f'pose-v45-inverted-{i}']['rows']==loaded['ordinary'][f'pose-v45-inverted-{i}']['rows']
        for n in reference:
            if n.endswith('/spin-mixed'):
                error=lambda mode:max(compare.norm(x['position'],y['position']) for x,y in zip(reference[n]['rows'],loaded[mode][n]['rows']))
                assert error('diagnostic')<error('ordinary'),n
        n='monstertruckBoss/spin-mixed'
        summary['boss_position_errors']={mode:max(compare.norm(x['position'],y['position']) for x,y in zip(reference[n]['rows'],loaded[mode][n]['rows'])) for mode in loaded}
isolation_manifest=json.loads((m/'isolation/isolation-run.json').read_text())
assert isolation_manifest['probe_sha256']==hashlib.sha256((root/'bin/Mac/SpringProbe').read_bytes()).hexdigest()
count=0
for stage in range(4):
    n=[m/'isolation-reference'/f'isolation-stage{stage:02d}-run{r}-windows.jsonl' for r in range(2)]
    mac=[m/'isolation'/f'isolation-stage{stage:02d}-run{r}-mac.jsonl' for r in range(2)]
    report=verify(n[0],n[1],mac[0],mac[1]);assert report['exact_parity']['complete'];count+=report['captures'][0]['cases']
assert count==189;summary['force_isolation_exact']=count
with tempfile.TemporaryDirectory() as d:
    tmp=Path(d);descriptor=json.loads((here.parent/'native-shape-mass-v48-base.json').read_text())
    for rel,sha in descriptor['files'].items():
        if sha['before'] is not None:
            src=here/'before'/rel;assert hashlib.sha256(src.read_bytes()).hexdigest()==sha['before']
            dest=tmp/rel;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(src.read_bytes())
    subprocess.run(['git','init','-q',str(tmp)],check=True)
    subprocess.run(['git','-C',str(tmp),'apply','--whitespace=nowarn',str(here.parent/'native-shape-mass-v48.patch')],check=True)
    for rel,sha in descriptor['files'].items():assert (tmp/rel).read_bytes()==(here/rel).read_bytes();assert hashlib.sha256((tmp/rel).read_bytes()).hexdigest()==sha['after']
summary['patch_sources_byte_exact']=True
print(json.dumps(summary,indent=2))
