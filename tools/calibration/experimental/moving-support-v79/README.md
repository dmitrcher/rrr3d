# V79: moving wheel-support contact creation order

This research checkpoint changes Mac physics and fixes the previously proven
first divergence at physical sample 28 in both stock-pole cases. Windows remains
the immutable PhysX 2.8.4 reference. No engine power, mass, spring, tire or material
parameter is fitted, and the running Mac never reads Windows capture data.

The new `RRR3D_EXPERIMENT_NATIVE_CONTACT_ORDER_V79=1` switch is off by default.
It requires the exact V76 investigation profile in `candidate-environment.json`.
The generic model computes source vehicle-box and wheel-line bounds, retains
sweep-and-prune endpoint order, and assigns contact creation ranks through
four-slot adjacency nodes and ascending created-pair bits. Initialization uses
the separately proven original unstable endpoint sort from V78. The Jolt wheel
constraint uses these ranks to create new spring/stop rows. Existing descending
constraint-ID solving remains unchanged. There is no fixture-name dispatch or
hardcoded wheel permutation in the implementation.

## Verified change

* At physical sample 28, every reported float32 physical observable now equals
  Windows in both stock-pole cases. The first remaining divergence is sample 29.
* Maximum angular-momentum error over each 300-sample history decreased from
  0.3823050476 to 0.0002607545 and from 0.3853166879 to 0.0002523085.
* Fresh full 608-case captures, 255864 states per run, repeat exactly. All 116
  previously strict cases remain strict; 492 still differ. Only two complete
  trajectories changed versus V76; 606 are unchanged. Seven complete-trajectory
  maximum-error metrics have no regressions.
* 291 independent reference gates pass in fresh control and candidate captures,
  each repeated twice. Flag-off focused captures preserve V76.
* All 53 CTest tests pass after rebuilding the full signed arm64 game. A solo
  real-track headless launch and a separate rendered six-car launch exit cleanly.
  The latter is a functional lifecycle check, not a Windows parity comparison.

## Limits and next divergence

This is not a complete reproduction of Native broadphase lifecycle behavior.
Only current vehicle boxes and wheel suspension-line bounds are represented;
static meshes and decorations do not allocate ranks in this model. Node deletion,
empty-node recycling, arbitrary actor insertion/removal, mixed static/dynamic
wheel-row creation and tire-row traversal require further source validation.
The unchanged 608-case matrix does not prove those untested states.

The first remaining sample29 difference is in row preparation. With identical
body pose, Native uses a stored local contact point whose X/Y words equal the
source wheel attachment, while Mac currently constructs rows from a world hit
point. A blind world-to-local-to-world roundtrip of the current Mac point does
not reproduce the Native Jacobians. Using Native's recorded local point is only
an offline diagnostic and reproduces all four Jacobians; it is not installed.
Recovering the original world hit calculation and local conversion is the next
task. The local Z or captured values must not be fitted or injected into physics.

## Reproduce

`apply-experiment.py --adapter-root PATH --sdk-root PATH` checks every source hash
before any mutation, applies five files over the exact V76 overlay chain, and
supports `--restore`. Rebuild afterwards. The repository's root release sources
are a different base. The before/after snapshots do not include the complete
prior Jolt overlay chain.

`run-probe.py --probe PATH --out NEW_PATH` launches the frozen research profile
and refuses to overwrite captures. The profile's config path is resolved locally.
`verify-checkpoint.py` rechecks retained compressed evidence and SHA256 files;
adding `--probe PATH` reruns all four focused cases from any working directory
and requires the full output to match the verified candidate.

`workspace-reproduction` scripts retain the original workspace paths and are
audit records, not portable launchers. All status reports explicitly retain
`goal_complete=false`. This checkpoint is not a new default game release.
