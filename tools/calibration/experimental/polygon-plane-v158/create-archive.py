from pathlib import Path
import zipfile,json,hashlib
root=Path.cwd();p=root/'work/polygon-plane-v158';src=root/'work/sync-rrr3d/tools/calibration/experimental/polygon-plane-v158'
audit=json.load(open(root/'outputs/physics-v158-candidate-broad-audit.json'))
assert len(audit['exact'])==608 and not audit['remaining'] and audit['two_repeats_exact']
manifest=json.load(open(root/'outputs/polygon-plane-v158/final-broad/manifest.json'));assert len(manifest['captures'])==2
items={}
for f in src.rglob('*'):
 if f.is_file() and f.name not in ['archive-receipt.json','github-release-verification.json']:
  items['source/'+str(f.relative_to(src))]=f.read_bytes()
items['mac/rrr3d_physics_calibration']=(root/'work/frozen-v158-candidate/rrr3d_physics_calibration').read_bytes()
assert hashlib.sha256(items['mac/rrr3d_physics_calibration']).hexdigest()==manifest['probe_sha256']
items['windows/WorldProbeV133.exe']=(root/'work/game-control-wake-v133/native/WorldProbeV133.exe').read_bytes()
assert hashlib.sha256(items['windows/WorldProbeV133.exe']).hexdigest()=='3148e303225de65e706398c02b7855711b6a45ceaab141fa38446fd38910b89f'
for track in [0,16,48,64]:items[f'fixture/track{track}.words']=(root/f'outputs/real-map-v124/repeats/track{track}-run0.words').read_bytes()
env=manifest['environment'];items['profile.cfg']=Path(env['RRR3D_CALIBRATION_PROFILE']).read_bytes();env['RRR3D_CALIBRATION_PROFILE']='profile.cfg';items['environment.json']=(json.dumps(env,indent=2)+'\n').encode()
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d')
items['Jolt-LICENSE.txt']=(active/'build/macos-arm64-m10/_deps/rrr3d_jolt-src/LICENSE').read_bytes()
items['README.txt']=(p/'README.md').read_bytes()
items['manifest.sha256.json']=(json.dumps({n:hashlib.sha256(b).hexdigest() for n,b in items.items()},indent=2)+'\n').encode()
archive=root/'outputs/MotorRock-v158-Projection-Plane-Investigation.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=8) as z:
 for n,b in items.items():
  info=zipfile.ZipInfo(n);info.compress_type=zipfile.ZIP_DEFLATED;info.external_attr=(0o100755 if n=='mac/rrr3d_physics_calibration' else 0o100644)<<16;z.writestr(info,b)
sha=hashlib.sha256(archive.read_bytes()).hexdigest();archive.with_suffix(archive.suffix+'.sha256').write_text(sha+'  '+archive.name+'\n')
(p/'archive-receipt.json').write_text(json.dumps(dict(archive=str(archive),bytes=archive.stat().st_size,sha256=sha,members=len(items),goal_complete=False),indent=2)+'\n');print(archive.name,archive.stat().st_size,sha)
