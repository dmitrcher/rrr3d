"""Run frozen CLI using the exact tested V64+V69 environment and unchanged params."""
from pathlib import Path
import argparse,json,os,subprocess
p=argparse.ArgumentParser();p.add_argument('--probe',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--cases',default='list:border-grounded,border-air-straight');a=p.parse_args()
assert b'RRR3D_EXPERIMENT_NATIVE_STATIC_FRICTION_V69' in a.probe.read_bytes(),'Probe lacks the recorded V69 source'
assert not a.out.exists(),'Refusing to overwrite an existing capture'
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((Path(__file__).resolve().parent/'candidate-environment.json').read_text()))
subprocess.run([str(a.probe.resolve()),str(a.out.resolve()),a.cases],env=env,check=True)
