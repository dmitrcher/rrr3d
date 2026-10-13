"""Run frozen CLI using the exact tested V64+V69+V70+V71+V72 environment and unchanged params."""
from pathlib import Path
import argparse,json,os,subprocess
p=argparse.ArgumentParser();p.add_argument('--probe',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--cases',default='list:pair-pole-0,pair-pole-1,pair-pole-oblique-0,pair-pole-oblique-1');a=p.parse_args()
assert b'RRR3D_EXPERIMENT_NATIVE_PAIR_SAT_V72' in a.probe.read_bytes(),'Probe lacks the recorded V72 source'
assert not a.out.exists(),'Refusing to overwrite an existing capture'
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((Path(__file__).resolve().parent/'candidate-environment.json').read_text()))
env['RRR3D_CALIBRATION_PROFILE']=str(Path(__file__).resolve().parent/'windows-source-v14.cfg')
subprocess.run([str(a.probe.resolve()),str(a.out.resolve()),a.cases],env=env,check=True)
