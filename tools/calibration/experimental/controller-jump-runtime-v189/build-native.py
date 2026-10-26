"""Fresh owned original-method observer using extracted MSVC, no Parallels."""
from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time
R=Path.cwd();W=R/'work/controller-jump-runtime-v189';P=(R/'work/crossover-reference-v188/prefix').resolve();T=R/'work/crossover-reference-v188/toolchain-extracted';D=P/'drive_c/MotorRock/jump-runtime-v189';D.mkdir(exist_ok=True)
N=W/'native';header=R/'work/controller-stabilize-v185/native/NativeGameStartupV130.h';shutil.copyfile(header,N/header.name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();source=N/'jump-probe.cpp';source_sha=sha(source);build_id=hashlib.sha256((source_sha+sha(header)+'MSVC15.00.30729.01/O2/EHsc/MD').encode()).hexdigest();(N/'build-id.h').write_text(f'static const char*kProbeSourceSha="{source_sha}";\nstatic const char*kProbeBuildId="{build_id}";\n')
for name in ['jump-probe.cpp','NativeGameStartupV130.h','build-id.h']:shutil.copyfile(N/name,D/name)
exe=D/'JumpProbeV189.exe';exe.unlink(missing_ok=True);(W/'native-build.json').unlink(missing_ok=True)
z=lambda p:'Z:'+str(p.resolve()).replace('/','\\');original=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-09-16/files-pasted-by-the-user-motorrock/work/windows/prog/Third Party');includes=[original/'include/px'/n/'include' for n in ['Physics','Foundation','PhysXLoader','Cooking']]
cmdfile=D/'build.cmd';cmdfile.write_text('@echo off\nsetlocal\nset "INCLUDE='+z(T/'VC/include')+';'+z(T/'WindowsSDK/Include')+'"\nset "LIB='+z(T/'VC/lib')+';'+z(T/'WindowsSDK/Lib')+'"\nset "PATH=C:\\MotorRock\\compiler;C:\\windows\\system32"\ncl /nologo /O2 /EHsc /MD /DWIN32 /DNOMINMAX '+' '.join('/I"'+z(p)+'"' for p in includes)+' jump-probe.cpp /link /OUT:JumpProbeV189.exe /manifest:no /LIBPATH:"'+z(original/'bin/px')+'" PhysXLoader.lib dbghelp.lib\nif errorlevel 1 exit /b 1\nif not exist JumpProbeV189.exe exit /b 2\nexit /b 0\n')
cmd=['/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine','--bottle',str(P),'--no-gui','--dll','msvcr90,msvcp90,msvcm90=n','--debugmsg','-all,+loaddll','--no-update','--workdir',r'C:\MotorRock\jump-runtime-v189','--cx-app',r'C:\windows\system32\cmd.exe','/d','/c',r'C:\MotorRock\jump-runtime-v189\build.cmd'];env=dict(os.environ);env.update(WINEPREFIX=str(P),CX_BOTTLE=str(P));t=time.monotonic()
with (W/'native-build.log').open('w') as log:
 p=subprocess.Popen(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
 try:code=p.wait(timeout=120)
 except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGTERM);p.wait(timeout=10);raise
assert code==0 and exe.exists(),(code,'No valid fresh observer; inspect build log')
shutil.copyfile(R/'work/crossover-reference-v188/owned-observer-native-crt.manifest',D/'JumpProbeV189.exe.manifest')
# SDK DLLs are unchanged. Game DLL/PDB are loaded from the separate pinned game-debug directory.
modules={}
for name in ['PhysXCore.dll','PhysXLoader.dll','PhysXCooking.dll','PhysXDevice.dll','cudart32_30_9.dll']:
 src=P/'drive_c/MotorRock/game-debug'/name;shutil.copyfile(src,D/name);modules[name]=sha(D/name)
receipt=dict(source_sha256=source_sha,header_sha256=sha(header),build_id=build_id,observer_sha256=sha(exe),command=cmd,batch_sha256=sha(cmdfile),compiler_sha256=sha(P/'drive_c/MotorRock/compiler/cl.exe'),import_library_sha256=sha(original/'bin/px/PhysXLoader.lib'),sdk_header_sha256=sha(includes[0]/'NxPhysics.h'),module_sha256=modules,returncode=code,elapsed_seconds=time.monotonic()-t,vm_started=False)
(W/'native-build.json').write_text(json.dumps(receipt,indent=2)+'\n');print('Fresh observer compiled',receipt['observer_sha256'])
