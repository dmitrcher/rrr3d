# V171: original GameCar wake events

Research candidate over the accepted V169 Source and unchanged V154 Jolt SDK. Five owned Source files; runtime flag `RRR3D_EXPERIMENT_GAMECAR_WAKE_V171=1`, default OFF. Installed ordinary game has not been promoted to this candidate. Windows game and SDK remain the reference.

## Cause and repair

The original GameCar::SetMoveCar wakes its attached NxActor when the move state changes to a non-None state. The Mac controller previously did not forward this event, while its physics adapter independently reset wake on every held nonzero input. V171 carries one pending wake event through GameCar -> OriginalRaceSession -> VehicleDriveCommand and suppresses that generic held-input wake for controller-driven vehicles under the candidate flag. Logical car copies do not transfer physical binding or pending requests. Reset clears pending requests; an actual fixed-step callback establishes a live binding.

The new diagnostic `RRR3D_GAMECAR_EVENT_REPLAY_V171=1` avoids calling the zero velocity-impulse API every frame, which otherwise masks the event behavior. Actual nonzero impulses at steps720/780 are preserved. Raw V133 replay behavior is unchanged.

## Evidence and limits

* `comparison.json`: actual original DLL/PDB-resolved GameCar constructor, SetMoveCar and NxActor, versus actual compiled Source GameCar/Jolt libraries. All40 command/binding cases and all360 wake-only steps match, twice. OFF negative control differs in11 command cases and167 awake states, first step54. Wake-only scene disables gravity, contacts, motor forces and stabilization; this proves wake semantics, not full game trajectory parity.
* `evidence/native-schedule-d`: only authoritative original capture. Original putToSleep is buffered, so both sleep setup and post-command observation occur after completed simulate/fetch. Earlier native/native-schedule/native-schedule-c immediate queries were stale and are excluded from acceptance. Original game DLL/PDB and WorldProbeV133 before/after hashes are in the capture manifest; SDK hashes are separately checked against the immutable reference after capture.
* `evidence/gamecar-event-replay/comparison.json`: actual Mac session/controller bridge,900 commands and900 states per run. Events occur only at120/360/420/480/600; two enabled runs identical. Inputs, impulses and serialized scene identical to OFF. This is transport/repeatability evidence, not a Windows full-controller trajectory comparison.
* `evidence/raw-regression-comparison.json`: original Windows reference and V171 raw actuator replay, four real maps x900 states x2 repeats,7200/7200 exact original float32 states, including pose, velocity, angular momentum, wheel observables and awake state. Serialized scenes unchanged.
* `ctest.log`: all53 project tests pass. V169 full608-scenario gate remains accepted; it is not claimed to have been rerun for V171. These changes concern the GameCar/event path; the raw replay regression checks that previously accepted physics is preserved.
* `installer-proof.json`: guarded overlay applies5 files, then0; unknown prerequisites are rejected before any mutation. The SDK layout is unchanged.

An initial replay runner accidentally overwrote the OFF flag with ON. Only that invalid OFF run was archived with hashes and rerun after correcting the runner; enabled repeats were retained. The negative-control receipt is preserved under evidence. No parameter tuning or torque arithmetic changes are included.

## Next stage

Observe the actual Windows game floating-point context, then compare original MotorProgress, RPM, gear changes, brake and steering outputs against Source on identical measured actor states. V170 proved that the current folded SEM arithmetic can differ, but production precision must be known before repairing it. After controller agreement, run full ordinary-game single-car trajectories; then controlled multiple bodies and AI; finally promote a verified Mac build and complete screenshots/loading-log and release verification. The goal remains active and incomplete.
