#!/usr/bin/env python3
"""Compare real Windows SSE estimates to real Mac NEON refinement outputs."""
import argparse,json,math,struct
from pathlib import Path

def load(path,columns):
    rows=[]
    for number,line in enumerate(path.read_text().splitlines(),1):
        values=[struct.unpack('f',struct.pack('f',float(v)))[0] for v in line.split()]
        if len(values)!=columns or not all(math.isfinite(v) for v in values):
            raise ValueError(f'{path}:{number}: malformed or nonfinite numerical record')
        if values[0]<=0:raise ValueError(f'{path}:{number}: positive normal inputs required')
        rows.append(values)
    if not rows:raise ValueError(f'{path}: empty capture')
    return rows

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['windows0','windows1','mac0','mac1']:p.add_argument(name,type=Path)
    p.add_argument('--out',type=Path,required=True);args=p.parse_args()
    windows=[load(getattr(args,'windows'+str(i)),27) for i in range(2)]
    mac=[load(getattr(args,'mac'+str(i)),9) for i in range(2)]
    if windows[0]!=windows[1] or mac[0]!=mac[1]:raise ValueError('Numerical repeats differ')
    if len(windows[0])!=len(mac[0]):raise ValueError('Input counts differ')
    bad=[]
    for w,m in zip(windows[0],mac[0]):
        if w[0]!=m[0]:raise ValueError('Input order or float32 values differ')
        if w[-2]!=m[4]:bad.append({'input':w[0],'windows':w[-2],'mac':m[4]})
    report={'inputs':len(windows[0]),'all_numeric_repeats_exact':True,
        'neon_estimate_with_two_squared_estimate_refinements_exact':len(windows[0])-len(bad),
        'nonexact':len(bad),'examples':bad[:5],
        'scope':'Observed Windows ARM VM x86 SSE and Mac ARM NEON instructions. This finite set does not prove all float values or other Windows CPUs.'}
    args.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
    return bool(bad)

if __name__=='__main__':raise SystemExit(main())
