from pathlib import Path
import subprocess,json,argparse,hashlib,sys
p=argparse.ArgumentParser();p.add_argument('--case');p.add_argument('--reference',type=Path,required=True);p.add_argument('--measurements',type=Path,required=True);p.add_argument('--tracer-sha256',required=True);p.add_argument('--vm',default='Windows 11');p.add_argument('--guest-base',default=r'C:\MotorRock\calibration-probes\contact-parent-v30');a=p.parse_args();out=a.measurements;out.mkdir(parents=True,exist_ok=True);folder=a.guest_base;debug=folder+r'\vertex-v38'
def guest(cmd,timeout=90):
 r=subprocess.run(['prlctl','exec',a.vm,'cmd.exe','/c',cmd],capture_output=True,timeout=timeout)
 if r.returncode:(out/'physics-v38-vertex-native-capture-error.log').write_bytes(r.stdout+r.stderr);r.check_returncode()
 return r.stdout+r.stderr
for name,sha in [('PhysXCore.dll','c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb'),('SpringProbe.exe','8c331b7ea35ed6181c0f8dd52cddca451f60c817d381d360cb53852e8c49ed9d'),(r'vertex-v38\TraceContact.exe',a.tracer_sha256)]:assert sha in guest('certutil -hashfile '+folder+'\\'+name+' SHA256').decode().lower().replace(' ','')
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
baseline=compare.load(a.reference,'windows-physx-d3d9')
names=[f'frame-v25-long-p{p}-x{x}-y{y}' for p in range(12) for x in range(3) for y in range(2)]
if a.case:assert a.case in names;names=[a.case]
else:names=['frame-v25-long-p7-x1-y0','frame-v25-long-p5-x0-y0','frame-v25-long-p5-x1-y1','frame-v25-long-p4-x1-y0']
for name in names:
 paths=[]
 for repeat in range(2):
  trace=folder+f'\\edge-{name}-r{repeat}-geometry.jsonl';native=folder+f'\\edge-{name}-r{repeat}-physics.jsonl'
  flags='set NX_SPRING_ITERATIONS=4&set NX_INITIAL_ROTATION_TRACE=1&set NX_CONTACT_QUAT_TRACE=1&set NX_INVERSE_MASS_TRACE=1&set NX_LOCAL_POINT_TRACE=1&set NX_SOLVER_ITERATION_TRACE=1&set NX_CONTACT_CALLER_TRACE=1&set NX_CONTACT_CALLER_ALL=1&'
  paths_r=[out/f'reference-native-vertex-{name}-{kind}-run{repeat}-v38-windows.jsonl' for kind in ['geometry','physics']];assert not any(p.exists() for p in paths_r)
  guest(flags+debug+r'\TraceContact.exe '+trace+' '+folder+r'\SpringProbe.exe '+native+' list:'+name)
  for p,src in zip(paths_r,[trace,native]):p.write_bytes(guest('type '+src))
  got=compare.load(paths_r[1],'windows-physx-d3d9');assert got[name]==baseline[name],('Native physical states changed',name,repeat);paths.append(paths_r[1]);print(name,repeat,'120 states baseline exact',flush=True)
 assert compare.load(paths[0],'windows-physx-d3d9')==compare.load(paths[1],'windows-physx-d3d9')
