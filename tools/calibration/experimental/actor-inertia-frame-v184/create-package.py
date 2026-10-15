from pathlib import Path
import json,hashlib,shutil,zipfile,subprocess
root=Path.cwd();p=root/'work/actor-inertia-frame-v184';out=root/'outputs/actor-inertia-frame-v184';bundle=out/'MotorRock-Mac-Steering-Inertia-v184';assert not bundle.exists();bundle.mkdir();sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
shutil.copy2(p/'SourceSteeringWorldProbe',bundle/'SourceSteeringWorldProbe');shutil.copy2(p/'verify-package.py',bundle/'verify.py');shutil.copy2(root/'work/sync-rrr3d/tools/calibration/profiles/windows-source-v14.cfg',bundle/'windows-source-v14.cfg');shutil.copy2(root/'work/sync-rrr3d/LICENSE',bundle/'PROJECT-LICENSE.txt')
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');licenses=bundle/'licenses';licenses.mkdir()
for name,source in [('Jolt',active/'build/macos-arm64-m10/_deps/rrr3d_jolt-src/LICENSE'),('bgfx',active/'build/bgfx-macos/bgfx/LICENSE'),('bx',active/'build/bgfx-macos/bx/LICENSE'),('bimg',active/'build/bgfx-macos/bimg/LICENSE'),('glm',active/'build/macos-arm64-m10/_deps/rrr3d_glm_source-src/copying.txt'),('Boost',active/'build/macos-arm64-m10/_deps/rrr3d_boost_source-src/LICENSE_1_0.txt')]:shutil.copy2(source,licenses/(name+'.txt'))
tiny=active/'build/macos-arm64-m10/_deps/rrr3d_tinyxml_source-src/tinyxml.h';(licenses/'TinyXML.txt').write_text(tiny.read_text().split('#ifndef')[0])
profile=json.loads((root/'outputs/scene-composition-v169/final-broad/manifest.json').read_text())['environment'];profile={k:v for k,v in profile.items() if k.startswith(('RRR3D_','NX_')) and not k.endswith(('_FILE','_PATH')) and k!='RRR3D_CALIBRATION_PROFILE'};profile.update(RRR3D_EXPERIMENT_GAMECAR_STEER_V177='1',RRR3D_EXPERIMENT_GAMECAR_WAKE_V171='1',RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173='1',RRR3D_EXPERIMENT_GAMECAR_STEER_DEADBAND_V179='1',RRR3D_EXPERIMENT_GAMECAR_STEER_MATRIX_V181='1',RRR3D_EXPERIMENT_GAMECAR_STEER_DIRECTION_V183='1',RRR3D_EXPERIMENT_NATIVE_ACTOR_INERTIA_FRAME_V184='1');(bundle/'profile.json').write_text(json.dumps(profile,indent=2)+'\n')
modes=['base','offset','tilt','tilt-yaw90','tilt-yaw126','tilt-yaw180'];references=bundle/'references';references.mkdir()
for mode in modes:
 folder=root/('outputs/controller-steering-transform-v180'+('-'+mode if mode!='base' else ''));shutil.copy2(folder/'accepted-native0.jsonl.gz',references/f'{mode}.jsonl.gz')
text='''MotorRock Mac steering/inertia research diagnostic, v184 (Apple Silicon, macOS13+).

Run: python3 verify.py --output verification

The compiled driver uses the current GameCar and Jolt libraries. It checks exact float32 body position/rotation and steering-angle outputs against immutable original Windows WheelsProgress captures, twice per fixture. One car, no AI, fixed dt1/60, zero motor/brake/gravity/damping and disabled tire force. Includes independent scene initialization, original body inertia-axis storage, initial COM moved after mass creation, tilted body and several headings. Only coordinate signed zero is canonicalized.

Native observations carry a source/build stamp. Proprietary Windows SDK headers, DLLs/PDBs and Windows executables are excluded. The installed ordinary game is separate. Full OnFixedStep, nonzero angular stabilization/clutch/clamps, jumping and complete ordinary race parity still require verification. Passing this tool does not complete the full game objective.
''';(bundle/'README.txt').write_text(text)
manifest=dict(scope='Original WheelsProgress plus SDK step versus actual Source GameCar/Jolt. Six45-case fixtures, two repeats. Research component acceptance; full game remains open.',modes=modes,files={str(f.relative_to(bundle)):sha(f) for f in sorted(bundle.rglob('*')) if f.is_file()},full_goal_complete=False);(bundle/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
archive=out/'MotorRock-Mac-Steering-Inertia-v184.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
 for f in sorted(bundle.rglob('*')):
  if f.is_file():z.write(f,bundle.name+'/'+str(f.relative_to(bundle)))
receipt=dict(archive=str(archive.relative_to(root)),bytes=archive.stat().st_size,sha256=sha(archive),binary_sha256=sha(bundle/'SourceSteeringWorldProbe'),members=len(list(bundle.rglob('*'))));(p/'archive-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt),flush=True)
