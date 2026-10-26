# V189: original JumpProgress runtime checks without a VM

The original GameCar::JumpProgress now executes through fresh, owned MSVC2008 observers on real PhysX actors in the dedicated CrossOver prefix. The unchanged original DLL/PDB and SDK are pinned by hash, and the method RVA is checked against0x498110. No active-code hooks, fake actors, AI, future-state inputs or parameter fitting are used. Mac production physics remains V187; the installed ordinary game/profile is not promoted.

## Numerical checks

- **530 isolated inputs**, two independent compatibility runs and two Source/Jolt runs: all shared initial states, post-method spring timers and public post-step position, velocity, rotation, angular momentum and activity match. The explicit existing equivalence policy canonicalizes signed zero and reports those occurrences; integer float-bit words are never converted to numeric floats for comparison.
- The matrix contains360 scalar contact/speed/pitch/spring/timestep gates,42 mass/scene-gravity combinations,120 two-axis speed-boundary inputs and8 velocity-direction inputs. Contact booleans here are synthetic function inputs; the four wheels are free. The method delta and a subsequent fixed1/60 integration step are deliberately distinct inputs. This is an isolated method test, not a game-clock integration test.
- **336/530 differences with V187 disabled**, same Source binary and input. This negative control demonstrates that the tests detect the former scene-gravity-dependent extra-force route.
- A fresh SDK trajectory harness first reproduces the immutable saved Windows gravity, game-air-gravity and game-air-pitch controls:360 states, twice. The actual original method replaces only the two existing extra-gravity/pitch calls in the two airborne controls:240 states, twice, all exact against the same saved Windows SDK data. Actual Mac GameCar/Jolt also matches those240 states twice.
- The trajectory harness creates the same actor under PC53, then invokes the original method under PC24. Its contact flag comes from the real SDK wheel getters; all contacts remain false throughout the qualified airborne cases. The original constructor, private method and SDK actor are real, while complete GameCar::OnFixedStep, game callbacks, continuous ground-contact transitions and actual race creation remain outside this proof.

The530 expected method results are **CrossOver compatibility evidence**, not a newly recorded native Windows GameCar golden. V188 disproved universal equivalence for other arithmetic branches. The saved Windows SDK trajectories remain the independent immutable reference. Source-only input/output agreement cannot expand this scope.

## Logger precision correction

The first long-run comparison reported17/120 complete sample records equal. Investigation found that all physical channels matched and only the derived `time=(tick+1)/60.0` label differed in103/120 records per case: the GameCar PC24 context rounded that logging expression. The owned trajectory observer now computes/formats the sample under a temporary PC53 guard and restores the previous precision afterward. No physics computation was moved under the guard.

Before/after captures prove720 SDK-control states and480 actual-method states physically unchanged. The sample times now match the saved schema too. The initial captures/source/comparison remain in before-clock-format-guard; they are a logging diagnosis, not a rejected physics implementation or a trajectory fit.

## Reproducibility

build-native.py and build-trajectory.py use the task-owned extracted compiler and Windows SDK from V188, with the original PhysX development files already present locally. They delete prior owned executables/receipts before compilation and check fresh identities. Never run the historical Parallels build scripts. Windows SDK/CRT/Game DLLs/PDBs/headers/libraries and Windows executables remain local.

build-source.py links the actual current GameCar/Engine/Jolt libraries and records their hashes. run-source.py rejects stale source/binary/library identities. The Mac diagnostic archive contains two self-contained arm64 observers, exact profiles, inputs, numerical evidence, a verifier and licenses. Its expected530 matrix is explicitly labeled compatibility data; its240 saved SDK states are the native Windows reference. The shipped copies are stripped of local symbols and ad-hoc signed; original build binaries remain untouched.

The full goal remains open: kinetic-energy/public-mass audit, real front/rear/all/no-wheel contact transitions, timer transitions during continuous motion, complete original fixed step, shifted COM/edge orientations, ordinary isolated races, real AI/multibody collisions and visual/menu/loading comparisons still need appropriate evidence.
