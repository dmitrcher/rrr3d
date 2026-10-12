# v49: trace and reproduce original angular integration

The v48 mass arithmetic is retained. A new trace follows the original CPU
rotation integrator and separates physical rotation from public API output.
There are no changes to engine power, braking, grip, damping or fixture inputs.
All new runtime switches remain off by default; this is an investigation.

The original NxActor quaternion getter returns the stored physical orientation.
A two-repeat read-only capture observes 120 quaternion calls and returns without
changing the original physical histories. Hardware write watches then locate
actual rotation integration at PhysXCore RVA 0x7f1d0, rather than assuming that
an API matrix/quaternion conversion caused the difference.

The original computes angular length before multiplying by dt, uses scalar
SSE float products/sums, divides the x87 FSIN result before its final float store,
and combines cross, dot and quaternion terms in a specific order. It does not
normalize after each integration step. Jolt's existing integration computes the
length after multiplying by dt and normalizes the result. Angular velocity
already matches at all 2040 samples across the 17 stock mixed-spin fixtures.

The portable reconstruction matches 2400 captured intermediate/output float
fields across 120 free-spin and 180 coupled sliding steps, in O0 and O2. Native
traces repeat twice and preserve all underlying original physical states. The
observed x87 control word is 0x037f (64-bit significand). Host double sqrt/sin/cos
reproduce these final float results; arbitrary transcendental boundary cases,
CCD/special angular limit branches and very long rotation drift are not certified.

`RRR3D_EXPERIMENT_NATIVE_ANGULAR_INTEGRATION=1` enables this rotation step only
at dynamic-body velocity integration. Position-constraint rotation corrections
and kinematic integration keep their existing implementation.

`RRR3D_EXPERIMENT_NATIVE_PUBLIC_ANGULAR_MOMENTUM=1` reconstructs the original
public quaternion-to-matrix decoder, global inertia tensor and momentum dot
orders. It only applies when the source local inverse tensor is diagonal; no
cross terms are dropped for other tensors. Its independent C++ model matches
46620 actual original fields in 2220 samples across 18 fixtures, in O0 and O2.
The original public tensor arithmetic is distinct from solver inertia. The
previous principal-axis Jolt momentum shortcut cannot reproduce this accessor.

`RRR3D_EXPERIMENT_NATIVE_DYNAMIC_ACTOR_OUTPUT=1` extends the already reconstructed
original quaternion COM-to-actor translation to free dynamic output. The old
frozen-only output gate remains unchanged.

Two fresh repeats with all three switches and matched neutral controller produce:

- All 17 stock spin orientations: exact on all 120 steps each.
- Full float32 physical state: exact for 16/17 spin fixtures.
- Expanded 24-case suite: 22/24 exact, retaining all six inverted frozen cases.
- Previously exact 72 tilted, six old offset and 189 isolated-force cases: retained
  in two fresh repeats, with all new switches disabled and enabled.
- Default-off new binary: the same physical histories as the actual v48 captures.
- Full Debug game builds and signs with 1200 assets and 14 movies; 53/53 CTest pass.

Strict decimal JSON comparison still reports 18 residual cases. For 46 spin
samples the remaining decimal discrepancy is only a printf/iostream rounding tie
(e.g. a float is printed as 1577.07813 versus 1577.07812). Both nine-digit strings
round-trip to exactly the same IEEE-754 word. `compare-float32.py` separately
compares all 32 bits, with no tolerance; the legacy strict comparator is unchanged.
Time, commands and non-float protocol fields are not quantized.

Two physical residual cases remain. DirtDevil's local and global COM X is
0.10000000894069672 on Mac versus the original 0.10000000149011612. The source
construction computes (target COM - shape position) + shape position in float,
which cannot recover the requested target in this case. This new lead is backed
by two preserved original integrator traces and two Mac raw body traces; it is
not fixed in v49. Coupled body-slide-x still differs after its first contact,
including angular velocity, even though this integrator independently matches
actual native sliding-step inputs and outputs. That locates its remaining cause
before integration, in contact/solver state.

This investigation does not establish wheel/suspension/full-game parity. The
older 608-case residual count is historical and has not been recaptured here.
No new GUI race result is claimed. Do not promote these switches globally yet.
The goal to eliminate all reproducible discrepancies remains active.

Apply `native-angular-v49.patch` after the v48 experimental adapter and v37 SDK
chain. Patch paths beginning `sdk/` refer to the configured Jolt source tree.
Before/after hashes and complete modified files are included in `base.json`.
From the extracted investigation archive, run:

```sh
python3 source/tools/calibration/experimental/native-angular-v49/verify.py --package .
```

This verifies archive hashes, original preservation, repeats, actual arithmetic,
retained parity, exact source patch application and the known residuals. Passing
validates the investigation and does not mark the overall goal complete.

A fresh replay of the bundled binary can be run separately:

```sh
python3 source/tools/calibration/experimental/native-angular-v49/replay.py --package . --out replay
```

It checks two runs of each control/candidate scope against the actual saved
histories. Only the absolute profile provenance path is relocated; the profile
contents and all other metadata/physical state must still match.
