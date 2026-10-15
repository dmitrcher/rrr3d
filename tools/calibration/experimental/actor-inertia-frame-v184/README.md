# V184: preserve original actor inertia axes

One Source-file overlay over V183; SDK remains V181. `RRR3D_EXPERIMENT_NATIVE_ACTOR_INERTIA_FRAME_V184=1`, default OFF. sourceVehicleMassProperties already computes original actor-axis moments. Jolt decomposed/sorted them again and stored a permutation quaternion. For equal moments the permutation uses an approximate90-degree quaternion, changing the reconstructed local tensor by a few float32 units. Restore the original actor-axis inverse diagonal with identity inertia rotation after body creation; retain rotation DOF locks. No mass, engine, tire, suspension or steering coefficient is fitted.

Actual SDK/Source tensor words with correction OFF differ: native[1145321931,1145321931,1151567371], Source[1145321933,1145321933,1151567372]. ON all three match exactly. This also closes the remaining tiny tilted translations during integration.

## Accepted evidence

* Final stamped Native observer and actual compiled Source GameCar/Jolt: six45-case fixtures,270 distinct cases, all body positions/rotations and observed original angles match, twice (540 checked poses). Covers zero/nonzero speeds, deadband edges, reverse speed, full/half lock, COM moved after mass creation, tilt and headings90/126/180. One car, no AI, fresh scenes and exact measured initial states. Only coordinate signed zero is canonicalized. Source executable and linked libraries are hash pinned; Native embeds its actual build/source identity and verifies immutable DLLs.
* Full608 scenarios,255864 states per repetition, two identical repetitions, zero differences. New frozen V184 binary/profile hashes are recorded. Immutable Windows golden remains unchanged.
* Tracks0/16/48/64:900 states each twice,7200 exact; fixture words unchanged.53 CTests pass. Motor/gear/brake/RPM Source ABI regression27024 results remains exact on the V183 controller, which V184 does not change.
* Actual OriginalRaceSession replay repeats900 steps exactly. Steering effects start at step480; steps0..479 stay unchanged. This checks the real bridge and repeatability; it is not a complete Windows ordinary-race reference.
* Guarded overlay chain181(10 files incl4 SDK),182(1 Source),183(6 Source),184(1 Source). Unsupported prerequisites must fail before any mutation. Current combined verifier reports0 changes.

The compact Mac research package contains the actually tested driver, exact profile, immutable Native captures and a standalone verifier. It does not replace the installed game. No Windows SDK binaries/headers/PDBs or Windows executables are distributed. Source and Jolt patch licenses accompany the checkpoint.

## Next

Nonzero angular stabilization/clutch/clamps, front/rear/air contact gates and JumpProgress; then complete actual OnFixedStep single-car trajectory with identical equipment and inputs, wider cars/maps/multibody/AI and ordinary Mac build promotion. Arbitrary COM during mass creation is distinct from the current post-creation COM-offset fixture and must be audited separately. Full goal remains active.
