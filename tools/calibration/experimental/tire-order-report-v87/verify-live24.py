from pathlib import Path
import json,gzip,struct
root=Path(__file__).resolve().parent;bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];value=lambda x:struct.unpack('<f',struct.pack('<I',x))[0]
p=next((root/'captures/native-kernel').glob('*0-private*.gz'))
with gzip.open(p,'rt') as f:native=[r for r in map(json.loads,f) if r.get('function')=='spring-solve-applied']
assert len(native)==80
checks=[]
for rep in range(2):
 folder=root/'captures/mac-report'
 with gzip.open(folder/f'force-{rep}-solver.jsonl.gz','rt') as f:solver=list(map(json.loads,f))
 with gzip.open(folder/f'force-{rep}-tire.jsonl.gz','rt') as f:tires=[r for r in map(json.loads,f) if r['event'].endswith('result')]
 observed={}
 for r in solver:
  if r['phase']=='ordered-stop-after':index=(r['iteration']-1)*16+(3-r['wheel'])
  elif r['phase']=='spring-after':index=(r['iteration']-1)*16+4+(3-r['wheel'])
  else:continue
  observed[index]=(r['velocity']+r['angular'],r['lambda'])
 for i,r in enumerate(tires):
  index=(i//8)*16+8+i%8
  observed[index]=([r['linear'][0],r['linear'][2],r['linear'][1],-r['angular'][0],-r['angular'][2],-r['angular'][1]],r['lambda'])
 assert len(observed)==80
 for index,(body,lam) in sorted(observed.items()):
  n=native[index];expected=[n['solve_body0']['words'][k] for k in (0,1,2,4,5,6)]
  assert list(map(bits,body))==expected,(rep,index,body,expected)
  expected_lambda=value(n['solve_row']['words'][28]);assert lam==expected_lambda or bits(lam)==bits(expected_lambda),(rep,index,lam,expected_lambda)
 checks.append({'repeat':rep,'body_checkpoints':80,'body_float32_words_exact':480,'lambdas_numerically_exact':80,'private_signed_zero_lambda_parity_claimed':False})
report={'scope':'Continuous live physical24 through5iterations; 20hard stops,20springs,40tire rows. No row reset or injected Windows states. All80body checkpoints and480float32 body words exact per repeat. Numeric lambdas checked; decimal telemetry cannot certify private signed-zero lambda bits.','checks':checks,'native_probe_immutable':True,'goal_complete':False}
print('Live physical24: all80body checkpoints/480body words exact in2repeats;80numeric lambdas exact per repeat; goal incomplete')
