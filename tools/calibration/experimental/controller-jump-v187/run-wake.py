"""Source wake regression with synthetic GameCar contact booleans, real Jolt body.

This is a lifecycle contract test; it does not claim native contact observations.
"""
from pathlib import Path
import hashlib,json,os,struct,subprocess
root=Path.cwd();out=root/'outputs/controller-jump-v187/wake';out.mkdir(exist_ok=True)
exe=root/'work/controller-jump-v187/SourceJumpWakeProbe'
base=json.loads((root/'outputs/controller-clutch-v186/final-broad/manifest.json').read_text())['environment']
word=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
reports=[]
for wake_tick in range(51):
    prefix=out/f'wake{wake_tick}'
    inputs=prefix.with_suffix('.words')
    inputs.write_text(' '.join(map(str,[wake_tick,*map(word,[0,0,0,1,0,0,0,1,1,1]),0,0,0,0]))+'\n')
    env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base)
    env.update(RRR3D_EXPERIMENT_GAMECAR_JUMP_V187='1',RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128='1',RRR3D_ENERGY_SLEEP_TRACE_FILE_V128=str(prefix.with_suffix('.energy.jsonl')))
    prefix.with_suffix('.energy.jsonl').unlink(missing_ok=True)
    path=prefix.with_suffix('.jsonl')
    with prefix.with_suffix('.log').open('w') as log:
        subprocess.run([str(exe),str(inputs),str(path)],env=env,stdout=log,stderr=log,check=True,timeout=30)
    rows=list(map(json.loads,path.read_text().splitlines()));assert len(rows)==wake_tick+1
    before=rows[:-1];after=rows[-1]
    energy=list(map(json.loads,prefix.with_suffix('.energy.jsonl').read_text().splitlines()))
    pending=any(r['step']==wake_tick-1 and r['pending'] for r in energy)
    inactive=bool(before) and not before[-1]['body_awake']
    good=after['body_awake'] and after['velocity'][2]==word(-20.0*(1.0/60.0))
    reports.append(dict(wake_tick=wake_tick,pending_before=pending,inactive_before=inactive,awake_after=after['body_awake'],vz_word=after['velocity'][2],exact=good))
report=dict(scope=__doc__.strip(),runs=reports,pending_boundary_cases=[r['wake_tick'] for r in reports if r['pending_before']],inactive_cases=[r['wake_tick'] for r in reports if r['inactive_before']],failures=[r for r in reports if not r['exact']],probe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),goal_complete=False)
(out/'comparison.json').write_text(json.dumps(report,indent=2)+'\n')
print('failures',report['failures'],'pending boundaries',report['pending_boundary_cases'],'inactive cases',report['inactive_cases'],flush=True)
assert report['pending_boundary_cases'] and report['inactive_cases'],'Sleep boundaries were not exercised'
assert not report['failures'],'Jump acceleration lost at sleep/wake boundary'
