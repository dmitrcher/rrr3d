raise SystemExit("Historical preparation: use the reviewed after snapshots and guarded installer; do not regenerate from active V187.")
from pathlib import Path
import hashlib,json,shutil
w=Path(__file__).resolve().parent
assert not (w/'overlay.json').exists(),'Do not regenerate reviewed snapshots'
a=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=a/'build/macos-arm64-m10/_deps/rrr3d_jolt-src';roots={'source':a,'sdk':sdk};entries=[]
def edit(group,name,transform):
 p=roots[group]/name;s=p.read_text();t=transform(s);assert t!=s,name
 for folder,text in [('before',s),('after',t)]:
  d=w/folder/group/name;d.parent.mkdir(parents=True,exist_ok=True);d.write_text(text)
 entries.append(dict(group=group,path=name,before_sha256=hashlib.sha256(p.read_bytes()).hexdigest(),after_sha256=hashlib.sha256(t.encode()).hexdigest()))
def replace(s,a,b):
 assert s.count(a)==1,(a[:80],s.count(a));return s.replace(a,b)
edit('source','src/Rock3dEngine/header/physics/OriginalVehiclePhysics.h',lambda s:replace(s,'    bool originalGameCarStabilization = false;','    bool originalGameCarStabilization = false;\n    // Explicit provenance for the original fixed JumpProgress acceleration.\n    bool originalGameCarJump = false;'))
edit('source','src/Rock3dGame/source/stub/OriginalRaceSession.cpp',lambda s:replace(s,'    result.originalGameCarStabilization = true;','    result.originalGameCarStabilization = true;\n    result.originalGameCarJump = true;'))
def backend(s):
 s=replace(s,'        bool sourceExtraGravity = false;','        bool sourceExtraGravity = false;\n        bool sourceOriginalJumpGravityV187 = false;')
 s=replace(s,'''        vehicle.sourceExtraGravity = applyExtraGravity;
        if (applyExtraGravity)''','''        vehicle.sourceExtraGravity = applyExtraGravity;
        vehicle.sourceOriginalJumpGravityV187 = gameCarControlled &&
            gameCarCommand.originalGameCarJump &&
            motorrock::tirev55::enabled("RRR3D_EXPERIMENT_GAMECAR_JUMP_V187");
        {
            JPH::BodyLockWrite lock(system_.GetBodyLockInterface(), vehicle.body);
            if (lock.Succeeded())
                lock.GetBody().GetMotionProperties()->SetSourceLinearAccelerationV187(
                    vehicle.sourceOriginalJumpGravityV187 && applyExtraGravity
                        ? toJolt(Vec3{0.0F, 0.0F, -20.0F}) : JPH::Vec3::sZero());
        }
        if (vehicle.sourceOriginalJumpGravityV187 && applyExtraGravity)
            bodies.ActivateBody(vehicle.body); // Original NX_ACCELERATION defaults to wakeup=true.
        if (applyExtraGravity)''')
 s=replace(s,'''            bodies.AddForce(
                vehicle.body,
                toJolt(Vec3{0.0F, 0.0F,
                            source.mass * description_.gravity}));''','''            if (!vehicle.sourceOriginalJumpGravityV187)
                bodies.AddForce(
                    vehicle.body,
                    toJolt(Vec3{0.0F, 0.0F,
                                source.mass * description_.gravity}));''')
 s=replace(s,'''                const JPH::Vec3 acceleration = system_.GetGravity() * (vehicle.sourceExtraGravity ? 2.0F : 1.0F)
                    + toJolt(vehicle.diagnosticAcceleration);''','''                const JPH::Vec3 acceleration = vehicle.sourceOriginalJumpGravityV187 && vehicle.sourceExtraGravity
                    ? system_.GetGravity() + toJolt(Vec3{0.0F, 0.0F, -20.0F}) + toJolt(vehicle.diagnosticAcceleration)
                    : system_.GetGravity() * (vehicle.sourceExtraGravity ? 2.0F : 1.0F)
                        + toJolt(vehicle.diagnosticAcceleration);''')
 return s
edit('source','src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp',backend)
def mh(s):
 a='\t/// Set the mass and inertia tensor'
 s=replace(s,a,'''    // Original NX_ACCELERATION: a per-step acceleration independent of mass.
    // Force reset owns its lifetime, including after the final internal substep.
    void SetSourceLinearAccelerationV187(Vec3Arg acceleration) { mSourceLinearAccelerationV187 = acceleration; }
    Vec3 GetSourceLinearAccelerationV187() const { return mSourceLinearAccelerationV187; }

'''+a)
 s=replace(s,'{ mForce = Float3(0, 0, 0); }','{ mForce = Float3(0, 0, 0); mSourceLinearAccelerationV187 = Vec3::sZero(); }')
 a='\n#ifdef JPH_TRACK_SIMULATION_STATS\n\tSimulationStats\t\t\tmSimulationStats;'
 return replace(s,a,'\n    Vec3 mSourceLinearAccelerationV187 { Vec3::sZero() };\n'+a)
edit('sdk','Jolt/Physics/Body/MotionProperties.h',mh)
def mi(s):
 old='\tmLinearVelocity = LockTranslation(mLinearVelocity + inDeltaTime * (mGravityFactor * inGravity + mInvMass * GetAccumulatedForce()));'
 return replace(s,old,'''    if (mSourceLinearAccelerationV187 == Vec3::sZero())
'''+old+'''
    else
        mLinearVelocity = LockTranslation(mLinearVelocity + inDeltaTime *
            (mGravityFactor * inGravity + mSourceLinearAccelerationV187 + mInvMass * GetAccumulatedForce()));''')
edit('sdk','Jolt/Physics/Body/MotionProperties.inl',mi)
def mc(s):
 s=replace(s,'\tinStream.Write(mForce);','\tinStream.Write(mForce);\n    inStream.Write(mSourceLinearAccelerationV187);')
 return replace(s,'\tinStream.Read(mForce);','\tinStream.Read(mForce);\n    inStream.Read(mSourceLinearAccelerationV187);')
edit('sdk','Jolt/Physics/Body/MotionProperties.cpp',mc)
(w/'overlay.json').write_text(json.dumps(entries,indent=2)+'\n')
old=Path('work/controller-clutch-v186')
for group in roots:
 prereq=json.loads((old/f'{group}-prerequisites.json').read_text())
 for e in json.loads((old/'overlay.json').read_text()):
  if e['group']==group:prereq[e['path']]=e['after_sha256']
 for e in entries:
  if e['group']==group:prereq.pop(e['path'],None)
 for name,sha in prereq.items():assert hashlib.sha256((roots[group]/name).read_bytes()).hexdigest()==sha,name
 (w/f'{group}-prerequisites.json').write_text(json.dumps(prereq,indent=2)+'\n')
print('Six-file Source/SDK overlay prepared; active tree not changed.')
