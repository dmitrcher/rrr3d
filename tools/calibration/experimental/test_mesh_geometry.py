#!/usr/bin/env python3
"""Check experimental first manifolds against independently captured Nx geometry."""
import json, subprocess, sys, math
from pathlib import Path

def main():
    fixture=json.loads(Path(sys.argv[2]).read_text())
    cases=fixture['cases']
    inputs=[c['input'] for c in cases]+[fixture['negative_control']['input']]
    command=''.join(' '.join(str(x) for x in row)+'\n' for row in inputs)
    result=subprocess.run([sys.argv[1]],input=command,capture_output=True,text=True,check=True,timeout=30)
    lines=iter(result.stdout.splitlines());checked=0
    for case in cases:
        count=int(next(lines))
        if count!=len(case['points']):raise AssertionError(f"{case['name']}: {count} points vs Nx {len(case['points'])}")
        for index,(point,separation,triangle) in enumerate(zip(case['points'],case['separations'],case['triangle_indices'])):
            values=[float(v) for v in next(lines).split()]
            if len(values)!=5 or not all(math.isfinite(v) for v in values):raise AssertionError('Nonfinite/wrong geometry')
            # Large Nx triangles clip edges at float precision. Point order is
            # compared directly; no nearest-point matching or sorting is used.
            error=math.dist(values[:3],point)
            if error>0.0001 or abs(values[3]-separation)>0.000003:
                raise AssertionError(f"{case['name']} point {index}: position {error}, separation {values[3]-separation}")
            if values[4]!=triangle:raise AssertionError(f"{case['name']} point {index}: triangle {values[4]} vs Nx {triangle}")
            checked+=1
    if int(next(lines))!=fixture['negative_control']['expected_count']:raise AssertionError('Separated floor emits an early contact')
    if next(lines,None) is not None:raise AssertionError('Extra output')
    print(f'{len(cases)} independent Nx first manifolds, {checked} ordered points, separated-floor negative control passed')
if __name__=='__main__':main()
