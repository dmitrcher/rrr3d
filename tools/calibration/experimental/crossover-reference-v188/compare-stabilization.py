"""Compare explicit uint32 physical words; never recast their integer values as floats."""
from pathlib import Path
import argparse,collections,gzip,hashlib,json
parser=argparse.ArgumentParser();parser.add_argument('capture');parser.add_argument('reference');parser.add_argument('report');args=parser.parse_args()
def read(p):
 with gzip.open(p,'rt') if p.suffix=='.gz' else p.open() as f:return [json.loads(s) for s in f if 'stabilize.pose' in s]
a,b=read(Path(args.capture)),read(Path(args.reference));assert len(a)==len(b)==1440
initial=['rotation_before','omega_before','momentum_before','inertia','inverse_inertia','mass_word','raw_fp_control','damping','roll_word','pitch_word','clutch_time_word','clutch_strength_word']
physical=['rotation_after','omega_after','momentum_after','rotation_after_step','omega_after_step','momentum_after_step','position_after_step','clutch_strength_after_word']
def differences(keys):return [dict(case=x['case'],fields=[k for k in keys if x[k]!=y[k]]) for x,y in zip(a,b) if any(x[k]!=y[k] for k in keys)]
assert all(x['case']==y['case'] for x,y in zip(a,b));bad=differences(physical);incoming=differences(initial)
# Regression against the exact failure mode of converting word integers to float32.
assert 1065353215 != 1065353216
import struct
assert struct.pack('<f',1065353215)==struct.pack('<f',1065353216)
report=dict(cases=len(a),exact=len(a)-len(bad),initial_differences=incoming,first_differences=bad[:8],field_counts=dict(collections.Counter(k for d in bad for k in d['fields'])),comparison='Exact integer/uint32 physical words, including signed zero. Checkpoint locals are diagnostic and excluded from physical acceptance.',capture=args.capture,reference=args.reference,source_hashes={str(p):hashlib.sha256(Path(p).read_bytes()).hexdigest() for p in [args.capture,args.reference]},full_goal_complete=False)
Path(args.report).write_text(json.dumps(report,indent=2)+'\n');print('exact',report['exact'],'/1440; initial differences',len(incoming))
