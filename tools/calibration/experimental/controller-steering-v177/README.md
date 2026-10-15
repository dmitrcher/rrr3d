# V177: preserve physical steering for original GameCar commands

Research candidate over V176 Source, unchanged V154 SDK. Six owned Source files. `RRR3D_EXPERIMENT_GAMECAR_STEER_V177=1` enables the original gameplay steering ownership; default OFF. Installed ordinary Mac app is not promoted. Windows remains immutable.

Original WheelsProgress changes CarWheel's visual steer angle and directly turns the actor around the rear pivot. It does not send that visual angle to NxWheelShape::setSteerAngle. The Mac adapter previously also passed the visual angle into Jolt's physical wheel steering. This changes the tire directions in addition to the original body yaw.

The GameCar command explicitly requests preserving the existing physical angles; OriginalRaceSession forwards the request. The owned Jolt controller skips only its angle assignment in PreCollide for that command. Existing nonzero angles are preserved rather than forced to zero. Raw SDK commands keep their ordinary controller behavior, regardless of the flag. The per-step request resets when command ownership changes. The adapter is compiled without native C++ RTTI to match the existing Jolt library; no dynamic_cast/typeid is used there. SDK hashes pin PreCollide's implementation and the callback order on which this contract depends. No Jolt SDK source changes or parameter tuning.

## Evidence

* Actual original15-case physical/visual steering matrix: two identical Native repetitions. Actual Source GameCar with four owned logical wheels and four actual Jolt wheels: enabled15/15 twice, disabled1/15 twice. Raw commands with flag enabled15/15 twice. Only coordinate-converted signed zero is canonicalized in comparison; all other values use exact float32 words.
* Actual OriginalRaceSession -> GameCar -> Jolt path: two900-step repeats. Visual steering is nonzero120 steps; physical angles remain zero on all900. Disabled path physically steers120 steps. This validates the bridge, not a full Windows trajectory.
* Rebuilt Source engine/gear/brake/RPM matrix remains13512/13512 twice against V175 immutable original captures and measured input files.
* Four raw-actuator real maps:900/900 original states on each track0/16/48/64 twice,7200 exact; serialized fixtures unchanged.
* All53 CTests pass. Guarded installer changes6 files then0; unknown prerequisites rejected before mutation. Relevant unchanged SDK method files are hash pinned.
* Full608-case regression passes:255864 observable float32 states per repeat, two identical repetitions, zero remaining differences against the unchanged original Windows reference. The calibration binary contains the V177 engine adapter; V179 later changes only the GameCar library, which this SDK-only calibration executable does not link.

V178 additionally measures moving/contact body poses; V179 corrects the separate steering-speed deadband. Nonzero-speed body rotation arithmetic, identity-yaw pivot translations, stabilization/jump composition, ordinary full race and other cars/equipment remain open. This checkpoint does not complete the goal.
