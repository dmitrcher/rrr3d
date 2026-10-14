# Original steering compatibility V118

Validated investigation checkpoint; overall goal remains incomplete. On the first nonzero steering command, Mac previously switched suspension and tire preparation to generic Jolt math, reused old lateral impulses and changed tire row order. Windows retains its actor-frame preparation, starts with zero tire impulses, rotates the projected basis using float-stored sine/cosine, and traverses persistent tire rows lateral before longitudinal.

The default-off flag `RRR3D_EXPERIMENT_NATIVE_STEERING_COMPAT_V118` restores those original paths for turned wheels. No coefficients, reference states or case/frame/wheel rules are used. Suspension and tire preparation/solve use consistent axes. Original zero-angle paths remain available with the flag disabled.

Actual original records establish each correction. Sixteen frame results and eight steered basis/sincos records match production arithmetic at O0/O2. Original handle slots bind the prepared rows; eight tire masses/Jacobians/inverse-Jacobians and eighty before/after solve checks per capture match actual original row order, limits, nonzero impulses and body words on both left/right steering. Private signed zeros are explicit. Eight complete 600-step Source captures match original histories. Windows executable, DLL and reference captures remain unchanged.

Apply with `apply-experiment.py --adapter-root PATH --sdk-root PATH`; `--restore` restores recorded V117 sources. Rebuild consumers. `verify.py --probe PATH` checks original arithmetic and fresh original-equivalent histories. Read-only dependency headers under evidence are for portable arithmetic verification, not additional runtime overlays. Whole-game and render parity remain unproven; ordinary installation unchanged.

Complete matrix: 601/608 strict, 7 open; all 599 previous strict cases retained. Two complete 600-step steering histories newly match Windows; 606 histories unchanged. Both 255,864-state repeats match exactly. No audited maximum error worsens. All 53 project checks pass under both profiles; 291 independent gates and solo/rendered functional checks pass.
