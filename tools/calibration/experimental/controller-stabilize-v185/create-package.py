from pathlib import Path
import shutil,json,hashlib,zipfile,subprocess
root=Path.cwd();work=root/'work/controller-stabilize-v185';out=root/'outputs/controller-stabilize-v185';bundle=out/'MotorRock-Mac-Stabilization-v185';assert not bundle.exists();bundle.mkdir();sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
proof=json.loads((out/'world-comparison.json').read_text());assert all(r['exact']==180 and not r['initial_mismatches'] for r in proof['runs'] if r['mode']=='on')
receipt=json.loads((work/'source-build.json').read_text());exe=work/'SourceStabilizeWorldProbe';assert sha(exe)==receipt['executable_sha256'] and sha(work/'source-world-probe.cpp')==receipt['source_sha256']
for name in ['full-broad-comparison.json','raw-regression-comparison.json']:
 proof=json.loads((out/name).read_text())
 if name.startswith('full'):assert len(proof['exact'])==608 and not proof['remaining'] and proof['two_repeats_exact']
 else:assert sum(r['exact_states'] for group in proof['tracks'].values() for r in group['runs'])==7200 and proof['identical_fixtures']
shutil.copy2(exe,bundle/exe.name);shutil.copy2(work/'verify-package.py',bundle/'verify.py');shutil.copy2(out/'accepted-native0.jsonl.gz',bundle/'native.jsonl.gz');shutil.copy2(root/'work/sync-rrr3d/tools/calibration/profiles/windows-source-v14.cfg',bundle/'windows-source-v14.cfg');profile=json.loads((out/'world-environment.json').read_text());profile={k:v for k,v in profile.items() if 'TRACE_' not in k};(bundle/'profile.json').write_text(json.dumps(profile,indent=2)+'\n')
old=root/'outputs/actor-inertia-frame-v184/MotorRock-Mac-Steering-Inertia-v184';shutil.copytree(old/'licenses',bundle/'licenses');shutil.copy2(old/'PROJECT-LICENSE.txt',bundle/'PROJECT-LICENSE.txt');(bundle/'README.md').write_text('Research diagnostic for Mac arm64 (macOS 13+). Run python3 verify.py --output verification from this folder. It checks 180 no-contact original GameCar stabilization and SDK integration cases twice, using the compiled Source world. It does not replace the installed game. Clutch, jump, physical contacts and the complete normal race remain separate open acceptance work.\n')
files={str(f.relative_to(bundle)):sha(f) for f in bundle.rglob('*') if f.is_file()};(bundle/'manifest.json').write_text(json.dumps(dict(scope='V185 actual GameCar PC24 stabilization: 180 isolated no-contact scenarios; actual Jolt integration and original Windows SDK integration, repeated. Full goal remains open.',files=files),indent=2)+'\n');archive=out/(bundle.name+'.zip')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
 for f in sorted(bundle.rglob('*')):
  if f.is_file():z.write(f,Path(bundle.name)/f.relative_to(bundle))
(archive.with_suffix('.zip.sha256')).write_text(sha(archive)+'  '+archive.name+'\n');(work/'archive-receipt.json').write_text(json.dumps(dict(archive=str(archive.relative_to(root)),bytes=archive.stat().st_size,sha256=sha(archive),binary_sha256=sha(exe)),indent=2)+'\n');print(archive.name,archive.stat().st_size,sha(archive))
