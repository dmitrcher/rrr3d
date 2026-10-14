from pathlib import Path
import json,shutil,gzip,hashlib
r=Path.cwd();w=r/'work/callback-queue-v105';d=r/'work/sync-rrr3d/tools/calibration/experimental/wheel-lifetime-v105';d.mkdir(exist_ok=False);e=d/'evidence';e.mkdir();old=r/'work/sync-rrr3d/tools/calibration/experimental/row-lifetime-v103';m104=json.load(open(r/'work/wheel-objects-v104/source-manifest.json'));m105=json.load(open(w/'source-manifest.json'));after105={f['path']:f for f in m105['files']};files=[]
for f in m104['files']:
 f=dict(f)
 if f['path'] in after105:f['after_sha256']=after105[f['path']]['after_sha256']
 files.append(f)
files.extend(f for f in m105['files'] if f['path'] not in {x['path'] for x in files})
base={'scope':'Self-contained V104 object existence/inactive registration plus V105 persistent callback queue, over acceptedV102 source; five guarded files, default-off flags; no coefficients/tire traversal/oracle states. Full validation pending.','files':files,'goal_complete':False};(d/'base.json').write_text(json.dumps(base,indent=2)+'\n')
for f in files:
 for state in ['before','after']:
  sha=f[state+'_sha256']
  if sha is None:continue
  roots=[w,r/'work/wheel-objects-v104'] if state=='after' else [r/'work/wheel-objects-v104',w]
  src=next(p for p in (x/state/f['location']/f['path'] for x in roots) if p.exists() and hashlib.sha256(p.read_bytes()).hexdigest()==sha);dst=d/state/f['location']/f['path'];dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(src,dst)
for name in ['apply-experiment.py','profile.cfg']:shutil.copy2(old/name,d/name)
for f in ['native-object-inputs.json','production-object-proof.json','live-long21-proof.json','next-curb19-callback-queue-lead.json']:shutil.copy2(r/'outputs/wheel-objects-v104'/f,e/f)
for f in ['native-callback-inputs.json','production-callback-proof.json','live-curb19-order-proof.json']:shutil.copy2(r/'outputs/callback-queue-v105'/f,e/f)
shutil.copy2(r/'outputs/callback-queue-v105/focused/report.json',e/'focused-v105-report.json');shutil.copy2(r/'outputs/wheel-objects-v104/focused/report.json',e/'focused-v104-report.json')
shutil.copy2(r/'outputs/row-lifetime-v103/rejection-inactive-object-proof.json',e/'rejected-v103-inactive-object-proof.json');shutil.copy2(r/'outputs/row-lifetime-v103/next-object-config-native-proof.json',e/'zero-coefficient-object-config-proof.json');shutil.copy2(r/'outputs/row-lifetime-v103/next-long24-tire-order-lead.json',e/'next-long24-tire-order-lead.json')
for name in ['wheel-reset.disasm.txt','release-caller.disasm.txt']:shutil.copy2(r/'work/row-lifetime-v103'/name,e/name)
for name in ['native-contact-lifetime','native-configuration-branch','mac-live-long21','mac-curb13','mac-curb19','mac-curb-order19']:shutil.copytree(r/'outputs/wheel-objects-v104'/name,d/'captures'/'v104'/name)
for name in ['mac-curb19','mac-queue-trace']:shutil.copytree(r/'outputs/callback-queue-v105'/name,d/'captures'/'v105'/name)
shutil.copytree(r/'work/wheel-objects-v104/configuration-branch-tracer',d/'observers/configuration-branch');shutil.copytree(r/'work/row-lifetime-v103/tire-window-tracer',d/'observers/tire-lifetime')
shutil.copy2(r/'work/wheel-objects-v104/object-probe.cpp',d/'object-probe.cpp');shutil.copy2(w/'callback-probe.cpp',d/'callback-probe.cpp')
(d/'probe-environment.json').write_text(json.dumps(json.load(open(r/'work/frozen-v105/manifest.json'))['environment'],indent=2)+'\n');shutil.copy2(old/'evidence/checkpoint-expected.jsonl.gz',e/'checkpoint-expected.jsonl.gz')
with gzip.open(e/'retained-roll-contact-v102.jsonl.gz','wt') as dst,gzip.open(r/'outputs/contact-birth-v102/broad/candidate-run0.jsonl.gz','rt') as src:
 for line in src:
  x=json.loads(line)
  if x.get('event')=='calibration.sample' and x['detail']=='body-wheel-contact-roll':dst.write(line)
