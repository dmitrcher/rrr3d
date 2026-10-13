from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parent;proof=root/'proofs'
with tempfile.TemporaryDirectory(prefix='motorrock-v75-proof-') as directory:
 for o in ['0','2']:
  for source in ['test-native-pair-local-cache.cpp','test-native-pair-local-cache-holdout.cpp','test-native-material-expanded.cpp','test-pair-material-errors.cpp']:
   exe=Path(directory)/source.removesuffix('.cpp')
   subprocess.run(['xcrun','clang++','-std=c++17','-O'+o,'-ffp-contract=off','-I',str(proof/'include'),str(proof/source),'-o',str(exe)],check=True)
   r=subprocess.run([str(exe)],check=True,capture_output=True,text=True)
   print('O'+o,source,r.stdout.strip())
print('Independent V75 numerical proofs passed. Overall physics parity remains incomplete.')
