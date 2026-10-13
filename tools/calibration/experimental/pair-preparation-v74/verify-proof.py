from pathlib import Path
import subprocess,tempfile,json
root=Path(__file__).resolve().parent;proof=root/'proofs';fixtures=json.loads((proof/'vertical-geometry-fixtures.json').read_text());results=[]
with tempfile.TemporaryDirectory(prefix='motorrock-v74-proof-') as directory:
 for o in ['0','2']:
  for source in ['test-native-pair-levers.cpp','test-cached-pair-levers.cpp','test-native-pair-masses.cpp','test-vertical-geometry.cpp','test-vertical-geometry-unfused-edges.cpp']:
   exe=Path(directory)/source.removesuffix('.cpp');flags=[] if 'vertical-geometry' in source else ['-ffp-contract=off'];subprocess.run(['xcrun','clang++','-std=c++17','-O'+o,*flags,'-I',str(proof/'include'),str(proof/source),'-o',str(exe)],check=True);r=subprocess.run([str(exe)],check=True,capture_output=True,text=True)
   if 'vertical-geometry' in source:
    differences=[]
    for f,l in zip(fixtures,r.stdout.splitlines()):
     actual=list(map(int,l.split()))[1:];assert len(actual)==len(f['expected']);differences.append(sum(a!=b for a,b in zip(actual,f['expected'])))
    assert differences==([0,0] if 'unfused-edges' in source else [2,2]);output='geometry coordinate differences per original capture: '+str(differences)
   else:output=r.stdout.strip()
   print('O'+o,source,output);results.append({'optimization':o,'source':source,'result':output})
print('V74 independent proofs passed. Full parity remains incomplete.')
