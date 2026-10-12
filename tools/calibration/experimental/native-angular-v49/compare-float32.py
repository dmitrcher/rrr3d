"""Compare actual float32 observables, preserving strict JSON results separately.
Both probes print nine significant digits, with different decimal halfway tie
rounding (MSVC2008 printf vs libc++). Cast back to the declared engine float32
format and compare all 32 bits. This applies no numerical tolerance.
"""
import argparse,json,struct,sys
from pathlib import Path
here=Path(__file__).resolve();calibration=next((p for p in [here.parent,here.parent.parent,here.parent.parent.parent] if (p/'compare.py').exists()),None)
if calibration:sys.path.insert(0,str(calibration))
else:sys.path.insert(0,str(Path.cwd()/'work/sync-rrr3d/tools/calibration'))
import compare
VECTORS=('position','velocity','angular_momentum','rotation')
WHEEL_FLOATS=('angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse')
def bits(x):return struct.unpack('<I',struct.pack('<f',float(x)))[0]
def physical(row):
 result=dict(row)
 for field in VECTORS:result[field]=[bits(x) for x in row[field]]
 result['wheels']=[{k:(bits(v) if k in WHEEL_FLOATS else v) for k,v in wheel.items()} for wheel in row['wheels']]
 return result

def audit(reference,candidate):
 a=compare.load(reference,'windows-physx-d3d9');b=compare.load(candidate,'mac-jolt-metal');assert set(a)==set(b)
 result={'comparison':'exact IEEE-754 float32 words; no tolerance','cases':{},'decimal_only_differences':[]}
 for name in a:
  assert len(a[name]['rows'])==len(b[name]['rows'])
  dif=[]
  for x,y in zip(a[name]['rows'],b[name]['rows']):
   if physical(x)!=physical(y):dif.append(x['tick'])
   if x!=y and physical(x)==physical(y):result['decimal_only_differences'].append({'case':name,'tick':x['tick']})
  result['cases'][name]={'samples':len(a[name]['rows']),'difference_ticks':dif,'exact':not dif}
 result['cases_exact']=[n for n,c in result['cases'].items() if c['exact']];result['cases_remaining']=[n for n,c in result['cases'].items() if not c['exact']]
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('windows');p.add_argument('mac');p.add_argument('--out',required=True);args=p.parse_args();r=audit(args.windows,args.mac);Path(args.out).write_text(json.dumps(r,indent=2)+'\n');print('float32 exact',len(r['cases_exact']),'/',len(r['cases']),'decimal-only',len(r['decimal_only_differences']),'remaining',r['cases_remaining'])
