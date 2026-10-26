from pathlib import Path
import subprocess,re,shlex,json,hashlib
root=Path.cwd();a=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');b=a/'build/macos-arm64-m10';text=(b/'build.ninja').read_text();obj=text[text.index('build src/Rock3dGame/CMakeFiles/rrr3d_original_game_car_smoke.dir/tests/OriginalGameCarSmoke.cpp.o:'):];obj=obj[:obj.index('\n\n')];link=text[text.index('build Debug/rrr3d_original_game_car_smoke:'):];link=link[:link.index('\n\n')]
def value(s,k):return re.search(r'^  '+k+r' = (.*)$',s,re.M).group(1)
args=['clang++',*shlex.split(value(obj,'DEFINES')),*shlex.split(value(obj,'FLAGS')),*shlex.split(value(obj,'INCLUDES')),str(root/'work/controller-jump-runtime-v189/source-probe.cpp'),*shlex.split(value(link,'LINK_FLAGS')),*shlex.split(value(link,'LINK_LIBRARIES')),'-o',str(root/'work/controller-jump-runtime-v189/SourceJumpRuntimeProbe')]
(root/'work/controller-jump-runtime-v189/SourceJumpRuntimeProbe').unlink(missing_ok=True)
(root/'work/controller-jump-runtime-v189/source-build.json').unlink(missing_ok=True)
subprocess.run(args,cwd=b,check=True)
source=root/'work/controller-jump-runtime-v189/source-probe.cpp';exe=root/'work/controller-jump-runtime-v189/SourceJumpRuntimeProbe'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
(root/'work/controller-jump-runtime-v189/source-build.json').write_text(json.dumps(dict(status='compiled',source_sha256=sha(source),executable_sha256=sha(exe),libraries={str(p.relative_to(a)):sha(p) for p in [b/'src/Rock3dGame/libRock3dGame.a',b/'src/Rock3dEngine/libRock3dEngine.a',b/'_deps/rrr3d_jolt-build/libJolt.a']}),indent=2)+'\n')
print('Source wake probe linked to actual candidate GameCar/Jolt libraries',flush=True)
