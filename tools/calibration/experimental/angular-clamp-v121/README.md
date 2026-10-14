# Original angular clamp arithmetic V121

Pending final matrix/package acceptance. Mac first diverged before the first contact at step62 of the airborne border fixture. Actual incoming velocities at angular force-preparation clamp are identical. Windows stores the XYZ squared norm and limit-squared/norm ratio as float, then keeps sqrt and each component product wide until the final float stores. Jolt rounds the common scaling factor before multiplying components.

The default-off `RRR3D_EXPERIMENT_NATIVE_ANGULAR_CLAMP_V121` branch reproduces this original arithmetic in source axes. Configured angular limits, damping and force-preparation timing remain unchanged; linear clamping retains its previous behavior. No coefficients, reference-state injection or case/frame/wheel rules are used.

Four actual immutable Native clamp records (inactive/active, two repeats) match the production helper at O0/O2. The actual stored sqrt arguments are independently derived from measured inputs. Source has identical nonzero incoming words and the measured old outputs show the rounding difference. Private signed-zero differences are explicit. Four live candidate captures match all2400 Native states and160 actual contact incoming bodies/tensors/levers/prepared masses/biases/retained impulses. Windows executable/DLL hashes and complete histories remain unchanged.

Apply/restore with `apply-experiment.py --adapter-root PATH --sdk-root PATH [--restore]`, then rebuild all consumers. Every source identity is checked before any mutation. `verify.py --probe PATH` checks actual original helper records and fresh complete trajectories. Read-only telemetry snapshots and host reproduction scripts are verification materials, not additional runtime overlays.

Whole-game and render parity remain unproven. This investigation package does not replace the ordinary installation.

Complete matrix:604/608 strict,4 open; all603 prior strict cases preserved. Both255,864-state repeats are identical;607 full histories unchanged. The entire600-step border-air-straight trajectory newly matches Windows. No raw/canonical maximum worsens. All53 project checks pass under both profiles;291 independent gates and solo/rendered functional checks pass. Portable verification covers10,380 fresh Native-equivalent states and600 flag-off V120-preserved states.
