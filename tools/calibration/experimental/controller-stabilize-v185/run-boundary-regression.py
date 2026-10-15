from pathlib import Path
import os,json,subprocess,gzip,hashlib,importlib.util,sys
root=Path.cwd();out=root/'outputs/controller-stabilize-v185/boundary-regression';out.mkdir(exist_ok=True);names=['game-angular-damping','game-clamp-roll','game-clamp-pitch','game-clamp-mixed'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'outputs/controller-stabilize-v185/broad-missing-command-boundary-rejected/manifest.json').read_text())['environment']);probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');native={n:[] for n in names}
with (root/'outputs/calibration-v14-source-windows.jsonl').open() as f:
 for row in map(json.loads,f):
  if row['event']=='calibration.sample' and row['detail'] in native:native[row['detail']].append(row['sample'])
spec=importlib.util.spec_from_file_location('words',root/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words);previous=None;reports=[]
for rep in range(2):
 path=out/f'run{rep}.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(names)],env=env,check=True,timeout=120);rows=list(map(json.loads,path.read_text().splitlines()));states={n:[r['sample'] for r in rows if r['event']=='calibration.sample' and r['detail']==n] for n in names};assert all(len(x)==120 for x in states.values())
 for name in names:assert [words.physical(x) for x in states[name]]==[words.physical(x) for x in native[name]],name
 semantic=[{k:v for k,v in row.items() if k not in ('utc','monotonic_ms','pid')} for row in rows]
 if previous is not None:assert semantic==previous
 previous=semantic;path.with_suffix('.jsonl.gz').write_bytes(gzip.compress(path.read_bytes(),mtime=0));path.unlink();reports.append(dict(repeat=rep,exact_scenarios=4,exact_states=480));print(rep,'4/4 PC53 boundary regressions exact',flush=True)
(out/'comparison.json').write_text(json.dumps(dict(runs=reports,source_probe_sha256=hashlib.sha256(probe.read_bytes()).hexdigest(),scope='Explicit game-command provenance preserves synthetic PC53 SDK controller fixtures while actual GameCar world test uses PC24.',goal_complete=False),indent=2)+'\n')
