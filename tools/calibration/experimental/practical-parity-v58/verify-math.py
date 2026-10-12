from pathlib import Path
import argparse,tempfile,subprocess,json
p=argparse.ArgumentParser();p.add_argument('--out',type=Path,required=True);args=p.parse_args();here=Path(__file__).resolve().parent;results=[]
with tempfile.TemporaryDirectory(prefix='motorrock-v58-math-') as tmp:
 for opt in ['-O0','-O2']:
  exe=Path(tmp)/'verify';subprocess.run(['clang++','-std=c++17',opt,'-ffp-contract=off','-I',str(here),str(here/'verify-cached-frame.cpp'),'-o',str(exe)],check=True);r=subprocess.run([str(exe)],text=True,capture_output=True,check=True);assert r.stdout.strip()=='324 original wheel frame float32 words exact';results.append({'optimization':opt,'result':r.stdout.strip()});print(opt,r.stdout.strip(),flush=True)
args.out.parent.mkdir(parents=True,exist_ok=True);args.out.write_text(json.dumps({'results':results,'goal_complete':False},indent=2)+'\n')
