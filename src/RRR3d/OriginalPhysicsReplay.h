#pragma once

#include "OriginalRace.h"
#include "physics/OriginalVehiclePhysics.h"

namespace rrr3d::diagnostics
{
// Original map geometry and GameCar fixed-step drive, without AI, weapons,
// bonuses, race progression or automatic respawns. Emits parity JSONL.
bool replaySoloPhysics(const r3d::game::originalrace::Race& race,
                       const r3d::physics::WorldDescription& description);
}
