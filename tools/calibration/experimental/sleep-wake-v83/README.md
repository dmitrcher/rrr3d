# V83: original zero-force autowake and dormant motion restoration

Windows addForce with NX_ACCELERATION and autowake=true refreshes the wake counter even for a zero vector and already awake actor. The prior Mac diagnostic setter merely stored the vector and skipped zero force. Default-off RRR3D_EXPERIMENT_NATIVE_DIAGNOSTIC_AUTOWAKE_V83 restores this API side effect by resetting the existing sleep timer and activating the actor, without a synthetic force or timer/threshold fitting. Sleeping remains allowed. This is diagnostic addForce wake semantics, not a claim that every Nx setter or passive sleep rule is reproduced.

A separate default-off RRR3D_EXPERIMENT_NATIVE_DORMANT_MOTION_RESTORE_V83 prevents cached solved velocities from overwriting cleared motion after Jolt deactivation. Direct live traces prove the old bug at physical48: Jolt clears both velocities, then the adapter restores vz0.250097811 while the body remains inactive. The angular trace applies the polar coordinate mapping to Jolt angular velocity and is used only for zero/nonzero tests, not axial component parity.

Eight Original captures at47/48/49/100, twice each, prove zero vector/mode5/autowake1 and wake counter0.38333329558->0.399999976158. GetterRVA1af050 reads backend+0x19c; setterRVA1b0da0 controls awake flag at0x150. All4800 Original physical states remain Golden exact; original EXE/DLL/tracer file hashes before/after remain identical. The early raw wake_getter_rva diagnostic is EAX minus runtime module base before the getter load, not the getter target; verification derives the real target from vtable[12]. Initial malformed trace rejected and excluded. The bounded disassembly and source observer are retained.

Four isolated modes(control/wake-only/dormant-only/combined), two1200state repeats each: wake-only restores the full no-sleep-control history; dormant-only removes stale motion but does not repair early sleeping and slightly worsens angular error on its own. Combined equals wake-only. No timer, sleep threshold, power, mass or suspension coefficient changed; no live Windows values injected.

Full608-case matrix, two255864-state runs, repeats exact:119 strict cases retained,489 open.607 complete histories unchanged; only sleep-settle changes and all seven max-error metrics have no regressions. Its position max error0.029747002->1.50001417e-11 source units and velocity max0.252169102->5.40000489e-12. First remaining numeric difference26 is not removed; overall exact case count remains119. All291 independent fresh control/candidate gates pass,53CTest pass, full game build plus solo/no-AI realtrack and separate rendered6car functional runs exit0. These do not prove full rendered game parity. The44 reported slip-maxima regressions disclosed atV82 are unchanged, not hidden or repaired by this fix.

Portable Python3 verification from any cwd:

```sh
python3 verify-native-wake.py
python3 verify-checkpoint.py --probe /absolute/path/to/rrr3d_physics_calibration
python3 run-probe.py --probe /absolute/path/to/rrr3d_physics_calibration --out /new/path/capture.jsonl --cases all
```

The guarded one-file installer expects exact V82 research overlay chain; root release sources differ. Source-manifest.json covers every exported file except itself. Numerical binaries are macOS ARM64 standalone probes, not a game installer. Normal installed game profile remainsV73. Workspace scripts retain local paths and are audit records. Next priorities are actual high lateral/braking discrepancies, ranked in workspace-reproduction/next-priority.json; do not substitute a tiny already negligible moving-support case for those remaining effects.
