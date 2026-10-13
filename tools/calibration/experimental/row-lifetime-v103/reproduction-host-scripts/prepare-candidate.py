from pathlib import Path
import json,hashlib,subprocess,os
root=Path.cwd();w=root/'work/row-lifetime-v103';o=root/'outputs/row-lifetime-v103'
proof=json.loads((o/'same-input-query-proof.json').read_text());header=(w/'NativeWheelRowLifetimeV103.h').read_text()
cpp='''#include "NativeWheelRowLifetimeV103.h"
#include <iostream>
int main(){unsigned mode,present;while(std::cin>>mode>>present)std::cout<<NativeWheelRowLifetimeV103::resetActorRows(mode,present!=0)<<"\\n";}
''';(w/'query-probe.cpp').write_text(cpp);data=''.join(f"{r['query_mode']} {int(r['spring_handle_present'])}\n" for r in proof['actual_query_inputs']);runs=[]
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))}
for opt in ['O0','O2']:
 exe=w/f'query-{opt}';subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,str(w/'query-probe.cpp'),'-o',str(exe)],check=True)
 values=list(map(int,subprocess.check_output([str(exe)],input=data.encode(),env=env).decode().splitlines()));assert values==[int(r['actual_group_reset']) for r in proof['actual_query_inputs']];runs.append({'optimization':opt,'actual_Native_query_branch_inputs_exact':len(values),'actual_reset_groups':sum(values)})
(o/'production-query-proof.json').write_text(json.dumps({'scope':'Production header branch binds128 actual Native mode/prior-handle observations,20 actual actor-wide reset groups O0/O2; no claim of full physical histories yet.','runs':runs,'goal_complete':False},indent=2)+'\n')
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=active/'build/macos-arm64-m10/_deps/rrr3d_jolt-src';p=sdk/'Jolt/Physics/Vehicle/VehicleConstraint.cpp';before=p.read_text();assert 'V103' not in before
s=before.replace('#include <Jolt/Physics/Vehicle/VehicleConstraint.h>','#include <Jolt/Physics/Vehicle/VehicleConstraint.h>\n#include <Jolt/Physics/Vehicle/NativeWheelRowLifetimeV103.h>')
start=s.index('void VehicleConstraint::SetupVelocityConstraint(float inDeltaTime)');pos=s.index('\tRMat44 body_transform = mBody->GetWorldTransform();',start)
block='''    const char* resetFlagV103=std::getenv("RRR3D_EXPERIMENT_NATIVE_WHEEL_GROUP_RESET_V103");
    const bool resetGroupV103=creationOrder && resetFlagV103 && std::strcmp(resetFlagV103,"1")==0;
    if(resetGroupV103)for(uint index=0;index<mWheels.size();++index) {
        const Wheel* wheel=mWheels[index];
        const bool currentQueryHit=wheel->mContactBody!=nullptr && !wheel->mSourceContactLost;
        const bool springPresent=mSourceSpringCreationOrderV64[index]!=0 && wheel->mSuspensionPart.IsActive();
        if(currentQueryHit && NativeWheelRowLifetimeV103::resetActorRows(0,springPresent)) {
            NativeWheelRowLifetimeV103::clearActorRows(mSourceSpringCreationOrderV64,mSourceStopCreationOrderV64);
            break;
        }
    }
'''
s=s[:pos]+block+s[pos:];files=[]
for rel,b,a in [('Jolt/Physics/Vehicle/VehicleConstraint.cpp',before,s),('Jolt/Physics/Vehicle/NativeWheelRowLifetimeV103.h',None,header)]:
 for state,data in [('before',b),('after',a)]:
  if data is not None:
   q=w/state/rel;q.parent.mkdir(parents=True,exist_ok=True);q.write_text(data)
 files.append({'path':rel,'location':'sdk','before_sha256':hashlib.sha256(b.encode()).hexdigest() if b is not None else None,'after_sha256':hashlib.sha256(a.encode()).hexdigest()})
(w/'source-manifest.json').write_text(json.dumps({'scope':'Default-off Native actor-wide vertical-row recreation on current wheel query with prior spring object absent. Only persistent row-index lifetime; no coefficients, case/frame dispatch or state injection. All later registration/sort rules unchanged.','files':files,'goal_complete':False},indent=2)+'\n')
assert not (sdk/'Jolt/Physics/Vehicle/NativeWheelRowLifetimeV103.h').exists();p.write_text(s);(sdk/'Jolt/Physics/Vehicle/NativeWheelRowLifetimeV103.h').write_text(header);print('Installed candidate after128 Native condition inputs exact O0/O2',runs,flush=True)
