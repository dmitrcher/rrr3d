from pathlib import Path
import hashlib,json
root=Path.cwd();work=root/'work/tire-lifetime-v106';source=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=source/'build/macos-arm64-m10/_deps/rrr3d_jolt-src';files={};records=[]
for name in ['VehicleConstraint.h','VehicleConstraint.cpp','WheeledVehicleController.cpp']:
 p=sdk/'Jolt/Physics/Vehicle'/name;s=p.read_text();files[name]=s
 b=work/'before/Jolt/Physics/Vehicle'/name;b.parent.mkdir(parents=True,exist_ok=True);assert not b.exists() or b.read_text()==s;b.write_text(s)
def edit(name,old,new):
 assert files[name].count(old)==1,(name,old);files[name]=files[name].replace(old,new)
edit('VehicleConstraint.h','    const Array<uint64>& GetSourceContactCreationOrderV87() const { return mSourceContactCreationOrderV79; }','''    const Array<uint64>& GetSourceContactCreationOrderV87() const { return mSourceContactCreationOrderV79; }
    const Array<uint64>& GetSourceTireObjectOrderV106() const { return mSourceTireObjectOrderV106; }''')
edit('VehicleConstraint.h','    uint64 mSourceCallbackQueueCounterV105 = 0;','''    uint64 mSourceCallbackQueueCounterV105 = 0;
    Array<uint64> mSourceTireObjectOrderV106;
    uint64 mSourceTireObjectCounterV106 = 0;''')
edit('VehicleConstraint.cpp','\tCalculatePitchRollConstraintProperties(body_transform);\n}\n\nvoid VehicleConstraint::ResetWarmStart()','''    const char* tireObjectFlagV106=std::getenv("RRR3D_EXPERIMENT_NATIVE_TIRE_OBJECT_LIFETIME_V106");
    const bool tireObjectsV106=callbackQueueV105 && tireObjectFlagV106 && std::strcmp(tireObjectFlagV106,"1")==0;
    if(tireObjectsV106) {
        if(mSourceTireObjectOrderV106.size()!=mWheels.size())mSourceTireObjectOrderV106.resize(mWheels.size(),0);
        for(uint index:creationIndicesV79) {
            // Spring/stop group recreation leaves both tire objects alive.
            // Loss of the cached wheel contact removes the complete group.
            if(!mWheels[index]->mContactBody)mSourceTireObjectOrderV106[index]=0;
            else if(mWheels[index]->HasContact() && !mSourceTireObjectOrderV106[index])
                mSourceTireObjectOrderV106[index]=++mSourceTireObjectCounterV106;
        }
        const char* path=std::getenv("RRR3D_TIRE_OBJECT_TRACE_FILE_V106");
        if(path && *path)if(FILE* file=std::fopen(path,"a")) {
            std::fprintf(file,"{\\"sdk_counter\\":%u,\\"tire_orders\\":[",GetSourceSimulationCounterV59());
            for(uint index=0;index<mWheels.size();++index)std::fprintf(file,"%s%llu",index?",":"",static_cast<unsigned long long>(mSourceTireObjectOrderV106[index]));
            std::fprintf(file,"]}\\n");std::fclose(file);
        }
    }
\tCalculatePitchRollConstraintProperties(body_transform);
}

void VehicleConstraint::ResetWarmStart()''')
edit('WheeledVehicleController.cpp','    const auto& sourceOrder=mConstraint.GetSourceContactCreationOrderV87();','''    const char* tireObjectFlagV106=std::getenv("RRR3D_EXPERIMENT_NATIVE_TIRE_OBJECT_LIFETIME_V106");
    const auto& tireObjectOrderV106=mConstraint.GetSourceTireObjectOrderV106();
    const bool tireObjectsV106=tireObjectFlagV106 && std::strcmp(tireObjectFlagV106,"1")==0
        && tireObjectOrderV106.size()==mConstraint.GetWheels().size();
    const auto& sourceOrder=tireObjectsV106?tireObjectOrderV106:mConstraint.GetSourceContactCreationOrderV87();''')
for name,s in files.items():
 p=sdk/'Jolt/Physics/Vehicle'/name;p.write_text(s);b=work/'before/Jolt/Physics/Vehicle'/name
 records.append({'location':'sdk','path':str(p.relative_to(sdk)),'before_sha256':hashlib.sha256(b.read_bytes()).hexdigest(),'after_sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
(work/'source-manifest.json').write_text(json.dumps({'scope':'Default-off independent tire object registration age. Actual latest object creation traversal; spring/stop reset retains tires. V105 required. No coefficients, case/frame dispatch or measured state injection. Private VehicleConstraint layout requires all consumers rebuilt.','files':records,'goal_complete':False},indent=2)+'\n')
