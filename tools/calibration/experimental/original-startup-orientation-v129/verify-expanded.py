from pathlib import Path
import subprocess,json
p=Path(__file__).resolve().parent;inc=str(p.resolve())
rows=json.loads((p/'expanded-inputs.json').read_text());data=''.join(' '.join(map(str,r['direction_words']))+'\n' for r in rows)
native=[json.loads(l) for l in (p/'native-expanded-run0.jsonl').read_text().splitlines()][1:69]
report=[]
for opt in ['O0','O2']:
 subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+inc,str(p/'portable-probe.cpp'),'-o',str(p/('portable-'+opt))],check=True)
 r=subprocess.run([str(p/('portable-'+opt))],input=data,text=True,capture_output=True,check=True);(p/('portable-'+opt+'.words')).write_text(r.stdout)
 for actual,n in zip(r.stdout.splitlines(),native):
  a=list(map(int,actual.split()));b=n['direction_words']+n['up_words']+n['matrix_words']+n['quaternion_xyzw_words'];diff=[(i,x,y) for i,(x,y) in enumerate(zip(a,b)) if x!=y];report.append({'optimization':opt,'track':n['track'],'word_mismatches':diff});print(opt,n['track'],diff)
(p/'portable-expanded-proof.json').write_text(json.dumps(report,indent=2)+'\n')
