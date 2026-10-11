#!/usr/bin/env python3
"""Check the portable callback against native Windows input/output records."""
import argparse
import hashlib
import json
import math
import struct
import subprocess
from pathlib import Path

def f32(x): return struct.unpack('f', struct.pack('f', x))[0]
def vector(v): return [f32(x) for x in v]
def cross(a,b):
    return [f32(a[1]*b[2]-a[2]*b[1]), f32(a[2]*b[0]-a[0]*b[2]), f32(a[0]*b[1]-a[1]*b[0])]
def normal(vertices):
    a,b,c = [vector(v) for v in vertices]
    n = cross([f32(x-y) for x,y in zip(b,a)], [f32(x-y) for x,y in zip(c,a)])
    length = f32(math.sqrt(f32(sum(x*x for x in n))))
    scale = f32(1/length)
    return [f32(x*scale) for x in n]

def records(path):
    input_rows=[];expected=[];names=[];pending=None;modified=None
    with Path(path).open() as stream:
        for line in stream:
            r=json.loads(line);s=r.get('sample',{});key=(r['detail'],s.get('tick'))
            if r['event']=='calibration.body-points' and s.get('triangle'):
                assert pending is None, 'Incomplete previous callback'
                assert 'local_quaternion0' in s and 'actor0_axis0' in s, 'Need NX_CONTACT_QUAT_TRACE=1'
                pending=(key,s);modified=None
            elif r['event']=='calibration.body-contact' and pending:
                assert key==pending[0];modified=s
            elif r['event']=='calibration.contact-quaternion' and pending:
                assert key==pending[0] and modified is not None
                before=pending[1]
                matrices=[]
                for actor in range(2):
                    cols=[vector(before[f'actor{actor}_axis{i}']) for i in range(3)]
                    matrices.extend(cols[j][i] for i in range(3) for j in range(3))
                inputs=[int(before['car_shape']==0)] + vector(before['local_quaternion0']) + matrices + normal(before['triangle_vertices'])
                output=[]
                output.extend(x for i in range(3) for x in vector(before['axis'+str(i)]))
                output.extend(x for i in range(3) for x in vector(modified['after_axis'+str(i)]))
                output.extend(vector(s['local_quaternion0']));output.extend(vector(s['local_quaternion1']))
                output.extend(vector(s['decoded_local_normal']))
                input_rows.append(inputs);expected.append(output);names.append(key);pending=None
    assert pending is None and expected, 'Empty/incomplete callback captures'
    return input_rows,expected,names

def verify(helper, paths):
    result={}
    for path in paths:
        inputs,expected,names=records(path)
        raw=''.join(' '.join(str(x) for x in row)+'\n' for row in inputs)
        done=subprocess.run([str(Path(helper).resolve())], input=raw, text=True, capture_output=True, check=True, timeout=30)
        actual=[vector([float(x) for x in line.split()]) for line in done.stdout.splitlines()]
        assert len(actual)==len(expected), 'Missing output'
        for i,(a,b) in enumerate(zip(actual,expected)):
            assert len(a)==len(b)==29, 'Unexpected output shape'
            assert a==b, (names[i],[(j,x,y) for j,(x,y) in enumerate(zip(a,b)) if x!=y])
        result[str(path)]={'callbacks':len(expected),'exact_float32_channels':29*len(expected),
                          'all_equal':True,'native_trace_sha256':hashlib.sha256(Path(path).read_bytes()).hexdigest()}
    return {'scope':'Callback math given native material seed and body matrices; not material-seed generation, solver-normal decoding or complete physics parity',
            'helper_sha256':hashlib.sha256(Path(helper).read_bytes()).hexdigest(),'captures':result}

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--helper',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    p.add_argument('traces',type=Path,nargs='+');args=p.parse_args()
    report=verify(args.helper,args.traces);args.out.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))
if __name__=='__main__':main()
