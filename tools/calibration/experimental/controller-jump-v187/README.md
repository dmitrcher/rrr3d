# V187: fixed JumpProgress acceleration, experimental

Original GameCar::JumpProgress adds Scene::cDefGravity=(0,0,-20) in NX_ACCELERATION mode, independently of scene gravity. The Mac adapter previously multiplied nominal mass by scene gravity, giving no additional acceleration in a zero-gravity scene. V187 carries explicit Session command provenance and uses a per-step acceleration accumulator independent of mass. The new path requires RRR3D_EXPERIMENT_GAMECAR_JUMP_V187=1 and originalGameCarJump=true; both are required. The ordinary game profile is not promoted.

## Evidence boundaries

- Original source plus matching DLL/PDB identity and bounded LLVM disassembly establish the method, constant reference, mode5/wakeup=true and branch order. They are static evidence. The constant's value comes from original Physx.cpp, not a runtime memory read. Ghidra default analysis timed out after 300 seconds and returned no function Evidence; the session was closed. This limitation did not authorize a VM launch.
- Recorded Windows SDK trajectories for gravity, game-air-gravity and game-air-pitch contain 120 steps each. Two Mac repeats match all float32 observable states. For gravity, Mac scene gravity=0 plus the original fixed extra acceleration is compared to SDK scene gravity=-20. This is bounded total-acceleration equivalence; the native GameCar method itself was not executed. The other two cases also exercise the recorded SDK NX_ACCELERATION path at scene gravity=-20.
- 360 command gate cases compare actual Source GameCar with the extracted original C++ method compiled into an API-recording unit adapter. This checks strict speed>1, nonzero pitch torque, contact boolean and post-decrement spring timer semantics. The adapter is not a real SDK actor or Windows binary execution; it does not establish live Windows contact/timer parity.
- 51 Source/Jolt wake cases cover activation ticks0..50 with synthetic GameCar contact booleans. The pending-sleep boundary at tick24 initially cancelled the acceleration. Automatic wakeEnergyV128 before body activation fixes it; already-inactive ticks25..50 are also covered.
- Seven real Jolt body masses check acceleration independence, two internal substeps, per-update force reset, motion reset and pending-force save/restore. An initial standalone harness crash came from a default-constructed, uninitialized JobSystemSingleThreaded; jobs(2048) fixed the harness. That crash is not engine-failure evidence. The subsequent numerical ResetMotion reproducer exposed a real retained-acceleration bug; it was fixed and preserved.
- The actual application Session smoke asserts forwarding of the new marker. Source game event replay is deterministic, but remains Source-only acceptance.
- General608, map7200 and V186 clutch720 regression reports carry their own scope. They do not prove complete original GameCar updates or ordinary gameplay parity.

No coefficients, motor power or track parameters were fitted. No VM was launched and no historical data was removed.

## Build and compatibility

The guarded six-file overlay modifies three Source files and three Jolt MIT files. Apply only to the exact V186 prerequisites, then rebuild the entire project: MotionProperties layout changes, so old objects and new headers must not be mixed. The added force accumulator is saved/restored with other forces and cleared by ResetForce and ResetMotion.

Jolt StateRecorder byte layout changes even when the experimental flag is off. Pre-V187 SDK state streams are not interchangeable with this build. No ordinary application consumer was found; this remains a diagnostic build constraint. Golden JSON recordings and the source calibration schema are unchanged.

## Full goal remains open

Native GameCar JumpProgress runtime behavior in the same scene, real wheel contact transitions, native timer expiry, complete original OnFixedStep, shifted-center/edge cases, continuous races, AI and graphics/menu parity still need their appropriate evidence. Existing recordings and static analysis will be used without starting the VM. Missing runtime evidence must remain explicit; these bounded tests cannot stand in for the full goal.
