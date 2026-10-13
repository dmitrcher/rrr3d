from pathlib import Path
import json,gzip,struct
root=Path.cwd();out=root/'outputs/combined-tire-v95';bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];checks=[]
for rep in range(2):
 path=next((out/'native-force245').glob(f'*-{rep}-private.jsonl.gz'));rows=list(map(json.loads,gzip.open(path,'rt')))
 arrays=[[r for r in rows if r.get('function')==fun]for fun in ['tire-lateral-hermite-result','tire-longitudinal-hermite-result','tire-force-scaled']]
 mac=list(map(json.loads,open(out/f'mac-readonly245/marauder-wheel1-brake-245-{rep}-combined.jsonl')));assert len(mac)==4;bywheel={r['wheel']:r for r in mac}
 for wheel,(lat,lon,scaled)in enumerate(zip(*arrays)):
  m=bywheel[wheel];params=lon['wheel_parameters']['words'];assert m['physical_tick']==245 and m['has_contact'] and m['flags']==64
  actual=[bits(m[k])for k in ['longitudinal','lateral','extremum_longitudinal','extremum_lateral','projected_force_impulse','normal_force','delta']]
  native=[lon['xmm_words'][0][0]&0x7fffffff,lat['xmm_words'][0][0]&0x7fffffff,params[7],params[13],scaled['stack']['words'][6],scaled['wheel_contact']['words'][12],scaled['xmm_words'][2][0]]
  assert actual==native,(rep,wheel,[(i,a,b)for i,(a,b)in enumerate(zip(actual,native))if a!=b]);checks.append({'repeat':rep,'wheel':wheel,'seven_actual_input_words_exact':True,'mac':m})
assert len(checks)==8
report={'scope':'Actual live Mac first-iteration TireMaxImpulse callback physical245/SDK246 and OriginalNative same-input traces. Both raw curve coefficients, serialized extrema, projected force impulse, normal force and dt match for every actual wheel in both repeats. Trace-only Mac1200physical states preserve V94. No oracle inputs/reset or physics change.','checks':checks,'goal_complete':False};(out/'mac-live-input-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Actual8Mac wheel callbacks match Native7input float32 words each;56words exact;1200V94states unchanged')
