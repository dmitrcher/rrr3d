from pathlib import Path
import json,hashlib,shutil,gzip
r=Path.cwd();w=r/'work/static-pair-order-v108';repo=r/'work/sync-rrr3d';d=repo/'tools/calibration/experimental/static-pair-order-v108';d.mkdir(exist_ok=False);e=d/'evidence';e.mkdir();active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=active/'build/macos-arm64-m10/_deps/rrr3d_jolt-src';m=json.loads((w/'source-manifest.json').read_text());m['requires']='Verified static-box-query-v107 and its prerequisites. Six-file delta; private VehicleConstraint layout changes and all consumers must rebuild.'
for f in m['files']:
 for state in ['before','after']:
  expected=f[state+'_sha256']
  if expected is None:continue
  p=w/'before'/f['location']/f['path'] if state=='before' else (sdk if f['location']=='sdk' else active)/f['path'];assert hashlib.sha256(p.read_bytes()).hexdigest()==expected
  target=d/state/f['location']/f['path'];target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target)
(d/'base.json').write_text(json.dumps(m,indent=2)+'\n')
for name in ['apply-experiment.py','profile.cfg']:shutil.copy2(repo/'tools/calibration/experimental/static-box-query-v107'/name,d/name)
(d/'probe-environment.json').write_text(json.dumps(json.loads((r/'work/frozen-v108/manifest.json').read_text())['environment'],indent=2)+'\n')
for name in ['native-manager-list-proof.json','live-dispatch-proof.json','live-curb224-proof.json']:shutil.copy2(r/'outputs/static-pair-order-v108'/name,e/name)
shutil.copy2(r/'outputs/static-pair-order-v108/focused/report.json',e/'focused-report.json')
for name in ['native-curb-pair-window224','native-manager-slots','mac-curb224']:shutil.copytree(r/'outputs/static-pair-order-v108'/name,d/'captures'/name)
shutil.copytree(r/'outputs/static-box-query-v107/native-next-queue224',d/'captures/native-queue224')
for p in w.glob('*.disasm.txt'):shutil.copy2(p,e/p.name)
for name in ['manager-list-proof.cpp','manager-list-fixtures.json']:shutil.copy2(w/name,d/name)
scripts=d/'reproduction-host-scripts';scripts.mkdir()
for p in w.glob('*.py'):shutil.copy2(p,scripts/p.name)
selected=['settle','slope-longitudinal','slope-lateral','bump-crossing','curb-side'];count=0
with gzip.open(e/'windows-checkpoint.jsonl.gz','wt') as dst:
 for line in open(r/'outputs/calibration-v14-source-windows.jsonl'):
  row=json.loads(line)
  if row.get('event')=='calibration.sample' and row['detail'] in selected:dst.write(line);count+=1
assert count==3000
with gzip.open(e/'retained-roll-contact-v107.jsonl.gz','wt') as dst,open(r/'outputs/static-box-query-v107/broad/candidate-run0.jsonl') as src:
 for line in src:
  row=json.loads(line)
  if row.get('event')=='calibration.sample' and row['detail']=='body-wheel-contact-roll':dst.write(line)
(d/'README.md').write_text('''# Static wheel contact-manager dispatch V108

Default-off six-file delta over the verified static-box-query-v107 module and its prerequisites. Apply with apply-experiment.py, then rebuild all consumers: VehicleConstraint private layout changes.

The wheel contact-manager processor keeps dense slots. On deferred retirement of a manager, the last registered wheel manager fills its vacancy. Body contact managers use a separate processor. Current dispatch slots must not be confused with first wheel callback arrival ages, generic spring/tire object creation ages or broadphase pair birth serials. The new SDK array carries only current dispatch order; it never declares an object recreated.

Actual original Native manager slot snapshots at curb19,203,220,224 bind the mechanism: wheel slots evolve0,2,1,3 ->3,2,1 ->3,1 ->3,1,0. The generic production helper replays actual original pair events under O0/O2 and matches every observed slot in both repeats. The actual Mac runtime derives its own events from source suspension-line, vehicle box and static geometry; it receives no original events or poses. Its dispatch ranks independently match Native snapshots. At formerly divergent224 all five iterations match actual Native:15 spring and30 tire prepared properties, incoming bodies, limits and result lambdas. Both complete600-step curb histories now match the original observable contract.

Geometry integration currently supports one original terrain mesh, one vehicle and static solid box decorations with no child shapes. Actual source creation order is respected. Other scene types retain their previous ordering; this domain restriction is not a claim that their parity is finished. New flags stay off by default. No coefficient fitting, wheel/frame/case permutation or reference-state reset is used.

Run verify.py --probe /absolute/path/to/rrr3d_physics_calibration. It validates production lifecycle arithmetic under O0/O2,3000 fresh full Native-equivalent states and180 retained V107 roll-contact states. The latter proves preservation only. Consult repeated608-case broad and complete-run regression/error reports before accepting the checkpoint. Complete-game/render equality is unproven, ordinary installed game is unchanged, and the overall goal remains incomplete.

Disassembly excerpts are diagnostic context: the shape contact-list removal explicitly replaces an element with the last pointer; the separate wheel-processor slot mechanism is bound by actual Native slot observations, not inferred from that shape-list routine alone.
''')
print('Draft six-file V108 delta exported;3000 full Native checkpoint states; broad validation pending.')
