from pathlib import Path
import gzip,json,os,subprocess,struct
root=Path.cwd();out=root/'outputs/tire-anchor-v91/mac-anchor-candidate';assert out.exists();probe=root/'work/frozen-v91/rrr3d_physics_calibration';env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'work/frozen-v91/manifest.json').read_text())['environment']);traces=[];counts=[];bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
def physical(r):
 r=dict(r)
 for f in ['position','velocity','rotation','angular_momentum']:r[f]=list(map(bits,r[f]))
 fields=['angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'];r['wheels']=[{k:bits(v) if k in fields else v for k,v in w.items()} for w in r['wheels']];return r
ref=[r['sample'] for r in map(json.loads,gzip.open(root/'work/sync-rrr3d/tools/calibration/experimental/tire-anchor-lifetime-v91/captures/windows-reference.jsonl.gz','rt')) if r['event']=='calibration.sample' and r['detail']=='mobility/devildriver/engine3/lateral']
for rep in range(2):
 p=out/f'candidate-{rep}.jsonl';trace=out/f'candidate-{rep}-anchor.jsonl';env['RRR3D_TIRE_ANCHOR_TRACE_FILE_V91']=str(trace)
 if rep==1:
  assert not p.exists() and not trace.exists()
  with (out/f'candidate-{rep}.log').open('wb') as log:subprocess.run([str(probe),str(p),'list:mobility/devildriver/engine3/lateral'],env=env,check=True,stdout=log,stderr=log,timeout=180)
 actual=[r['sample'] for r in map(json.loads,p.open()) if r['event']=='calibration.sample'];assert len(actual)==len(ref)==600;assert list(map(physical,actual))==list(map(physical,ref));traces.append([json.loads(s) for s in trace.open()]);lookup={(r['step'],r['wheel']):r for r in traces[-1]}
 native=[json.loads(s) for s in gzip.open(root/f'outputs/tire-anchor-v91/native-anchor-window/force-devildriver-engine3-lateral-58-{rep}-private.jsonl.gz','rt')];gates=[r for r in native if r.get('function')=='tire-anchor-gate'];wheelids={r['ebx']:i for i,r in enumerate(gates[:4])};count=0
 for r in gates:
  tick=r['sample_tick_v71'];wheel=wheelids[r['ebx']];m=lookup[tick,wheel];w=r['wheel_contact']['words'];ready=next(z for z in native if z.get('function')=='tire-anchor-bias-ready' and z.get('sample_tick_v71')==tick and z.get('ebx')==r['ebx']);assert list(map(bits,m['current']))==w[:3];assert list(map(bits,m['stored']))==w[22:25];assert bits(m['lateral_error'])==ready['prepare_frame']['words'][63];assert bits(m['longitudinal_error'])==ready['prepare_frame']['words'][64];count+=1
 counts.append(count);print('Candidate',rep,'all600statesfloat32exactWindows;',count,'actualanchorpoints/errors exactNative',flush=True)
assert traces[0]==traces[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Candidate optional read-only trace. Cross-platform states use unchanged strict float32 contract; decimal printf halfway ties may differ.','two_repeats_exact':True,'all600_physical_states_exact_windows_each':True,'actual_native_anchor_points_errors_exact':counts,'goal_complete':False},indent=2)+'\n')
