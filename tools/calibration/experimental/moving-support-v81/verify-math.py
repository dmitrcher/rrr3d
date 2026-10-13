from pathlib import Path
import json,subprocess,tempfile
here=Path(__file__).resolve().parent;fixtures=json.loads((here/'slip-fixtures.json').read_text())['fixtures'];payload=str(len(fixtures))+'\n'+'\n'.join(' '.join(map(str,f['input_words']))for f in fixtures)+'\n'
with tempfile.TemporaryDirectory(prefix='motorrock-v81-proof-')as d:
 for opt in ['O0','O2']:
  exe=Path(d)/opt;subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-I',str(here/'include'),str(here/'verify-slip.cpp'),'-o',str(exe)],check=True)
  actual=[list(map(int,l.split()))for l in subprocess.check_output([str(exe)],input=payload,text=True).splitlines()];assert actual==[f['expected_words']for f in fixtures];print(opt,len(fixtures)*3,'source words exact')