scripts=d/'reproduction-host-scripts';scripts.mkdir()
for root,names in [(r/'work/wheel-objects-v104',['prepare-candidate.py','prepare-queue-candidate.py','capture-native-contact-lifetime.py','capture-native-configuration-branch.py','capture-mac-live-long21.py','prove-live-long21.py','capture-mac-curb13.py','capture-mac-curb19.py','capture-mac-curb-order19.py']),(w,['run-focused.py','run-broad.py','compare-broad.py','run-gates-complete-profile.py','run-game-smoke.py','run-game-render-race.py','practical-error-audit.py','compare-regression.py','capture-mac-curb19.py','export-draft.py'])]:
 for name in names:shutil.copy2(root/name,scripts/(root.name+'-'+name))
tail=(old/'verify.py').read_text();tail=tail[tail.index(' if args.probe:'):]
head='''"""Verify production object and callback helpers against actual Native observations."""
from pathlib import Path
import argparse,json,subprocess,os,struct,gzip,tempfile,hashlib
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args();bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
base=json.loads((root/'base.json').read_text());inc=root/'after/sdk/Jolt/Physics/Vehicle'
for f in base['files']:assert hashlib.sha256((root/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(root/'profile.cfg');report={'arithmetic':[],'goal_complete':False}
a=json.loads((root/'evidence/native-object-inputs.json').read_text());queries=a['query_inputs'];configs=a['configuration_inputs'];objects=''.join(f"{x['query_mode']} {int(x['spring_handle_present'])} 0\\n" for x in queries)+''.join(f"1 1 {x['original_travel_word']}\\n" for x in configs);object_expected=[[int(x['actual_group_reset']),0] for x in queries]+[[0,int(x['actual_Native_positive_travel_spring_branch'])] for x in configs]
fixtures=json.loads((root/'evidence/native-callback-inputs.json').read_text())['fixtures'];callback_data='';callback_expected=[]
for f in fixtures:
 callback_data+=str(len(f['frames']))+'\\n'
 for x in f['frames']:callback_data+=' '.join(str(int(v)) for v in x['current_query_wheels'])+'\\n';callback_expected.append(x['actual_Native_prepare_order'])
with tempfile.TemporaryDirectory(prefix='motorrock-v105-') as tmp:
 tmp=Path(tmp)
 for opt in ['O0','O2']:
  for name,data,expected in [('object',objects,object_expected),('callback',callback_data,callback_expected)]:
   exe=tmp/(name+'-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-I'+str(inc),str(root/(name+'-probe.cpp')),'-o',str(exe)],check=True);actual=[list(map(int,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode()).decode().splitlines()];assert actual==expected
  report['arithmetic'].append({'optimization':opt,'actual_Native_query_inputs_exact':len(queries),'actual_Native_positive_travel_branch_hits_exact':len(configs),'actual_Native_callback_prepare_frames_exact':len(callback_expected),'zero_travel_and_arbitrary_callback_domains_certified':False})
'''
tail=tail.replace('print(json.dumps(report,indent=2))',''' if args.probe:
  expected=[physical(x['sample']) for x in map(json.loads,gzip.open(root/'evidence/retained-roll-contact-v102.jsonl.gz','rt'))];path=tmp/'retained-roll-contact.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'list:body-wheel-contact-roll'],env=env,check=True,capture_output=True,timeout=180);actual=[physical(x['sample']) for x in map(json.loads,path.open()) if x.get('event')=='calibration.sample'];assert actual==expected and len(actual)==180;report['fresh_rejected_fixture_retained_v102']={'states':180,'all_observable_float_words_exact_accepted_v102':True,'all_words_exact_Windows_claimed':False}
print(json.dumps(report,indent=2))''')
(d/'verify.py').write_text(head+tail)
s=(r/'work/row-lifetime-v103/test-exported-installer.py').read_text().replace('row-lifetime-v103','wheel-lifetime-v105').replace('physics-v103','physics-v105').replace('motorrock-v103','motorrock-v105');(w/'test-exported-installer.py').write_text(s)
print('Five-file combined V105 draft exported over acceptedV102; full validation pending')
