#!/usr/bin/env python3
from pathlib import Path
import json,math,struct,importlib.util,argparse
parser=argparse.ArgumentParser(description='Audit native momentum trace with float32 inputs; does not certify the solver')
parser.add_argument('trace',type=Path);parser.add_argument('--repeat',type=Path,required=True);parser.add_argument('--baseline',type=Path,required=True);parser.add_argument('--out',type=Path,required=True);args=parser.parse_args();files=[args.trace,args.repeat]
spec=importlib.util.spec_from_file_location('c',Path(__file__).resolve().parents[1]/'compare.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
a=c.load(files[0],'windows-physx-d3d9');b=c.load(files[1],'windows-physx-d3d9');assert a==b
base=c.load(args.baseline,'windows-physx-d3d9');assert set(a)==set(base)
for name in a:
 for field in ['rows','calibration.engine-description','calibration.peer-description']:assert a[name].get(field)==base[name].get(field)
def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def native(x):return [native(i) for i in x] if isinstance(x,list) else f32(x)
results={};count=0
for line in files[0].open():
 r=json.loads(line)
 if r['event']!='calibration.angular-momentum':continue
 s=r['sample'];count+=1
 case=results.setdefault(r['detail'],{'samples':0,'max_public_tensor_momentum_error':0,'max_float_dot_error':0,'max_double_dot_error':0,'max_current_principal_double_error':0})
 case['samples']+=1
 omega=native(s['angular_velocity']);L=native(s['momentum']);cols=native(s['world_inertia_columns']);R=native(s['mass_orientation_columns']);I=native(s['mass_inertia'])
 dotfloat=[f32(f32(f32(cols[0][i]*omega[0])+f32(cols[1][i]*omega[1]))+f32(cols[2][i]*omega[2])) for i in range(3)]
 dotdouble=[f32(sum(cols[j][i]*omega[j] for j in range(3))) for i in range(3)]
 local=[sum(R[j][i]*omega[i] for i in range(3)) for j in range(3)]
 principal=[sum(R[j][i]*I[j]*local[j] for j in range(3)) for i in range(3)]
 for field,x in [('max_public_tensor_momentum_error',native(s['tensor_momentum'])),('max_float_dot_error',dotfloat),('max_double_dot_error',dotdouble),('max_current_principal_double_error',principal)]:case[field]=max(case[field],math.sqrt(sum((v-w)**2 for v,w in zip(x,L))))
report={'native_repeat_identical':True,'all_physical_rows_and_mass_properties_identical_to_v14':True,'cases':len(a),'samples':count,'per_case':results,'scope':'Float32 inputs recovered from nine-digit native trace. Current principal double formula is an approximate consistency check, not the exact SDK momentum arithmetic. Both float/double world-tensor products have residuals on some cases. No general exact model accepted.'}
args.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
