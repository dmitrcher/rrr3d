from pathlib import Path
import subprocess,base64,zipfile,hashlib,json
folder=r'C:\MotorRock\calibration-probes\startup-import-v130';p=Path(__file__).resolve().parent
def guest(cmd,timeout=90):
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=timeout)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout+r.stderr
with zipfile.ZipFile(p/'observer-source.zip','w',zipfile.ZIP_DEFLATED) as z:
 for name in ['world-probe.cpp','NativeGameStartupV130.h','build-world.cmd','Fixture.h','WindowsReferenceGeometry.h','Catalog.h','ParityTrace.h','experimental/SpringProbeCases.h','experimental/IsolationProbeCases.h']:z.write(p/name,name)
payload=base64.b64encode((p/'observer-source.zip').read_bytes()).decode();guest('if not exist '+folder+' mkdir '+folder)
guest('echo -----BEGIN CERTIFICATE----->'+folder+r'\payload.b64')
for i in range(0,len(payload),700):guest('echo '+payload[i:i+700]+'>>'+folder+r'\payload.b64')
guest('echo -----END CERTIFICATE----->>'+folder+r'\payload.b64');guest('certutil -f -decode '+folder+r'\payload.b64 '+folder+r'\source.zip');guest('tar -xf '+folder+r'\source.zip -C '+folder)
(p/'observer-build.log').write_bytes(guest(folder+r'\build-world.cmd'))
guest(r'xcopy /E /I /Y C:\MotorRock\calibration-probes\contact-pose-v45\Microsoft.VC90.CRT '+folder+r'\Microsoft.VC90.CRT')
for name in ['PhysXCore.dll','PhysXCooking.dll','PhysXDevice.dll','PhysXLoader.dll','cudart32_30_9.dll']:
 reference=r'C:\MotorRock\calibration-probes\contact-pose-v45'+'\\'+name
 destination=folder+'\\'+name
 a=guest('certutil -hashfile '+reference+' SHA256').decode().lower().replace(' ','')
 b=guest('certutil -hashfile '+destination+' SHA256').decode().lower().replace(' ','')
 assert a.splitlines()[1]==b.splitlines()[1], 'Existing SDK dependency differs: '+name
sha=hashlib.sha256((p/'world-probe.cpp').read_bytes()).hexdigest();assert sha in guest('certutil -hashfile '+folder+r'\world-probe.cpp SHA256').decode().lower().replace(' ','')
guest('certutil -f -encode '+folder+r'\WorldProbeV130Cleanup.exe '+folder+r'\observer.b64');raw=guest('type '+folder+r'\observer.b64').decode();data=base64.b64decode(''.join(x for x in raw.splitlines() if not x.startswith('-----')));(p/'WorldProbeV130Cleanup.exe').write_bytes(data)
exe_sha=hashlib.sha256(data).hexdigest();assert exe_sha in guest('certutil -hashfile '+folder+r'\WorldProbeV130Cleanup.exe SHA256').decode().lower().replace(' ','')
(p/'observer-build.json').write_text(json.dumps({'source_sha256':sha,'exe_sha256':exe_sha,'guest_source_and_exe_verified':True},indent=2)+'\n');print('Real-map Native reader compiled and source/exe hashes verified',flush=True)
