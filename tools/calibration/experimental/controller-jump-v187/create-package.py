from pathlib import Path
import gzip,hashlib,json,shutil,zipfile

root=Path.cwd();w=root/'work/controller-jump-v187';out=root/'outputs/controller-jump-v187';bundle=out/'MotorRock-Mac-Jump-v187';bundle.mkdir(exist_ok=False)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
accept=json.loads((out/'acceptance.json').read_text());assert accept['checkpoint_accepted'] and not accept['full_goal_complete']
for binary,receipt,source in [('SourceJumpGravityProbe','source-build.json','source-gravity-probe.cpp'),('SourceAccelerationLifetimeProbe','acceleration-lifetime-build.json','source-acceleration-lifetime.cpp'),('SourceJumpGatesProbe','source-gates-build.json','source-gates.cpp')]:
    r=json.loads((w/receipt).read_text());assert sha(w/binary)==r['executable_sha256'] and sha(w/source)==r['source_sha256'];shutil.copy2(w/binary,bundle/binary)
shutil.copy2(w/'verify-package.py',bundle/'verify.py')
shutil.copy2(root/'work/sync-rrr3d/tools/calibration/profiles/windows-source-v14.cfg',bundle/'windows-source-v14.cfg')
for filename in ['compare.py','experimental/native-angular-v49/compare-float32.py']:shutil.copy2(root/'work/sync-rrr3d/tools/calibration'/filename,bundle/Path(filename).name)
selected=[];golden=root/'outputs/calibration-v14-source-windows.jsonl'
assert sha(golden)=='4bcc66c8f4f28ccb77a07759175573cc56d72035636642617fae4e75addecc1f'
with golden.open() as stream:
    for row in map(json.loads,stream):
        if row['detail'] in ('gravity','game-air-gravity','game-air-pitch'):selected.append(row)
(bundle/'native-sdk.jsonl.gz').write_bytes(gzip.compress((''.join(json.dumps(r,separators=(',',':'))+'\n' for r in selected)).encode(),mtime=0))
profile=json.loads((out/'final-broad/manifest.json').read_text())['environment'];profile={k:v for k,v in profile.items() if 'TRACE_' not in k};profile.pop('RRR3D_CALIBRATION_PROFILE',None);(bundle/'profile.json').write_text(json.dumps(profile,indent=2)+'\n')
old=root/'outputs/controller-stabilize-v185/MotorRock-Mac-Stabilization-v185';shutil.copytree(old/'licenses',bundle/'licenses');shutil.copy2(old/'PROJECT-LICENSE.txt',bundle/'PROJECT-LICENSE.txt')
shutil.copy2(out/'acceptance.json',bundle/'acceptance.json');shutil.copy2(w/'README.md',bundle/'EVIDENCE-SCOPE.md')
scope='V187 experimental Mac arm64 diagnostic: three recorded SDK total-acceleration trajectories, Source-derived command gates and Jolt acceleration lifetime. No native GameCar JumpProgress runtime observation, no full-game parity claim.'
(bundle/'README.md').write_text('Run python3 verify.py --output verification from this folder on Mac arm64. The verifier checks SHA256 before execution. It verifies 3 x 120 recorded Windows SDK states twice, 360 original-source command gates twice and 7 Jolt body masses/lifecycles twice. See EVIDENCE-SCOPE.md for the scene-gravity/extra-acceleration equivalence and missing native runtime evidence. This diagnostic does not replace the installed game or start Windows.\n')
files={str(f.relative_to(bundle)):sha(f) for f in bundle.rglob('*') if f.is_file()};(bundle/'manifest.json').write_text(json.dumps(dict(scope=scope,source_golden_sha256=sha(golden),files=files),indent=2)+'\n')
archive=out/(bundle.name+'.zip')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
    for f in sorted(bundle.rglob('*')):
        if f.is_file():z.write(f,Path(bundle.name)/f.relative_to(bundle))
receipt=dict(archive=str(archive.relative_to(root)),bytes=archive.stat().st_size,sha256=sha(archive),binaries={n:sha(w/n) for n in ('SourceJumpGravityProbe','SourceAccelerationLifetimeProbe','SourceJumpGatesProbe')})
(w/'archive-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt,indent=2))
