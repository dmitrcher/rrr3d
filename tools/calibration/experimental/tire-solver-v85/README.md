# V85 tire solver investigation

Windows PhysX remains unchanged. At identical native inputs, the mixed six-term velocity sum and preservation of the original solver delta reproduce 40 captured tire rows: 520 raw float32 words at O0/O2. Continuous live step21 checkpoints also reproduce 150 raw body words through springs and tire passes. The high-load lateral fixture now agrees through physical step23; its first difference is step24.

Full 608-case matrix, two exact repeats: **119 fully exact, 489 open**, with all previously exact cases retained. Whole-history error maxima both improve and worsen; see the compressed regression audit. This is a verified local solver correction, not certification of overall physics parity. Neither coefficient tuning nor Windows state injection is used. The two flags are independently default off. The normal installed game remains at its previous baseline.

Apply this overlay only after the verified V84 overlay chain; `apply-experiment.py` checks the exact SDK SHA before writing. Supply `--sdk-root` and `--adapter-root`; `--restore` restores the recorded SDK base. Rebuild required. `candidate-environment.json` records the full research environment; resolve its profile path relative to this directory. The older hard-stop V66/V68 flags are excluded: they alone do not remove step24 and can regress trajectories.

Run `clang++ -std=c++17 -ffp-contract=off -O0 -Iproofs proofs/verify-tire.cpp -o /tmp/tire-v85` and execute it; repeat at O2. `verify-checkpoint.py --probe PATH` executes a fresh 1200-state reproduction using a V85-built probe. Captures include native observer evidence, two Mac repeats, and immutable Windows reference subsets. Full matrix and independent 291-case control/candidate gates, 53 CTests, full build, solo game and rendered AI functional smoke reports are retained. AI smoke is not parity evidence.

Next causal lead: direct signed suspension travel plus unchanged scene slop reproduces all four native hard-stop error rates at step24; a radius coordinate roundtrip does not. That correction and remaining inertia differences are not enabled in V85.
