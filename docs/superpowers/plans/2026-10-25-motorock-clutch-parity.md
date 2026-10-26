# MotorRock clutch parity implementation plan

> **For agentic workers:** Use executing-plans inline; preserve the established Source/build workspace and guarded snapshots.

**Goal:** Apply the original Windows local-Z clutch request at the same stabilization boundary in the Mac game, using actual actor mass and one-shot consumption.
**Architecture:** Continue the already authorized V185 STATUS-RU plan. Real GameCar produces the clutch request, Session forwards it, and the stabilizer consumes it after local momentum conversion. Remove the earlier immediate oil-contact momentum mutation only under the experimental V186 flag.
**Tech Stack:** C++20, Jolt/PhysX reference, Python numerical drivers, archived Windows reference captures (VM must remain stopped).
**Spec:** tools/calibration/experimental/controller-stabilize-v185/STATUS-RU.md and the active user parity objective.

## Global Constraints
- User constraint 2026-10-26: do not start the virtual machine. Use existing immutable Windows recordings; mark new Windows observations unavailable rather than infer parity.
- Windows remains immutable reference; do not tune coefficients to fit results.
- No AI/other cars in isolated tests; identical float32 body inputs, dt=1/60, deterministic repeats.
- Native observations must come from original game DLL and real SDK actor; hash source, executable and original modules.
- Experimental flags default off. Keep the PC53 SDK-fixture boundary distinct from marked PC24 GameCar commands.
- Preserve the golden capture and unique historical data. No historical cleanup authorized by the storage question.
- Existing user-authorized Source/build and research branch remain in use; avoid a second large checkout.

## Review Focus
- Positive/negative/zero strength and one-shot consumption: Task 1 world matrix, Task 2 sequential original calls.
- Nominal versus public actor mass: Task 1 independent native bits, no coefficient fitting.
- Expiry/cancellation/immunity: Task 2 real GameCar command behavior, matched original observations where available.
- Oil event must defer force to the fixed step: Task 2 actual Session test or explicit incomplete evidence.
- Unmarked standalone SDK commands keep their arithmetic: Task 3 baseline regression.

### Task 1: Reproduce and integrate the real controller request
**Files:** work/controller-clutch-v186/source-world-probe.cpp, run-world.py; six guarded Source files in overlay.json.
**Interfaces:** consumes V185 VehicleDriveCommand::originalGameCarStabilization; produces clutchLocked bool and clutchStrength float through GameCar/Session/engine.
- [x] Write a real GameCar + Jolt-world driver against recorded original StabilizeForce/SDK states, without applying the pending overlay.
- [x] Run baseline: expect numerical failure for nonzero clutch despite exact incoming q/inverseI/omega/momentum.
- [x] Review/apply the six-file guarded overlay, rebuild all header consumers, and rerun the same test.
- [x] Expect 720 no-contact cases, two identical repeats; clamped rotation and omega before integration and complete body pose/momentum after integration match Windows.
- [ ] Commit accepted Source snapshots and evidence after review; do not claim timer or Session event acceptance from this matrix alone.

### Task 2: Verify request lifetime and Session timing
**Files:** work/controller-clutch-v186 lifetime/reference drivers and session verification; Source consumers if evidence requires a correction.
**Interfaces:** Task 1 fields, GameCar::LockClutch/CancelClutch/ConsumeClutchStrength and Session oil-contact application.
- [ ] Observe original repeated stabilization calls and compare the evolving Source world, proving strength is applied once.
- [ ] Exercise expiry, immunity, cancellation and the actual Session oil path; numerical equality alone does not prove event timing.
- [ ] Fix only independently reproduced errors and preserve each failing control.
- [ ] Record verified versus still-open boundaries explicitly.

### Task 3: Review, regression and synchronization
**Files:** V186 reports/README/NEXT, research repository snapshots and release receipts where applicable.
**Interfaces:** accepted Task 1/2 behavior and unchanged V185/SDK boundary.
- [x] Run 53 CTest, 608 baseline scenarios twice in independent bounded processes, and raw 7200 map-state regression.
- [ ] Review the complete change; resolve important findings with a failing reproducer before the fix.
- [ ] Sync accepted code/evidence to GitHub; verify remote HEAD. Publish a diagnostic only if it is independently runnable and honestly scoped.
- [ ] Leave the full parity goal active: contact gates, jump, full original update and ordinary race/application acceptance remain required.

Checkpoint 2026-10-26: Task 1 numerical integration passes. Task 2 has Source lifecycle and actual Session oil coverage; native uninterrupted sequence/timer remains open under the no-VM constraint. Full physics goal remains active. See V186 README/NEXT and acceptance evidence.
