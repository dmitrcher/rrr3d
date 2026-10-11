# Actual Mac Native contact arithmetic v32

This is an **unaccepted experimental prototype**, layered after v30
`row-jacobians.patch`. The six-file before/after SHA contract and patch are in
`../native-contact-arithmetic-v32-base.json` and
`../native-contact-arithmetic-v32.patch`. `NativeContactArithmetic.h` is an exact
copy of the SDK helper used by the compiled ARM64 Mac probe.

The ordinary game defaults to the existing solver. This experiment only handles
source block-PGS normal rows for a dynamic/static pair with zero friction and
zero restitution. Each flag must be the literal string `1`:

| Bit | Flag suffix after `RRR3D_EXPERIMENT_NATIVE_CONTACT_` |
|---:|---|
| 1 | `MASS` |
| 2 | `BIAS` |
| 4 | `POINT_VELOCITY` |
| 8 | `ORIGINAL_DELTA` |
| 16 | `FORCE_FIRST` |

`capture.py` explicitly selects the fourteen preceding source experiments,
`windows-source-v14.cfg`, seed42, and the requested mask. No AI, weapons,
friction, suspension, engine or wheel contacts participate in these twelve
isolated body/plane cases. All cases run120 physical steps at1/60s.

Three modes separate interventions:

- `ordinary`: Mac computes its own geometry and errors.
- `points`: explicitly replay recorded Native positions/separations for one
  first-contact step. Mac still computes the constraint error.
- `errors`: additionally replay the recorded Native constraint error for that
  step. This is diagnostic, not an independent geometry/error implementation.

The contracts select the **first observed contact**, including six speculative
contacts and six penetrating contacts. The low-height ticks are2 (v0) and1 (v1);
older impact-only replay contracts used3/2 and must not be substituted here.
`RRR3D_CONTACT_NATIVE_TRACE_STEP` and `RRR3D_CONTACT_NATIVE_FIRST_ONLY=1` only
limit telemetry. They do not stop simulation. The `scaled_bias` field is the
Native candidate, **not the applied legacy bias when bit2 is off**.

## Reproduce the published evidence

Extract the v32 audit archive, then run from its root:

```sh
python3 source/tools/calibration/experimental/native-contact-v32/verify.py --measurements measurements --contracts contracts/contracts.json
python3 source/tools/calibration/experimental/native-contact-v32/factorial.py --measurements measurements --contracts contracts/contracts.json
python3 source/tools/calibration/experimental/native-contact-v32/isolation.py --measurements measurements
python3 source/tools/calibration/experimental/native-contact-v32/verify-patch.py --snapshots snapshots
```

These use only Python's standard library and the included `compare.py`. They
check raw Native solver/body words, actual Mac entries, prepared mass/bias,
repeat equality and complete120-step physical states. Full-mask diagnostic
results are480/480 entry states and12/12 first-step positions/velocities exact.
All32 masks ×12 cases ×2 repeats are included; only mask31 satisfies all480
entry-state comparisons. Every ordinary120-step contact remains discrepant.

To capture fresh Mac evidence, use an empty destination:

```sh
chmod +x executables/macos/rrr3d_physics_spring_probe
python3 source/tools/calibration/experimental/native-contact-v32/capture.py --probe executables/macos/rrr3d_physics_spring_probe --measurements fresh --contracts contracts/contracts.json --mask 31 --mode errors --repeat 0
python3 source/tools/calibration/experimental/native-contact-v32/capture.py --probe executables/macos/rrr3d_physics_spring_probe --measurements fresh --contracts contracts/contracts.json --mask 31 --mode errors --repeat 1
```

The packaged executable uses system macOS frameworks only. The immutable Native
captures and Core/probe SHA identities come from the v31 audit. Fresh Native
capture uses `../NativeSolverTrace/capture.py` and the existing real Parallels
runtime; this Mac helper does not synthesize a Windows baseline. Capturing
`ordinary --mask 0` also requires the included v30 Mac baseline in the destination.

The archive includes the six exact before/after files for checking the layered
patch. It is not a complete vendor SDK checkout. Full game installation was not
updated by this experiment. Build and53 CTest results are fresh, but standalone
contact parity, tilted frames, dynamic/dynamic pairs, friction, restitution,
suspension, long-running races and AI are still outside the accepted result.
