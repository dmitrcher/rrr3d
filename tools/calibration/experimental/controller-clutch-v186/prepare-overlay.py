raise SystemExit("Historical preparation script: use the reviewed after/ snapshots and install-guarded.py. Do not regenerate from an active V186 tree.")
from pathlib import Path
import json,hashlib
root=Path.cwd();p=root/'work/controller-clutch-v186';a=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');changes={};flags='RRR3D_EXPERIMENT_GAMECAR_CLUTCH_V186'
def change(rel,old,new):
 text=changes.get(rel,(a/rel).read_text());assert old in text,(rel,old);changes[rel]=text.replace(old,new,1)
carh='src/Rock3dGame/include/OriginalGameCar.h';car='src/Rock3dGame/source/stub/OriginalGameCar.cpp';session='src/Rock3dGame/source/stub/OriginalRaceSession.cpp';engineh='src/Rock3dEngine/header/physics/OriginalVehiclePhysics.h';engine='src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp';header='src/Rock3dEngine/header/physics/NativeGameClutchV186.h'
for rel in [carh,engineh]:change(rel,'bool applyOriginalSteeringTransform = false;','bool applyOriginalSteeringTransform = false;\n    bool clutchLocked = false;\n    float clutchStrength = 0.0F;')
change(car,'    command.angularDamping = dynamics_.angularDamping;','''    const char* originalClutch = std::getenv("'''+flags+'''");
    if (originalClutch && std::strcmp(originalClutch, "1") == 0 && IsClutchLocked())
    {
        command.clutchLocked = true;
        command.clutchStrength = ConsumeClutchStrength();
    }
    command.angularDamping = dynamics_.angularDamping;''')
change(session,'    result.applyOriginalSteeringTransform = command.applyOriginalSteeringTransform;','''    result.applyOriginalSteeringTransform = command.applyOriginalSteeringTransform;
    result.clutchLocked = command.clutchLocked;
    result.clutchStrength = command.clutchStrength;''')
change(session,'        const auto& vehicleDefinition = vehicleForRacer(racer);\n        const Quat inverseRotation{','''        const char* originalClutch = std::getenv("'''+flags+'''");
        if (originalClutch && std::strcmp(originalClutch, "1") == 0)
            return true; // The fixed-step stabilizer consumes the original local-Z request.
        const auto& vehicleDefinition = vehicleForRacer(racer);
        const Quat inverseRotation{''')
change(engine,'#include "physics/NativeGameStabilizeV185.h"','#include "physics/NativeGameStabilizeV185.h"\n#include "physics/NativeGameClutchV186.h"')
change(engine,'bool sourceGamePC24 = false) noexcept','bool sourceGamePC24 = false, bool clutchLocked = false,\n        float clutchStrength = 0.0F) noexcept')
change(engine,'            const auto momentum=sourceGamePC24','''            const float inverseMass = body.GetMotionProperties()->GetInverseMass();
            volatile float publicMass = inverseMass > 0.0F ? 1.0F / inverseMass : 0.0F;
            const auto momentum=clutchLocked
                ? motorrock::stabilizev186::stabilize(q,inv,{{omega.x,omega.y,omega.z}},angularDamping,clampRollInput,clampPitchInput,anyContact,true,clutchStrength,publicMass)
                : sourceGamePC24''')
change(engine,'motorrock::tirev55::enabled("RRR3D_EXPERIMENT_GAMECAR_STABILIZE_V185"));','''(motorrock::tirev55::enabled("RRR3D_EXPERIMENT_GAMECAR_STABILIZE_V185") ||
                         motorrock::tirev55::enabled("'''+flags+'''")),
                        gameCarCommand.clutchLocked,gameCarCommand.clutchStrength);''')
changes[header]=(p/'NativeGameStabilizeCandidateV186.h').read_text().replace('#include "physics/NativeGameStabilizeV185.h"','#include "NativeGameStabilizeV185.h"')
entries=[]
for rel,new in changes.items():
 original=a/rel;before=original.read_bytes() if original.exists() else None
 for tag,text in [('before',before),('after',new.encode())]:
  if text is None:continue
  d=p/tag/'source'/rel;d.parent.mkdir(parents=True,exist_ok=True);d.write_bytes(text)
 entries.append(dict(group='source',path=rel,before_sha256=hashlib.sha256(before).hexdigest() if before is not None else None,after_sha256=hashlib.sha256(new.encode()).hexdigest()))
(p/'overlay.json').write_text(json.dumps(entries,indent=2)+'\n');print('Prepared',len(entries),'files; NOT applied; V185 regression remains live on unchanged binaries.')
