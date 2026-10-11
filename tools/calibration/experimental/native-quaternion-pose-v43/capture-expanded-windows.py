from pathlib import Path
import subprocess,json,sys,hashlib,argparse
p=argparse.ArgumentParser();p.add_argument('--guest-base',default=r'C:\MotorRock\calibration-probes\contact-pose-v42');p.add_argument('--vm',default='Windows 11');p.add_argument('--export',type=Path,required=True);p.add_argument('--reference',type=Path,required=True);p.add_argument('--out',type=Path,required=True);a=p.parse_args();folder=a.guest_base;out=a.out;out.mkdir(parents=True,exist_ok=False)
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
export=json.loads(a.export.read_text());original=compare.load(a.reference,'windows-physx-d3d9')
def guest(cmd):
 p=subprocess.run(['prlctl','exec',a.vm,'cmd.exe','/c',cmd],capture_output=True,timeout=120)
 if p.returncode:(out/'capture-error.log').write_bytes(p.stdout+p.stderr);p.check_returncode()
 return p.stdout+p.stderr
assert export['sha256'] in guest('certutil -hashfile '+folder+r'\SpringProbe.exe SHA256').decode().lower().replace(' ','')
assert export['core_sha256_unchanged'] in guest('certutil -hashfile '+folder+r'\PhysXCore.dll SHA256').decode().lower().replace(' ','')
report={'probe_sha256':export['sha256'],'core_sha256':export['core_sha256_unchanged'],'runs':{}}
for repeat in range(2):
 names=list(original);dest=folder+f'\\original72-r{repeat}.jsonl';guest('set NX_SPRING_ITERATIONS=4&'+folder+r'\SpringProbe.exe '+dest+' list:'+','.join(names));path=out/f'original-long72-run{repeat}-windows.jsonl';path.write_bytes(guest('type '+dest));got=compare.load(path,'windows-physx-d3d9');assert got==original,'Added fixture definitions alter immutable original scenarios';report['runs']['original'+str(repeat)]={'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'cases':len(got),'all_120_steps_equal_immutable_baseline':True};print('original',repeat,'72 baseline cases unchanged',flush=True)
 names=[f'pose-v42-frozen-{i}' for i in range(6)];dest=folder+f'\\expanded-r{repeat}.jsonl';flags='set NX_SPRING_ITERATIONS=4&set NX_INITIAL_ROTATION_TRACE=1&set NX_INVERSE_MASS_TRACE=1&set NX_LOCAL_POINT_TRACE=1&set NX_CONTACT_QUAT_TRACE=1&set NX_SOLVER_ITERATION_TRACE=1&';guest(flags+folder+r'\SpringProbe.exe '+dest+' list:'+','.join(names));path=out/f'expanded-pose-run{repeat}-windows.jsonl';path.write_bytes(guest('type '+dest));got=compare.load(path,'windows-physx-d3d9');assert set(got)==set(names);report['runs']['expanded'+str(repeat)]={'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'cases':len(got)};print('expanded',repeat,'6 COM/shape fixtures captured',flush=True)
assert compare.load(out/'expanded-pose-run0-windows.jsonl','windows-physx-d3d9')==compare.load(out/'expanded-pose-run1-windows.jsonl','windows-physx-d3d9');report['expanded_repeatable']=True;(out/'capture-manifest.json').write_text(json.dumps(report,indent=2)+'\n')
