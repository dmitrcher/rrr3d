#!/usr/bin/env python3
"""Fit opt-in Mac settings to Windows; reject profiles that regress holdouts."""
import argparse
import json
import math
import os
import subprocess
from pathlib import Path
from compare import compare, load, norm

TRAIN=('force','gravity','wheel-drive','brake','settle')
HOLDOUT=('force-heavy','impulse','spin','coast','steer-left','steer-right','lateral','suspension-drop')


def score(report, reference, names):
    values=[]
    for name in names:
        rows=reference[name]['rows']; first=rows[0]['position']
        distance=max(1.,max(norm(s['position'],first) for s in rows))
        speed=max(1.,max(norm(s['velocity'],[0,0,0]) for s in rows))
        r=report['cases'][name]
        values.append(r['rms']['position_error']/distance + r['rms']['velocity_error']/speed + r['rms']['rotation_error_radians']/math.pi + .25*r['wheel_contact_mismatch_fraction'])
    return sum(values)/len(values)


def fit(reference_path, binary, destination):
    out=Path(destination).resolve();out.mkdir(parents=True,exist_ok=True)
    reference=load(reference_path,'windows-physx-d3d9')
    if set(reference)!=set(TRAIN+HOLDOUT):raise ValueError('Fitting requires all 13 cases')
    settings={'solver_substeps':2,'gravity_scale':1.,'torque_scale':1.,'brake_scale':1.,'spring_scale':1.,'damper_scale':1.,'longitudinal_grip_scale':1.,'lateral_grip_scale':1.,'body_friction_scale':1.}
    history=[]
    def evaluate(params,label):
        profile=out/'trial-profile.cfg';profile.write_text(''.join(f'{k} {v}\n' for k,v in params.items()))
        trace=out/'trial.jsonl'
        env=dict(os.environ,RRR3D_CALIBRATION_PROFILE=str(profile))
        subprocess.run([str(Path(binary).resolve()),str(trace),'all'],env=env,check=True,timeout=90)
        r=compare(reference_path,trace)
        train=score(r,reference,TRAIN);validation=score(r,reference,HOLDOUT)
        history.append({'label':label,'settings':params.copy(),'training_score':train,'holdout_score':validation})
        return train,validation,trace.read_bytes(),r
    base_train,base_valid,base_bytes,base_report=evaluate(settings,'baseline')
    best_train,best_valid,best_bytes,best_report=base_train,base_valid,base_bytes,base_report
    # Keep search deliberately small and reviewable. Grip/material values are
    # available for manual experiments; do not fit them to one synthetic road.
    grid={'solver_substeps':(1,2,4),'gravity_scale':(.95,1.,1.05),'spring_scale':(.8,1.,1.2,1.5),'damper_scale':(.8,1.,1.2),'torque_scale':(.8,1.,1.2),'brake_scale':(.8,1.,1.2),'longitudinal_grip_scale':(.2,.5,.8,1.,1.2),'lateral_grip_scale':(.2,.5,.8,1.,1.2)}
    for key,values in grid.items():
        winner=settings.copy()
        for value in values:
            if value==settings[key]:continue
            trial=settings.copy();trial[key]=value
            train,valid,raw,r=evaluate(trial,f'{key}={value}')
            # Preserve the already-correct force/gravity responses. A ground
            # fit must not compensate a tire error by changing gravity.
            rigid_ok=all(r['cases'][n]['maximum']['velocity_error'] <= 1e-3 and r['cases'][n]['maximum']['position_error'] <= base_report['cases'][n]['maximum']['position_error']+1e-3 for n in ('force','gravity'))
            history[-1]['rigid_body_guard_passed']=rigid_ok
            if rigid_ok and train<best_train-1e-6:
                best_train,best_valid,best_bytes,best_report=train,valid,raw,r;winner=trial
        settings=winner
    accepted=best_train<base_train and best_valid<=base_valid*1.02+1e-6
    candidate=out/'candidate-profile.cfg';candidate.write_text('# Fitted on force/gravity/drive/brake/settle; validate real races separately.\n'+''.join(f'{k} {v}\n' for k,v in settings.items()))
    (out/'baseline.jsonl').write_bytes(base_bytes);(out/'candidate.jsonl').write_bytes(best_bytes)
    compare(reference_path,out/'baseline.jsonl',out/'baseline')
    compare(reference_path,out/'candidate.jsonl',out/'candidate')
    result={'training_cases':TRAIN,'held_out_cases':HOLDOUT,'baseline_training_score':base_train,'candidate_training_score':best_train,'baseline_holdout_score':base_valid,'candidate_holdout_score':best_valid,'synthetic_holdout_accepted':accepted,'settings':settings,'history':history,'policy':'No writes to game defaults. Only promote a reviewed opt-in profile after synthetic holdouts and real race validation.'}
    (out/'fit-report.json').write_text(json.dumps(result,indent=2)+'\n')
    (out/'trial.jsonl').unlink();(out/'trial-profile.cfg').unlink()
    return result


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('windows');p.add_argument('mac_binary');p.add_argument('--out',required=True);a=p.parse_args()
    try:
        r=fit(a.windows,a.mac_binary,a.out)
        print(json.dumps({k:v for k,v in r.items() if k!='history'},indent=2))
    except (OSError,ValueError,subprocess.SubprocessError) as e:p.exit(2,str(e)+'\n')
if __name__=='__main__':main()
