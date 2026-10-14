from pathlib import Path
import json,hashlib
root=Path.cwd();w=root/'work/callback-queue-v105';w.mkdir(exist_ok=False);sdk=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/_deps/rrr3d_jolt-src');cp=sdk/'Jolt/Physics/Vehicle/VehicleConstraint.cpp';hp=sdk/'Jolt/Physics/Vehicle/VehicleConstraint.h';c=cp.read_text();h=hp.read_text();assert 'V105' not in c and 'V105' not in h
header='''#pragma once
#include <cstdint>
namespace NativeWheelCallbackQueueV105 {
inline void enqueue(bool currentContact, uint64_t &order, uint64_t &counter) {
    if(currentContact && !order)order=++counter;
}
inline bool before(uint64_t a, uint64_t b) { return a < b; }
}
'''
nc=c.replace('#include <Jolt/Physics/Vehicle/NativeWheelObjectLifetimeV104.h>','#include <Jolt/Physics/Vehicle/NativeWheelObjectLifetimeV104.h>\n#include <Jolt/Physics/Vehicle/NativeWheelCallbackQueueV105.h>')
needle='    if(creationOrder)for(uint index:creationIndicesV79) {';assert nc.count(needle)==1
block='''    const char* queueFlagV105=std::getenv("RRR3D_EXPERIMENT_NATIVE_CALLBACK_QUEUE_V105");
    const bool callbackQueueV105=objectLifetimeV104 && queueFlagV105 && std::strcmp(queueFlagV105,"1")==0;
    if(callbackQueueV105) {
        if(mSourceCallbackQueueOrderV105.size()!=mWheels.size())mSourceCallbackQueueOrderV105.resize(mWheels.size(),0);
        bool allCurrentRanksKnownV105=mSourceContactCreationOrderV79.size()==mWheels.size();
        for(uint index:creationIndicesV79) {
            NativeWheelCallbackQueueV105::enqueue(mWheels[index]->HasContact(),mSourceCallbackQueueOrderV105[index],mSourceCallbackQueueCounterV105);
            if(mWheels[index]->HasContact() && (!allCurrentRanksKnownV105 || !mSourceContactCreationOrderV79[index]))allCurrentRanksKnownV105=false;
        }
        // Supported broadphase ranks already encode Native callback order.
        // With unregistered static colliders, keep callback arrival order;
        // spring/stop recreation does not recreate the wheel callbacks.
        if(!allCurrentRanksKnownV105)std::stable_sort(creationIndicesV79.begin(),creationIndicesV79.end(),[&](uint a,uint b) {
            return NativeWheelCallbackQueueV105::before(mSourceCallbackQueueOrderV105[a],mSourceCallbackQueueOrderV105[b]);
        });
        const char* traceV105=std::getenv("RRR3D_WHEEL_CALLBACK_TRACE_FILE_V105");
        const char* stepV105=std::getenv("RRR3D_WHEEL_CALLBACK_TRACE_STEP_V105");
        if(traceV105 && *traceV105 && (!stepV105 || GetSourceSimulationCounterV59()==uint(std::strtoul(stepV105,nullptr,10))))
            if(FILE* file=std::fopen(traceV105,"a")) {
                std::fprintf(file,"{\\"sdk_counter\\":%u,\\"all_current_ranks_known\\":%s,\\"callback_orders\\":[",GetSourceSimulationCounterV59(),allCurrentRanksKnownV105?"true":"false");
                for(uint i=0;i<mWheels.size();++i)std::fprintf(file,"%s%llu",i?",":"",static_cast<unsigned long long>(mSourceCallbackQueueOrderV105[i]));
                std::fprintf(file,"],\\"prepare_visits\\":[");
                for(uint i=0;i<creationIndicesV79.size();++i)std::fprintf(file,"%s%u",i?",":"",creationIndicesV79[i]);
                std::fprintf(file,"]}\\n");std::fclose(file);
            }
    }
''';nc=nc.replace(needle,block+needle)
needle='    Array<bool> mSourceSpringObjectPresentV104;';assert h.count(needle)==1;nh=h.replace(needle,needle+'''
    Array<uint64> mSourceCallbackQueueOrderV105;
    uint64 mSourceCallbackQueueCounterV105 = 0;
''');files=[]
changes=[('Jolt/Physics/Vehicle/VehicleConstraint.cpp',cp,c,nc),('Jolt/Physics/Vehicle/VehicleConstraint.h',hp,h,nh),('Jolt/Physics/Vehicle/NativeWheelCallbackQueueV105.h',sdk/'Jolt/Physics/Vehicle/NativeWheelCallbackQueueV105.h',None,header)]
for rel,p,b,a in changes:
 if b is None:assert not p.exists()
 for state,data in [('before',b),('after',a)]:
  if data is not None:
   q=w/state/'sdk'/rel;q.parent.mkdir(parents=True,exist_ok=True);q.write_text(data)
 files.append({'location':'sdk','path':rel,'before_sha256':hashlib.sha256(b.encode()).hexdigest() if b is not None else None,'after_sha256':hashlib.sha256(a.encode()).hexdigest()})
(w/'source-manifest.json').write_text(json.dumps({'scope':'Default-off persistent wheel callback arrival order for current contacts without complete broadphase ranks, independent of spring/stop object recreation. V104 required. Original known-rank ordering remains. No coefficients, tire traversal, measured-state resets or frame/case dispatch. Private VehicleConstraint layout changes; rebuild all consumers.','files':files,'goal_complete':False},indent=2)+'\n')
for rel,p,b,a in changes:p.write_text(a)
print('Installed V105 callback queue candidate over V104; current fullV104 snapshot remains frozen; full rebuild required',flush=True)
