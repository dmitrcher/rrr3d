from pathlib import Path
import zipfile,json,hashlib
root=Path.cwd();p=root/'work/mesh-parts-v149';src=root/'work/sync-rrr3d/tools/calibration/experimental/local-mesh-contact-v149';items={}
for f in src.rglob('*'):
 if f.is_file() and f.name not in ['archive-receipt.json','release.json','github-release-verification.json','verify-release.py']:
  items['source/'+str(f.relative_to(src))]=f.read_bytes()
items['mac/rrr3d_physics_calibration']=(root/'work/frozen-v149-candidate/rrr3d_physics_calibration').read_bytes()
items['windows/WorldProbeV133.exe']=(root/'work/game-control-wake-v133/native/WorldProbeV133.exe').read_bytes()
items['windows/TraceSelectionV148.exe']=(root/'work/contact-selection-v148/observer/TraceSelectionV148.exe').read_bytes()
for track in [0,16,48,64]:items[f'fixture/track{track}.words']=(root/f'outputs/real-map-v124/repeats/track{track}-run0.words').read_bytes()
env=json.load(open('outputs/mesh-parts-v149/final-broad/manifest.json'))['environment'];items['profile.cfg']=Path(env['RRR3D_CALIBRATION_PROFILE']).read_bytes();env['RRR3D_CALIBRATION_PROFILE']='profile.cfg';items['environment.json']=(json.dumps(env,indent=2)+'\n').encode()
items['README.txt']=b'''MotorRock V149 local mesh contact investigation. The mac executable is the tested arm64 calibration probe; the ordinary installed game remains unchanged.
Resolve RRR3D_CALIBRATION_PROFILE in environment.json to the absolute profile.cfg path. Run mac/rrr3d_physics_calibration OUTPUT.jsonl list:sleep-settle (600 states), or all (608 isolated scenarios).
Both full runs match255864 original Windows float32 states,53 CTest checks pass, default-off900 moving states retain V143. Four-map first differences remain342/244/358/238. Geometry at map0 first342 is corrected; two later material-frame rows still require per-contact frames. This is not full game parity.
Windows WorldProbeV133.exe is unchanged. Use the existing original SDK/CRT dependencies in C:/MotorRock/calibration-probes/control-wake-v133 and original game DLL/PDB in C:/MotorRock/game-debug. From that dependency directory run the probe with absolute fixture and output paths. Proprietary SDK/game DLLs are not bundled.
TraceSelectionV148.exe is the exact validated read-only process observer for the original DLL version. Its source, offsets, capture scope and SDK hashes are in source/contact-selection-v148/.
The guarded four-file source overlay requires the verified V143 research base and existing SDK research patches. It is not an overlay for the ordinary older production root. No SDK code is changed by V149. Convex/flat group numeric ordering, global SAT, active edges and general cooking boundaries still need certification. Source physics profiles remain experimental and default off.
'''
items['manifest.sha256.json']=(json.dumps({n:hashlib.sha256(b).hexdigest() for n,b in items.items()},indent=2)+'\n').encode()
archive=root/'outputs/MotorRock-v149-Local-Mesh-Contacts-Investigation.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=8) as z:
 for n,b in items.items():
  info=zipfile.ZipInfo(n);info.compress_type=zipfile.ZIP_DEFLATED;info.external_attr=(0o100755 if n=='mac/rrr3d_physics_calibration' else 0o100644)<<16;z.writestr(info,b)
sha=hashlib.sha256(archive.read_bytes()).hexdigest();archive.with_suffix(archive.suffix+'.sha256').write_text(sha+'  '+archive.name+'\n');(p/'archive-receipt.json').write_text(json.dumps(dict(archive=str(archive),bytes=archive.stat().st_size,sha256=sha,members=len(items),goal_complete=False),indent=2)+'\n');print(archive.name,archive.stat().st_size,sha)
