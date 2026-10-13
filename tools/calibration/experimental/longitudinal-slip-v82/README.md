# V82: source longitudinal slip and Jolt force sign conversion

Original PhysX reports forward minus radius*axle; the prior adapter reported the opposite sign. V81 proved the Source arithmetic on40 independent Native inputs/120 float32 output words at O0/O2. V82 installs that arithmetic behind RRR3D_EXPERIMENT_NATIVE_LONGITUDINAL_SLIP_V82 (default off), and explicitly converts the normalized sign back for Jolt prescribed tire impulses. Original flag bit2 selects raw velocity slip for reporting and friction. The40 measured Native fixtures cover normalized reporting; the flag4 branch is recovered from original disassembly but is not separately certified by those captures.

Full608-case matrix, two fresh runs of255864 states each, repeats exact.119 cases strictly match original Windows float32 observables,489 remain open. All117 V80 exact cases retained; oil-slide and tire-overload become exact. Moving support pole1 first mismatch shifts29->32. All recorded quantities other than engine_longitudinal_slip remain exactly unchanged in every case, including positions, velocities, angular momentum, suspension and tire impulses. There is no motor/mass/material fitting or live oracle injection.

Slip words disagree280871->247748.380 cases change slip output,228 complete histories remain unchanged.240 slip maxima improve and44 worsen on already divergent input states; Source subtraction removes accidental cancellation of other input errors. These44 regressions are retained explicitly in the report and are still open. This is not universal slip equivalence or a claim that all slip errors improve. Seven body/suspension/contact maximum-error metrics remain unchanged in all608 cases.

All291 independent control/candidate gates repeated twice pass.53 CTests pass; full game build and separate solo/no-AI realtrack and rendered6car functional launches pass. These are isolated engine proofs and game smoke checks, not full Windows/Mac rendered-game parity. Default installed game profile remainsV73. This checkpoint contains numerical tools, not a game installer.

Use Python3 and macOS ARM64; mathematical proof additionally requires Xcode Command Line Tools:

```sh
python3 verify-checkpoint.py --probe /absolute/path/to/rrr3d_physics_calibration
python3 proofs/verify-math.py
python3 run-probe.py --probe /absolute/path/to/rrr3d_physics_calibration --out /new/path/result.jsonl --cases all
```

Guarded apply-experiment.py expects the exact V80 research overlay chain and refuses an unrelated base before mutation. run-probe.py relocates the retained unchanged Windows profile and never overwrites captures. workspace-reproduction scripts retain workspace paths and serve as audit records. proofs/native-captures contain immutable Original debugger traces; original EXE/DLL hashes and all4800 original samples were preserved. Native addresses are runtime-specific and never used by live Mac physics.

Next priority is premature Mac sleep/deactivation and stale velocity restoration in sleep-settle; observations and hypotheses are in workspace-reproduction/next-work.md. Overall goal remains incomplete.
