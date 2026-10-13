"""Run frozen CLI using the exact tested V80 environment and unchanged params."""
from pathlib import Path
import argparse,json,os,subprocess
p=argparse.ArgumentParser();p.add_argument('--probe',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--cases',default='list:pair-stock-pole-0,pair-stock-pole-1');a=p.parse_args()
assert b'RRR3D_EXPERIMENT_NATIVE_WHEEL_BOX_QUERY_V80' in a.probe.read_bytes(),'Probe lacks the recorded V80 source'
assert not a.out.exists(),'Refusing to overwrite an existing capture'
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((Path(__file__).resolve().parent/'candidate-environment.json').read_text()))
env['RRR3D_CALIBRATION_PROFILE']=str(Path(__file__).resolve().parent/'windows-source-v14.cfg')
subprocess.run([str(a.probe.resolve()),str(a.out.resolve()),a.cases],env=env,check=True)
