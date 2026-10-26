"""Verify extracted owned compiler can build a fresh x86 probe without a VM."""
from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time
R=Path.cwd();W=R/'work/crossover-reference-v188';P=(W/'prefix').resolve();T=W/'toolchain-extracted';D=P/'drive_c/MotorRock/compiler';D.mkdir(exist_ok=True)
files={};sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for group,names in [('VC/bin',['cl.exe','c1.dll','c1xx.dll','c2.dll','link.exe']),('Common7/IDE',['mspdb80.dll','mspdbcore.dll','mspdbsrv.exe'])]:
 for name in names:
  src=T/group/name;dst=D/name;shutil.copyfile(src,dst);files[name]=sha(dst)
for src in sorted((T/'VC/bin/1033').glob('*.dll')):
 dst=D/'1033'/src.name;dst.parent.mkdir(exist_ok=True);shutil.copyfile(src,dst);files['1033/'+src.name]=sha(dst)
source=D/'compiler-smoke.cpp';source.write_text('#include <windows.h>\n#include <cstdio>\n#include <vector>\nint main(){std::vector<int> v(4,3);printf("MSC_VER=%d MSC_FULL_VER=%d PTR_BYTES=%u VECTOR_SUM=%d\\n",_MSC_VER,_MSC_FULL_VER,(unsigned)sizeof(void*),v[0]+v[1]+v[2]+v[3]);return v.size()==4 && sizeof(void*)==4?0:2;}\n')
z=lambda p:'Z:'+str(p.resolve()).replace('/','\\')
cmdfile=D/'build-smoke.cmd';cmdfile.write_text('@echo off\nsetlocal\nset "INCLUDE='+z(T/'VC/include')+';'+z(T/'WindowsSDK/Include')+'"\nset "LIB='+z(T/'VC/lib')+';'+z(T/'WindowsSDK/Lib')+'"\nset "PATH=C:\\MotorRock\\compiler;C:\\windows\\system32"\nif exist compiler-smoke.exe del /q compiler-smoke.exe\ncl /nologo /O2 /EHsc /MD compiler-smoke.cpp /Fecompiler-smoke.exe /link /manifest:no\nif errorlevel 1 exit /b 1\nif not exist compiler-smoke.exe exit /b 2\nexit /b 0\n')
cmd=['/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine','--bottle',str(P),'--no-gui','--dll','msvcr90,msvcp90,msvcm90=n','--debugmsg','-all,+loaddll','--no-update','--workdir',r'C:\MotorRock\compiler','--cx-app',r'C:\windows\system32\cmd.exe','/d','/c',r'C:\MotorRock\compiler\build-smoke.cmd']
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(WINEPREFIX=str(P),CX_BOTTLE=str(P));t=time.monotonic()
with (W/'compiler-smoke-build.log').open('w') as log:
 p=subprocess.Popen(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
 try:code=p.wait(timeout=120)
 except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGTERM);p.wait(timeout=10);raise
receipt=dict(returncode=code,command=cmd,elapsed_seconds=time.monotonic()-t,tool_files=files,source_sha256=sha(source),batch_sha256=sha(cmdfile),exe_sha256=sha(D/'compiler-smoke.exe') if (D/'compiler-smoke.exe').exists() else None,vm_started=False)
(W/'compiler-smoke-build.json').write_text(json.dumps(receipt,indent=2)+'\n');print(receipt);assert code==0
