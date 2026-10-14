from pathlib import Path
import json,hashlib,shutil,gzip
r=Path.cwd();w=r/'work/static-box-query-v107';repo=r/'work/sync-rrr3d';d=repo/'tools/calibration/experimental/static-box-query-v107';d.mkdir(exist_ok=False);e=d/'evidence';e.mkdir();active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');m=json.loads((w/'source-manifest.json').read_text());m['requires']='Apply verified tire-lifetime-v106 over its prerequisite wheel-lifetime-v105 first. Two-file delta, default-off.'
for f in m['files']:
 for state in ['before','after']:
  sha=f[state+'_sha256']
  if sha is None:continue
  p=w/'before'/f['location']/f['path'] if state=='before' else active/f['path'];assert hashlib.sha256(p.read_bytes()).hexdigest()==sha
  dst=d/state/f['location']/f['path'];dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,dst)
(d/'base.json').write_text(json.dumps(m,indent=2)+'\n')
for name in ['apply-experiment.py','profile.cfg']:shutil.copy2(repo/'tools/calibration/experimental/tire-lifetime-v106'/name,d/name)
(d/'probe-environment.json').write_text(json.dumps(json.loads((r/'work/frozen-v107/manifest.json').read_text())['environment'],indent=2)+'\n')
for name in ['physics-v107-native-static-box-query-proof.json','physics-v107-native-candidate-bounds-proof.json']:shutil.copy2(r/'outputs'/name,e/name)
for name in ['live-curb13-proof.json','next-curb224-spring-order-lead.json']:shutil.copy2(r/'outputs/static-box-query-v107'/name,e/name)
for name in ['native-transform','native-boundary-transform','native-next-curb224','mac-curb-checkpoints']:shutil.copytree(r/'outputs/static-box-query-v107'/name,d/'captures'/name)
for name in ['focused','focused-final']:shutil.copy2(r/'outputs/static-box-query-v107'/name/'report.json',e/(name+'-report.json'))
s=d/'shadow';s.mkdir()
for name in ['NativeWheelBoxQueryV80.h','NativeWheelBoxRayV59.h','NativeWheelRayV52.h','NativeWheelPoseV54.h','NativeWheelPairLifetimeV106.h']:shutil.copy2(active/'src/Rock3dEngine/header/physics'/name,s/name)
for name in ['verify-box-query.cpp','candidate-bounds-proof.cpp','candidate-bounds-input.txt','box-query-fixtures.json']:shutil.copy2(w/name,d/name)
scripts=d/'reproduction-host-scripts';scripts.mkdir()
for p in w.glob('*.py'):shutil.copy2(p,scripts/p.name)
selected=['settle','slope-longitudinal','slope-lateral','bump-crossing'];count=0
with gzip.open(e/'windows-checkpoint.jsonl.gz','wt') as dst:
 for line in open(r/'outputs/calibration-v14-source-windows.jsonl'):
  row=json.loads(line)
  if row.get('event')=='calibration.sample' and row['detail'] in selected+['curb-side'] and (row['detail']!='curb-side' or row['sample']['tick']<224):dst.write(line);count+=1
assert count==2624
# Roll remains a preserved open fixture; never call it Native-equivalent.
with gzip.open(e/'retained-roll-contact-v106.jsonl.gz','wt') as dst,open(r/'outputs/tire-lifetime-v106/broad/candidate-run0.jsonl') as src:
 for line in src:
  row=json.loads(line)
  if row.get('event')=='calibration.sample' and row['detail']=='body-wheel-contact-roll':dst.write(line)
(d/'README.md').write_text('''# Static wheel-box query V107

Default-off two-file delta over the verified tire-lifetime-v106 module and its prerequisites. Apply with apply-experiment.py, then rebuild the adapter and all consumers. VehicleConstraint private layout is unchanged by this delta.

Original static box queries enter the same 1ae390 -> 1ea750 local-face/world-transform kernel already reconstructed for dynamic boxes in V80. Direct interpolation of the world ray changes contact lever arms. At curb13 Native and Mac previously differed by one float32 ULP in the first spring Jacobian; current prepared axes/Jacobians/inverseJacobians and resulting lambdas match in both repeats over five iterations. Incoming angular signed-zero differences remain explicitly recorded.

The local face kernel's epsilon is consulted only after a raw source suspension-line/box AABB overlap. Jolt's padded broadphase alone admits a spurious bump candidate48, whereas the original calls the static query only from49. The production candidate helper matches independently observed Native query presence under O0/O2 and retains the original unnormalized source endpoint. No coefficients, case/frame rules or measured-state injection are used. Measured-pose shadow replay is a separate diagnostic, never part of runtime physics.

Live curb first divergence moves13 ->224. At224 all corresponding spring properties and first incoming body match Native; the actual spring traversal remains Native0,1,3 versus Mac3,1,0. This is the next callback/broadphase lifetime lead, not a reason to fit parameters or hardcode row permutations.

Run verify.py --probe /absolute/path/to/rrr3d_physics_calibration. It checks the original static query17-word fixtures and production candidate helper under O0/O2. It then checks2400 full Native-equivalent states plus the224-state curb prefix and180 preserved V106 roll-contact states. The roll fixture confirms preservation only; full curb600 Native parity is not claimed. Broad repeated608-case validation and complete-run error/regression reports must be consulted. Complete game/race/render equivalence remains unproven, ordinary installed game is unchanged, and the overall goal remains incomplete.
''')
print('Draft two-file delta exported;2624 original checkpoint states; full validation pending.')
