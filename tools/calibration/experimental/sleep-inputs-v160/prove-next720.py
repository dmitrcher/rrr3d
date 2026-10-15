from pathlib import Path
import json,gzip,struct
b=lambda f:struct.unpack('<I',struct.pack('<f',f))[0];p=Path('work/sleep-inputs-v160');src=Path('outputs/sleep-inputs-v160/source-track64-step720');manifest=json.load(open('outputs/sleep-inputs-v160/native-wheel720/manifest.json'));assert manifest['repeats']==2
plain=[x['sample'] for x in map(json.loads,open('outputs/sleep-inputs-v160/moving-maps/track64-on0.jsonl')) if x['event']=='solo.sample'];diag=[x['sample'] for x in map(json.loads,open(src/'track64-physical.jsonl')) if x['event']=='solo.sample'];assert len(plain)==900 and plain==diag
prep=[json.loads(s.replace('nan','NaN')) for s in open(src/'track64-prep.jsonl')];sp=[x for x in map(json.loads,open(src/'track64-solver.jsonl')) if x['phase']=='spring-before'];byjac={tuple(map(b,x['spring']['jacobian'])):x for x in prep};assert len(byjac)==4;checks=[]
for rep in [0,1]:
 n=[x for x in map(json.loads,gzip.open(f'outputs/sleep-inputs-v160/native-wheel720/track64-run{rep}-private.jsonl.gz','rt')) if x.get('function')=='spring-solve-input'];assert len(n)==60;mapping=[]
 for x in n[:4]:
  w=x['solve_row']['words'];z=byjac[tuple(w[12:15])];assert w[11]==b(z['spring']['mass']);assert w[20:23]==list(map(b,z['spring']['inverse_jacobian']));mapping.append(z['wheel'])
 a=n[0]['solve_body0']['words'];assert a[:3]+a[4:7]==list(map(b,sp[0]['velocity']+sp[0]['angular']))
 checks.append(dict(repeat=rep,all4prepared_jacobians_mass_inverse_jacobians_exact=True,starting_body_velocity_angular_exact=True,native_first4=mapping,source_first4=[x['wheel'] for x in sp[:4]]))
report=dict(scope='Two read-only original720 captures preserve every900immutable public states and hashes. Prepared springs uniquely bound by exact Jacobian, not index. Measured orders are evidence, not runtime inputs.',checks=checks,diagnostic900states_unchanged=True,goal_complete=False);(p/'next720-order-proof.json').write_text(json.dumps(report,indent=2)+'\n');print(checks)
