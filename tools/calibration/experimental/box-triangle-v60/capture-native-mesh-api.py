from pathlib import Path
import subprocess,base64,zipfile,json,hashlib
root=Path.cwd();here=root/'work/box-triangle-v60';out=root/'outputs/native-mesh-api-v60';out.mkdir(exist_ok=True);guest=r'C:\MotorRock\calibration-probes\mesh-api-v60';original=r'C:\MotorRock\calibration-probes\contact-pose-v45'
def cmd(t,timeout=120):
 assert len(t)<3500
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',t],capture_output=True,timeout=timeout)
 if p.returncode:print(p.stdout.decode(errors='replace'),p.stderr.decode(errors='replace'),flush=True);p.check_returncode()
 return p.stdout+p.stderr
build=r'''@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /fp:precise /DWIN32 /DNOMINMAX /I"C:\MotorRock\source\prog\Third Party\include\px\Physics\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Foundation\include" /I"C:\MotorRock\source\prog\Third Party\include\px\PhysXLoader\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Cooking\include" /Fo"%~dp0BoxRay.obj" /Fe"%~dp0BoxRay.exe" "%~dp0mesh-api-windows.cpp" /link /LIBPATH:"C:\MotorRock\source\prog\Third Party\bin\px" PhysXLoader.lib
exit /b %ERRORLEVEL%
'''
(here/'build-box.cmd').write_bytes(build.replace('\n','\r\n').encode())
with zipfile.ZipFile(here/'mesh-source.zip','w',zipfile.ZIP_DEFLATED) as z:
 for name in ['mesh-api-windows.cpp','build-box.cmd']:z.write(here/name,name)
b64=base64.b64encode((here/'mesh-source.zip').read_bytes()).decode();cmd('if not exist '+guest+' mkdir '+guest);cmd('echo -----BEGIN CERTIFICATE----->'+guest+r'\source.b64')
for i in range(0,len(b64),700):cmd('echo '+b64[i:i+700]+'>>'+guest+r'\source.b64')
cmd('echo -----END CERTIFICATE----->>'+guest+r'\source.b64');cmd('certutil -f -decode '+guest+r'\source.b64 '+guest+r'\source.zip');cmd('tar -xf '+guest+r'\source.zip -C '+guest)
(out/'build.log').write_bytes(cmd(guest+r'\build-box.cmd'))
cmd('xcopy /E /I /Y '+original+r'\Microsoft.VC90.CRT '+guest+r'\Microsoft.VC90.CRT')
for name in ['PhysXLoader.dll','PhysXCore.dll','PhysXCooking.dll','PhysXDevice.dll','cudart32_30_9.dll']:cmd('copy /Y '+original+'\\'+name+' '+guest+'\\'+name)
records=[]
for repeat in range(2):
 remote=guest+f'\\rays{repeat}.jsonl';cmd('cd /d '+guest+'&& '+guest+r'\BoxRay.exe >'+remote)
 data=cmd('type '+remote);path=out/f'native-run{repeat}.jsonl';path.write_bytes(data);rows=list(map(json.loads,data.decode().splitlines()));assert len(rows)==6;records.append({'file':path.name,'sha256':hashlib.sha256(data).hexdigest()});print('6 actual Native triangle queries',repeat,'complete',flush=True)
assert (out/'native-run0.jsonl').read_bytes()==(out/'native-run1.jsonl').read_bytes()
(out/'capture-manifest.json').write_text(json.dumps({'source_sha256':hashlib.sha256((here/'mesh-api-windows.cpp').read_bytes()).hexdigest(),'query_count':6,'two_native_repeats_exact':True,'captures':records,'scope':'Original SDK cooked mesh getTriangle API, exact vertices and convex-edge flags. Original Windows game/probe unchanged.','goal_complete':False},indent=2)+'\n')
