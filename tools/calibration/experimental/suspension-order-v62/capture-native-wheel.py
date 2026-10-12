from pathlib import Path
import subprocess,json,hashlib
root=Path.cwd();out=root/'outputs/native-border-wheel-v62';out.mkdir(exist_ok=False);guest=r'C:\MotorRock\calibration-probes\wheel-row-v56';original=r'C:\MotorRock\calibration-probes\contact-pose-v45'
cases=[('border-grounded',61)]
def cmd(t):
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',t],capture_output=True,timeout=240)
 if p.returncode:print(p.stdout.decode(errors='replace'),p.stderr.decode(errors='replace'));p.check_returncode()
 return p.stdout
captures=[]
for i,(name,step) in enumerate(cases):
 for rep in range(2):
  print(name,step,rep,'start',flush=True);prefix=guest+f'\\border-v62-{i}-{rep}';private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl'
  cmd(f'set MOTORROCK_NATIVE_TRACE_STEP={step}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+guest+r'\TraceWheel.exe '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name)
  for kind,p in [('private',private),('physical',physical)]:(out/f'{i}-{kind}-run{rep}.jsonl').write_bytes(cmd('type '+p))
  captures.append({'case':name,'integration_counter':step,'index':i,'repeat':rep});print(name,step,rep,'complete',flush=True)
(out/'capture-manifest.json').write_text(json.dumps({'cases':cases,'captures':captures,'source_sha256':hashlib.sha256((root/'work/wheel-row-v56/tracer/trace-wheel.cpp').read_bytes()).hexdigest(),'goal_complete':False},indent=2)+'\n')
