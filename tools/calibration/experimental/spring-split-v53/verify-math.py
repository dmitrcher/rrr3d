from pathlib import Path
import subprocess,tempfile,json
root=Path(__file__).resolve().parent
with tempfile.TemporaryDirectory(prefix='motorrock-spring-v53-') as tmp:
 for name in ['solve','preparation']:
  for level in ['O0','O2']:
   binary=Path(tmp)/(name+'-'+level)
   subprocess.run(['clang++','-std=c++17','-'+level,'-ffp-contract=off',str(root/('verify-'+name+'.cpp')),'-o',str(binary)],check=True)
   subprocess.run([str(binary)],check=True)
print('Original numerical inputs: 120 solves + 24 final updates, 60 J*v + 4 prepared spring coefficient sets. Both ARM64 optimization levels passed.')
