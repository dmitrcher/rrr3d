# Spring split v53 — experimental source checkpoint

This default-off experiment follows the v51 SDK chain and the v52 wheel ray geometry adapter. It is not a claim of complete vehicle parity. `base.json` pins every input/output source file. Ordinary repository physics source is not replaced by the snapshots.

`RRR3D_EXPERIMENT_NATIVE_SPRING_SPLIT_V53=1` enables two accumulated spring impulses and the original final-pass ratio. `RRR3D_EXPERIMENT_NATIVE_SPRING_PREP_V53=1` additionally enables the recorded preparation arithmetic and J*v operation order. They require the source spring-final/bias/cold/raw configuration. The verified candidate also enables both v52 ray flags and the v51 environment recorded in the capture manifests.

Run `python3 verify-math.py` to verify the original recorded scalar operations at O0/O2. `apply-experiment.py --sdk-root <Jolt directory> --adapter-root <game source>` installs only when all recorded before hashes match. `--restore` restores the exact before images. A rebuild is required. Jolt generic constraint layouts remain unchanged; split impulse state lives in each wheel. Optional `RRR3D_NATIVE_SPRING_V53_TRACE=<file>` appends up to 256 diagnostic rows per process.

Three complete first-contact states now match the original Windows probe, including rotation and angular momentum. Four prepared spring rows and 20 actual Mac solve steps agree with original Windows internal records. One signed-zero difference remains in an internal Jacobian component; the report lists it explicitly. The next step still has different wheel contact geometry.

Fresh broad runs: 608 cases, 255,864 states per run, two repeatable Mac runs. All prior 65 exact full cases remain exact. First divergence is later in 358 cases, earlier in none; 543 full cases remain open. Retained separate gates: 291 cases with two control/candidate repetitions. Full Debug build and 53 CTests pass.

Preparation has been directly proved for the first four flat-ground spring rows, not for every spring configuration. Ground/wheel skin arithmetic uses the original fixture's 0.025 + 0.025. Candidate ray selection remains Jolt's; v52 origin handling is still limited to identity actor rotation. Dynamic contacted bodies, complete gameplay, renderer parity and editor behavior are not certified by this checkpoint.
