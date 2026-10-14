from pathlib import Path
import json,hashlib
root=Path.cwd();work=root/'work/static-box-query-v107';source=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');p=source/'src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp';s=p.read_text();b=work/'before/adapter/src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp';b.parent.mkdir(parents=True,exist_ok=True);assert not b.exists();b.write_text(s)
old='''                if(collector.nativeVehicleV70 && flag("RRR3D_EXPERIMENT_NATIVE_WHEEL_BOX_QUERY_V80") &&
                   lock.GetBody().IsDynamic() && vehicleIndex(lock.GetBody().GetUserData(),sourceVehicleV80)) {'''
new='''                const bool originalDynamicBoxV80=collector.nativeVehicleV70 && flag("RRR3D_EXPERIMENT_NATIVE_WHEEL_BOX_QUERY_V80")
                    && lock.GetBody().IsDynamic() && vehicleIndex(lock.GetBody().GetUserData(),sourceVehicleV80);
                // Static boxes enter the same original 1ae390 -> 1ea750 query.
                // Transform the local face point back to world, rather than
                // interpolating the world ray; the latter changes lever arms.
                const bool originalStaticBoxV107=lock.GetBody().IsStatic()
                    && flag("RRR3D_EXPERIMENT_NATIVE_STATIC_WHEEL_BOX_QUERY_V107");
                if(originalDynamicBoxV80 || originalStaticBoxV107) {'''
assert s.count(old)==1;s=s.replace(old,new);p.write_text(s)
m={'scope':'Default-off original local-face/world-transform wheel query for static source boxes, using existing independently traced V80 kernel. Immutable original Windows static query17words matches at curb13/bump50 under O0/O2. No coefficients, frame/case dispatch or reference injection. V106 source base, adapter only; private SDK layout unchanged.','files':[{'location':'adapter','path':str(p.relative_to(source)),'before_sha256':hashlib.sha256(b.read_bytes()).hexdigest(),'after_sha256':hashlib.sha256(p.read_bytes()).hexdigest()}],'goal_complete':False};(work/'source-manifest.json').write_text(json.dumps(m,indent=2)+'\n')
