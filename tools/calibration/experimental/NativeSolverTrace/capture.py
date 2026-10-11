from pathlib import Path
import subprocess,json,argparse,hashlib
p=argparse.ArgumentParser(description='Capture the fixed-version Native solver in a real Parallels VM.')
p.add_argument('--measurements',type=Path,required=True)
p.add_argument('--tracer-sha256',required=True)
p.add_argument('--vm',default='Windows 11')
p.add_argument('--guest-base',default=r'C:\MotorRock\calibration-probes\contact-parent-v30')
a=p.parse_args();a.measurements.mkdir(parents=True,exist_ok=True)
folder=a.guest_base;debug=folder+r'\solver-kernel-v31'
def guest(cmd,timeout=90):
 r=subprocess.run(['prlctl','exec',a.vm,'cmd.exe','/c',cmd],capture_output=True,timeout=timeout)
 if r.returncode:raise RuntimeError((r.stdout+r.stderr).decode(errors='replace'))
 return r.stdout+r.stderr
sha=guest('certutil -hashfile '+folder+r'\PhysXCore.dll SHA256').decode(errors='replace')
assert 'c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb' in sha.lower().replace(' ','')
for filename,expected in [('SpringProbe.exe','8c331b7ea35ed6181c0f8dd52cddca451f60c817d381d360cb53852e8c49ed9d'),(r'solver-kernel-v31\TraceContact.exe',a.tracer_sha256)]:
 actual=guest('certutil -hashfile '+folder+'\\'+filename+' SHA256').decode(errors='replace').lower().replace(' ','')
 assert expected.lower() in actual,('Unexpected binary',filename)
for m in range(3):
 for h in range(2):
  for v in range(2):
   name=f'isolate-03-contact-m{m}-h{h}-v{v}-f1'
   for repeat in range(2):
    trace=folder+f'\\kernel-{name}-r{repeat}-geometry.jsonl';native=folder+f'\\kernel-{name}-r{repeat}-physics.jsonl'
    flags='set NX_SPRING_ITERATIONS=4&set NX_INITIAL_ROTATION_TRACE=1&set NX_CONTACT_QUAT_TRACE=1&set NX_INVERSE_MASS_TRACE=1&set NX_LOCAL_POINT_TRACE=1&set NX_SOLVER_ITERATION_TRACE=1&set NX_CONTACT_CALLER_TRACE=1&set NX_CONTACT_CALLER_ALL=1&'
    paths=[a.measurements/f'reference-native-kernel-{name}-{kind}-run{repeat}-v31-windows.jsonl' for kind in ['geometry','physics']]
    assert not any(p.exists() for p in paths)
    guest(flags+debug+r'\TraceContact.exe '+trace+' '+folder+r'\SpringProbe.exe '+native+' list:'+name)
    for p,src in zip(paths,[trace,native]):p.write_bytes(guest('type '+src))
    print(name,repeat,flush=True)
