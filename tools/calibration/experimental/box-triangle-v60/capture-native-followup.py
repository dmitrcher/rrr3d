from pathlib import Path
import subprocess,json,hashlib
r=Path.cwd();out=r/'outputs/triangle-native-followup-v60';out.mkdir(exist_ok=False)
here=r/'work/lost-tires-v59/border-contact';s=(here/'trace-contact.cpp').read_text()
folder=r'C:\MotorRock\calibration-probes\border-contact-v59';original=r'C:\MotorRock\calibration-probes\contact-pose-v45'
def guest(t,timeout=120):
 assert len(t)<3500
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',t],capture_output=True,timeout=timeout)
 if p.returncode:(out/'failure.log').write_bytes(p.stdout+p.stderr);p.check_returncode()
 return p.stdout
manifest={'scope':'Read-only original SDK contact tracer uses integrator counter rather than geometry-call count. No physics inputs or physical vehicle parameters changed.','source_sha256':hashlib.sha256(s.encode()).hexdigest(),'native_binaries_unchanged':True,'captures':[],'goal_complete':False}
# Constraint preparation occurs before the current step integrator, as verified in wheel traces.
for name,counter in [('border-air-straight',60),('border-grounded',62)]:
 for rep in range(2):
  print('Native counter',counter,'repeat',rep,'start',flush=True);prefix=folder+f'\\follow-{name}-{counter}-{rep}';private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl'
  log=guest(f'set MOTORROCK_NATIVE_TRACE_STEP={counter}&& set NX_SPRING_ITERATIONS=4&& '+folder+r'\TraceContact.exe '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name,240);(out/f'native-{name}-{counter}-{rep}.log').write_bytes(log)
  for k,p in [('private',private),('physical',physical)]:(out/f'native-{name}-{counter}-{rep}-{k}.jsonl').write_bytes(guest('type '+p))
  manifest['captures'].append({'name':name,'counter':counter,'repeat':rep});print('Native completed',counter,rep,flush=True)

(out/'manifest.json').write_text(json.dumps(manifest,indent=2))
