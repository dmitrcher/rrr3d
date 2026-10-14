from pathlib import Path
import subprocess,base64,zipfile,hashlib,json
folder=r'C:\MotorRock\calibration-probes\relative-contact-v147';p=Path(__file__).resolve().parent
def guest(cmd,timeout=90):
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=timeout)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout+r.stderr
with zipfile.ZipFile(p/'observer-source.zip','w',zipfile.ZIP_DEFLATED) as z:
 for name in ['trace-contact.cpp','build.cmd']:z.write(p/name,name)
payload=base64.b64encode((p/'observer-source.zip').read_bytes()).decode();guest('if not exist '+folder+' mkdir '+folder)
guest('echo -----BEGIN CERTIFICATE----->'+folder+r'\payload.b64')
for i in range(0,len(payload),700):guest('echo '+payload[i:i+700]+'>>'+folder+r'\payload.b64')
guest('echo -----END CERTIFICATE----->>'+folder+r'\payload.b64');guest('certutil -f -decode '+folder+r'\payload.b64 '+folder+r'\source.zip');guest('tar -xf '+folder+r'\source.zip -C '+folder)
(p/'observer-build.log').write_bytes(guest(folder+r'\build.cmd'))
guest(r'xcopy /E /I /Y C:\MotorRock\calibration-probes\contact-pose-v45\Microsoft.VC90.CRT '+folder+r'\Microsoft.VC90.CRT')
sha=hashlib.sha256((p/'trace-contact.cpp').read_bytes()).hexdigest();assert sha in guest('certutil -hashfile '+folder+r'\trace-contact.cpp SHA256').decode().lower().replace(' ','')
guest('certutil -f -encode '+folder+r'\TraceRelativeV147.exe '+folder+r'\observer.b64');raw=guest('type '+folder+r'\observer.b64').decode();data=base64.b64decode(''.join(x for x in raw.splitlines() if not x.startswith('-----')));(p/'TraceRelativeV147.exe').write_bytes(data)
exe_sha=hashlib.sha256(data).hexdigest();assert exe_sha in guest('certutil -hashfile '+folder+r'\TraceRelativeV147.exe SHA256').decode().lower().replace(' ','')
(p/'observer-build.json').write_text(json.dumps({'source_sha256':sha,'exe_sha256':exe_sha,'guest_source_and_exe_verified':True},indent=2)+'\n');print('Read-only clamp observer compiled and hashes verified',flush=True)
