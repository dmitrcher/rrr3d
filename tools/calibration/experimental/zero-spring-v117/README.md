# Original zero-parameter spring activity V117

Validated investigation checkpoint; overall goal remains incomplete. Windows retains suspension rows when travel is configured and both stiffness and damping are zero. Its original preparation clamps damping to a minimum value. Mac disabled these rows before its compatible preparation could run. This skipped tiny but reproducible suspension effects.

The original minimum is already present in NativeSpringSplitV53. Its use is shared with initial Jolt construction to avoid zero/zero before the original preparation replaces the coefficients. The experiment `RRR3D_EXPERIMENT_NATIVE_ZERO_SPRING_V117` is off by default and requires original split/preparation modes. Zero travel remains inactive. The effective mass is computed from the actual body and Jacobian; its observed one-word variation is retained.

Actual original handle slots bind wheel identity. Prepared masses and all nonzero Jacobian/inverse-Jacobian words match Source. Forty before/after solve checks per repeat match actual impulses and body velocity words. One private signed-zero Jacobian difference is reported explicitly. Original executable, DLL and public histories remain unchanged. No parameters are fitted and no reference states enter runtime.

Apply with `apply-experiment.py --adapter-root PATH --sdk-root PATH`; `--restore` restores the recorded V116 SDK base. Rebuild consumers. `verify.py --probe PATH` checks actual original preparations at O0/O2 and fresh original-equivalent histories. Whole-game and render parity remain unproven; this package does not replace the ordinary installation.

Complete matrix: 599/608 strict, 9 open; all 596 previous strict cases retained. Three complete 180-step histories newly match Windows; 605 histories are unchanged. Both 255,864-state repeats match exactly. No audited maximum error worsens. All 53 project checks pass under both profiles; 291 independent gates and solo/rendered functional checks pass.
