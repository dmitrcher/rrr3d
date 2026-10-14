# Separate suspension force conversion V115

Validated investigation checkpoint; overall goal remains incomplete. The original Windows wheel update first stores the spring impulse times the rounded inverse step, then adds the unrounded stop product and stores the result. Summing impulses first changes force by one float32 bit in the observed cases. The Mac adapter now reads both rows separately and reproduces the original store order. The experimental flag `RRR3D_EXPERIMENT_NATIVE_SUSPENSION_FORCE_STORE_V115` is off by default and requires V88 force storage and the PhysX compatibility profile.

Twelve original stores from three cases match the production helper at O0/O2. Source solver inputs match the original bit for bit; original callback addresses establish wheel identity. Read-only observers preserve every state of ten complete Windows histories and ten Mac histories. The Windows executable and PhysX DLL remain unchanged. No coefficients or recorded states are used by the game.

Apply with `apply-experiment.py --adapter-root PATH --sdk-root PATH`, then rebuild. `--restore` restores the recorded V114 base. All files are checked before any mutation. `verify.py --probe PATH` checks arithmetic and fresh isolated histories against the packaged immutable reference excerpts. Whole-game physics and render equality remain unproven; this package does not replace the ordinary installation.

Complete matrix: 594/608 strict, 14 open; all 591 previous strict cases retained. Three new complete 600-step histories match Windows; 605 histories unchanged. Both 255,864-state repeats match exactly. No audited maximum error worsens. All 53 project checks pass under both profiles; 291 independent gates and solo/rendered functional checks pass.
