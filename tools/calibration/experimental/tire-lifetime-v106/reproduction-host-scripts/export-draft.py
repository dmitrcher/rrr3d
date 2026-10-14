from pathlib import Path
import hashlib,json,shutil,gzip
root=Path.cwd();w=root/'work/tire-lifetime-v106';repo=root/'work/sync-rrr3d';d=repo/'tools/calibration/experimental/tire-lifetime-v106';d.mkdir(exist_ok=False);e=d/'evidence';e.mkdir();active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=active/'build/macos-arm64-m10/_deps/rrr3d_jolt-src';m=json.loads((w/'source-manifest.json').read_text());m['scope']='Guarded five-file V106 delta over accepted combinedV105. Independent tire object creation age and deferred static mesh/box pair manager destruction; full wheel-object deletion invalidates previous spring force. Both flags default off. Anchor state retained. No physical coefficient changes, frame/case dispatch or measured-state injection. Validation pending.';m['requires']='Apply verified wheel-lifetime-v105 over acceptedV102 first; before snapshots are exactly its resulting SDK/adapter.'
for f in m['files']:
 location=f['location'];base=sdk if location=='sdk' else active
 for state in ['before','after']:
  expected=f[state+'_sha256']
  if expected is None:continue
  p=w/'before'/f['path'] if state=='before' else base/f['path'];assert hashlib.sha256(p.read_bytes()).hexdigest()==expected,(f,state)
  target=d/state/('adapter' if location=='source' else location)/f['path'];target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target)
 f['location']='adapter' if location=='source' else location
(d/'base.json').write_text(json.dumps(m,indent=2)+'\n')
for name in ['apply-experiment.py','profile.cfg']:shutil.copy2(repo/'tools/calibration/experimental/wheel-lifetime-v105'/name,d/name)
(d/'probe-environment.json').write_text(json.dumps(json.loads((root/'work/frozen-v106/manifest.json').read_text())['environment'],indent=2)+'\n')
for name in ['native-pair-reset-proof.json','live-bump57-proof.json']:shutil.copy2(root/'outputs/tire-lifetime-v106'/name,e/name)
for kind in ['focused-tire-only','focused-with-pair','focused-final']:shutil.copy2(root/'outputs/tire-lifetime-v106'/kind/'report.json',e/(kind+'-report.json'))
for name in ['tire-clear-driver.disasm.txt','contact-manager-destruction.disasm.txt']:shutil.copy2(w/name,e/name)
for name in ['native-handle-writes','native-manager-reset','native-bump-pair-window80','mac-bump57-after-force-reset','live-pair']:shutil.copytree(root/'outputs/tire-lifetime-v106'/name,d/'captures'/name)
for name in ['watch-tracer','manager-tracer']:shutil.copytree(w/name,d/'observers'/name)
scripts=d/'reproduction-host-scripts';scripts.mkdir()
for p in w.glob('*.py'):shutil.copy2(p,scripts/p.name)
shadow=d/'shadow';shadow.mkdir();shutil.copy2(w/'pair-geometry-proof.cpp',shadow/'pair-geometry-proof.cpp')
for name in ['NativeWheelPoseV54.h','NativeWheelRayV52.h']:shutil.copy2(active/'src/Rock3dEngine/header/physics'/name,shadow/name)
(d/'pair-shadow-input.txt').write_text('0 0 0 12 0 0 0 1\n')
selected=['settle','slope-longitudinal','slope-lateral','bump-crossing'];samples=[];poses=[]
with gzip.open(e/'windows-checkpoint.jsonl.gz','wt') as dst:
 for line in open(root/'outputs/calibration-v14-source-windows.jsonl'):
  r=json.loads(line)
  if r.get('event')=='calibration.sample' and r['detail'] in selected:
   dst.write(line);samples.append(r)
   if r['detail']=='bump-crossing' and r['sample']['tick']<80:
    s=r['sample'];poses.append(' '.join(map(str,[s['tick']+1]+s['position']+s['rotation'])))
assert len(samples)==2400;(d/'pair-shadow-input.txt').write_text('0 0 0 12 0 0 0 1\n'+'\n'.join(poses)+'\n')
with gzip.open(e/'retained-roll-contact-v105.jsonl.gz','wt') as dst,open(root/'outputs/callback-queue-v105/broad/candidate-run0.jsonl') as src:
 for line in src:
  r=json.loads(line)
  if r.get('event')=='calibration.sample' and r['detail']=='body-wheel-contact-roll':dst.write(line)
(d/'README.md').write_text('''# Native wheel and tire object lifetime V106

Default-off investigation delta over the verified `wheel-lifetime-v105` module. Apply that prerequisite first. Apply this module with `apply-experiment.py --sdk-root JOLT --adapter-root SOURCE`, then rebuild every consumer: VehicleConstraint's private layout changes. Both V106 flags are enabled by `probe-environment.json` in the numerical comparison profile.

Windows stores tires independently of spring/stop object lifetimes. Tire solve traversal follows latest object registration; numeric allocator slots and current broadphase ranks do not preserve that order. Native contact-manager destruction clears all four objects of the affected wheel one step after broadphase pair removal, even while another collider supplies a valid current hit. The previous spring force must then be zero for the next tire callback. Wheel anchor data belongs to the wheel and survives this deletion.

The implementation derives lifetime events from current original suspension-line bounds and original static mesh/box bounds. It never consumes Windows states. Shadow replay of saved Native poses is an independent diagnostic only; the game/probe starts from the fixture and computes its own entire trajectory.

Native evidence binds actual manager flag mask2, call sites, deleted tire handles and pair removals. At bump57 all five solver iterations are checked against actual Windows:20 spring and40 tire inputs/results, including masses, geometric rows, incoming linear/angular velocities, signed bounds and result lambdas. Two original observer repeats preserve every Golden sample and the immutable original EXE/DLL hashes.

Run `python3 verify.py --probe /absolute/path/to/rrr3d_physics_calibration`. This compiles the production pair helper under O0/O2 and independently checks its deferred events against original observed Native events. It runs 2400 fresh Native-equivalent states plus180 retained V105 roll-contact states. The latter confirms preservation, not agreement with Windows.

Full608-case repeated comparison and magnitude/regression reports must be consulted before accepting this candidate. Dynamic contact-manager destruction, decoration mesh lifetime, complete game races and graphics parity remain outside this implementation's verified domain. Ordinary installed game remains its previously installed version. The overall parity goal remains incomplete.
''')
print('Exported guarded V106 draft',len(m['files']),'files; validation pending')
