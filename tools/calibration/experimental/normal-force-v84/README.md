# V84: force-domain wheel callback

The old Mac callback capped suspension impulses and divided them back into forces. Native Windows caps the original force before force/projection/dt tire scaling. At high mass this produced 14999.999 instead of 15000 and changed lateral bounds on physical20. RRR3D_EXPERIMENT_NATIVE_FORCE_DOMAIN_V84 preserves the force through the callback, report and tire limit, without altering mass, power, tire coefficients or suspension tuning. All switches are off by default.

Original callback EXE RVA0x3730 uses x87 control895 (extended precision) and stores load after the product. A provisional per-operation float32 helper failed the independent slope measurement: 5970.2236328125 versus Source5970.22314453125. It was rejected before publication. Final binary64 intermediate products reproduce all measured integer-mass/gravity loads. Arbitrary unmeasured float inputs are not certified as universal x87 equivalence. Source probe release compares force>load*tireSpring; original GameCar compares force/load>tireSpring. Untested exact threshold boundary equivalence remains open.

20 immutable original Windows captures preserve all12000 full Golden states, original EXE/DLL hashes unchanged.48 callbacks cover high-load flat ground, stock settle, two slopes, clutch/oil and tire overload;24 before/after tire-force scaling records add72 word checks. Actual C++ helper at O0/O2 matches168 raw words. Reconstructed old impulse clamp fails8 captured high-load callbacks. No measured Windows data is injected into the live Mac simulation.

608 scenarios, two255864-state repeats, retain all119 exact cases;489 remain open and no new complete exact case is claimed.293 whole histories unchanged,315 changed. Full trajectories have both improvements and regressions: position maxima120 improve/101 worsen; velocity106/111; orientation120/103; suspension65/87. These are fully disclosed in evidence/physics-v84-regression-audit.json. This research checkpoint is a proved upstream calculation repair, not a globally improved or completed game parity release. All291 separate fresh control/candidate gates and53 CTests pass. Full game build and solo/no-AI and rendered race functional launches pass; these are not Windows/Mac race parity proof.

High-load physical20 now matches all observables. First remaining difference21 is inside fourth tire iteration: Source sums six mixed velocity terms; Jolt still uses two Dot sums. The measured first uncapped lateral row has identical input body, geometry, inertia, mass and bounds, but different jv rounding. See next-work.md for the next targeted correction. SDK remainsV79 and the installed normal game remains the existing baseline.

Portable checks from any directory:

```sh
python3 verify-native-force.py --check-cpp
python3 verify-checkpoint.py --probe /absolute/path/to/rrr3d_physics_calibration
python3 run-probe.py --probe /absolute/path/to/rrr3d_physics_calibration --out /new/path/capture.jsonl --cases all
```

The guarded installer expects the exact V83 overlay and validates all input files before mutating either one. Numerical ARM64 probes are standalone research tools, not a game installer. Overall goal incomplete.
