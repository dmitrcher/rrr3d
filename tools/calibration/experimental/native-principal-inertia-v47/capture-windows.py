from pathlib import Path
import subprocess,base64,zipfile,json,hashlib,sys
w=Path(__file__).resolve().parent;folder=r'C:\MotorRock\calibration-probes\mass-inertia-v47';parent=r'C:\MotorRock\calibration-probes\contact-pose-v45'
def cmd(t,timeout=60):
 assert len(t)<3500
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',t],capture_output=True,timeout=timeout)
 if p.returncode:Path('outputs/physics-v47-native-mass-error.log').write_bytes(p.stdout+p.stderr);p.check_returncode()
 return p.stdout+p.stderr
p=w/'source.zip'
with zipfile.ZipFile(p,'w',zipfile.ZIP_DEFLATED) as z:
 for name in ['trace-mass.cpp','build-trace.cmd']:z.write(w/name,name)
blob=base64.b64encode(p.read_bytes()).decode();cmd('if not exist '+folder+' mkdir '+folder);cmd('echo -----BEGIN CERTIFICATE----->'+folder+r'\source.b64')
for i in range(0,len(blob),700):cmd('echo '+blob[i:i+700]+'>>'+folder+r'\source.b64')
cmd('echo -----END CERTIFICATE----->>'+folder+r'\source.b64');cmd('certutil -f -decode '+folder+r'\source.b64 '+folder+r'\source.zip');cmd('tar -xf '+folder+r'\source.zip -C '+folder)
Path('outputs/physics-v47-native-mass-build.log').write_bytes(cmd(folder+r'\build-trace.cmd',120));cmd('xcopy /E /I /Y '+parent+r'\Microsoft.VC90.CRT '+folder+r'\Microsoft.VC90.CRT')
source_sha=hashlib.sha256((w/'trace-mass.cpp').read_bytes()).hexdigest();assert source_sha in cmd('certutil -hashfile '+folder+r'\trace-mass.cpp SHA256').decode().lower().replace(' ','')
for file,sha in [('PhysXCore.dll','c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb'),('SpringProbe.exe','3935cb983910290e2ff844bb437286d140d0fafa6cfbcbfbcbfd0f07b6208262')]:assert sha in cmd('certutil -hashfile '+parent+'\\'+file+' SHA256').decode().lower().replace(' ','')
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
names=json.loads((w/'cases.json').read_text());out=Path('outputs/native-principal-mass-v47');out.mkdir(exist_ok=False);runs=[]
for repeat in range(2):
 native=folder+f'\\untraced-r{repeat}.jsonl';cmd('set NX_SPRING_ITERATIONS=4&'+parent+r'\SpringProbe.exe '+native+' list:'+','.join(names),180);path=out/f'untraced-run{repeat}.jsonl';path.write_bytes(cmd('type '+native));base=compare.load(path,'windows-physx-d3d9');assert set(base)==set(names)
 trace=folder+f'\\mass-r{repeat}.jsonl';native=folder+f'\\traced-r{repeat}.jsonl';cmd('set MOTORROCK_NATIVE_TRACE_STEP=&set NX_SPRING_ITERATIONS=4&'+folder+r'\TraceMass.exe '+trace+' '+parent+r'\SpringProbe.exe '+native+' list:'+','.join(names),180)
 for path,remote in [(out/f'mass-run{repeat}.jsonl',trace),(out/f'physical-run{repeat}.jsonl',native)]:path.write_bytes(cmd('type '+remote))
 assert compare.load(out/f'physical-run{repeat}.jsonl','windows-physx-d3d9')==base;runs.append(base);print('Native',repeat,'24 traced cases equal untraced',flush=True)
assert runs[0]==runs[1]
(out/'capture-manifest.json').write_text(json.dumps({'cases':names,'source_sha256':source_sha,'source_verified':True,'core_and_original_probe_unchanged':True,'traced_physical_states_equal_untraced':True,'repeats':2},indent=2)+'\n')
