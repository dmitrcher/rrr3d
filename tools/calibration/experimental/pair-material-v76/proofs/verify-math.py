"""Verify independent original Windows fixtures without the game or PhysX SDK."""
from pathlib import Path
import json,subprocess,tempfile
root=Path(__file__).resolve().parent
fixtures=json.load(open(root/'box-inverse-fixtures.json'));geometry=fixtures['geometry'];inverse=fixtures['inverse_translation'];results=[]
with tempfile.TemporaryDirectory(prefix='motorrock-v76-math-') as d:
 for optimization in ['0','2']:
  for name in ['test-material-expanded','test-pair-material-errors','test-native-box-inverse']:
   exe=Path(d)/name
   subprocess.run(['xcrun','clang++','-std=c++17','-O'+optimization,'-ffp-contract=off','-I'+str(root/'include'),str(root/(name+'.cpp')),'-o',str(exe)],check=True)
   output=subprocess.run([str(exe)],check=True,capture_output=True,text=True).stdout
   if name=='test-material-expanded':assert output=='26/26 Native paired material solver quaternions exact (signed zero canonical)\n'
   elif name=='test-pair-material-errors':assert output=='88/88 converted normal errors exact\n'
   else:
    lines=output.splitlines();assert len(lines)==len(geometry)*2+len(inverse)
    for mode in [0,1]:
     exact=0
     for fixture,line in zip(geometry,lines[mode*len(geometry):(mode+1)*len(geometry)]):
      kind,m,axis,*data=line.split();assert kind=='G' and int(m)==mode
      actual=list(map(int,data));assert len(actual)==len(fixture['expected']);exact+=actual==fixture['expected']
      if mode==1:assert actual==fixture['expected'],fixture['file']
     assert exact==(26 if mode==0 else 28)
    for fixture,line in zip(inverse,lines[len(geometry)*2:]):
     kind,*data=line.split();assert kind=='I' and list(map(int,data))==fixture['expected'],fixture['file']
    output='28/28 ordered geometry outputs and 4/4 original inverse translations exact; control26/28\n'
   results.append({'optimization':optimization,'proof':name,'result':output.strip()});print(optimization,name,output.strip(),flush=True)
print(json.dumps({'results':results,'goal_complete':False},indent=2))
