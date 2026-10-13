from pathlib import Path
import argparse,json,subprocess,tempfile
p=argparse.ArgumentParser();p.add_argument('--build',type=Path);a=p.parse_args();here=Path(__file__).resolve().parent
fixtures=json.loads((here/'box-query-fixtures.json').read_text())['fixtures']
payload=str(len(fixtures))+'\n'+'\n'.join(' '.join(map(str,f['input_words']))for f in fixtures)+'\n'
def check(build):
 build.mkdir(parents=True,exist_ok=True)
 for opt in ['O0','O2']:
  exe=build/('verify-'+opt)
  subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-I',str(here/'include'),str(here/'verify-box-query.cpp'),'-o',str(exe)],check=True)
  actual=[list(map(int,l.split()))for l in subprocess.check_output([str(exe)],input=payload,text=True).splitlines()]
  assert actual==[f['expected_words']for f in fixtures]
  print(opt,len(fixtures)*17,'original float32 words exact')
if a.build:check(a.build)
else:
 with tempfile.TemporaryDirectory(prefix='motorrock-v80-proof-')as d:check(Path(d))
