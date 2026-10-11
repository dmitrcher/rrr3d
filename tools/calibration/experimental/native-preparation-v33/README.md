# Native contact preparation and derived Mac error v33

This audit removes **recorded Native constraint error** from the v32 Mac
first-contact diagnostic. Mac computes the error from its own separation/skin
and generated raw/material quaternions. Recorded geometry is still needed for
full first-row parity; ordinary contact generation remains unaccepted.

The strict literal`1` flag `RRR3D_EXPERIMENT_NATIVE_CONTACT_ERROR_FRAME` enables
the new error transform. It requires the existing source-mesh and
`MATERIAL_FRAME_CANDIDATE` experiments. Applied normal rows retain v32's
zero-friction/restitution dynamic/static source block-PGS gate. The flag is off
by default. Derived input fields are separate from explicit diagnostic error
replay; diagnostic replay has priority. `derived_error:true` and
`diagnostic_error:false` distinguish independently calculated errors in trace.

The six-file patch in `../derived-contact-error-v33.patch` layers **after v32**.
`../derived-contact-error-v33-base.json` records exact before/after SHA values.
The normal error helper is `../DerivedContactErrorFrame.h`. Arbitrary tilted
raw seed construction is still a candidate; these checks cover twelve first
identity-body/flat-mesh contacts, six speculative and six penetrating.

## Native observations

The Windows tracer observes actual preparation in the unchanged v30 probe and
PhysX2.8.4 Core, in a real Parallels Windows11 VM. SHA identities:

```text
PhysXCore.dll  c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb
SpringProbe   8c331b7ea35ed6181c0f8dd52cddca451f60c817d381d360cb53852e8c49ed9d
TraceContact  20eab6161b13353e383d89b3573e56ad2553f9c8d4fe116343875e420993b25a
```

Temporary INT3 instrumentation reads process memory and x86 register state;
no numerical solver inputs are replaced. Hardware watch is disabled in this
tracer. All120 physical states match the uninstrumented v30 baseline in all
24 Native runs. Full Native raw trace is not claimed byte/semantic identical:
heap addresses, padding and undefined fields vary. The defined first-eight
preparation values and modeled results repeat exactly.

Observed material path: `203900` → global error rotation → `20aad0` material
frame adjustment → `ac00` matrix-to-quaternion → inverse error rotation →
`75490` setter called from return RVA`2042ef`, then user dispatcher`207440`.
Converter`74070` copies raw error unchanged at`7467b/74684`; the material path
subsequently replaces it. `stack_return_rva` is the first stack DWORD expressed
relative to Core; **only at a function-entry breakpoint is it a return address**.
Mid-function uses are stack-top candidates, not call-chain proof.

Raw100 quaternion starts at **byte0x48, float words18..21**. Reading19..22
shifts the quaternion by one word. Input136 quaternion is words3..6. Prepared
solver128 quaternion remains words16..19. `material-inputs-ready` records its
stack before the20-byte argument cleanup; its first-frame quaternion is
words43..46. `material-quaternion-adjusted` records the material quaternion at
words9..12 of its stack. Native `material-error-ready` is before tangent masking;
flag0x20 clears tangential error before the setter. The Mac normal helper uses
only the first component.

## Reproduce the archive

From the extracted archive root, make executables executable if the extractor
has not preserved their mode, then run:

```sh
chmod +x executables/macos/rrr3d_physics_spring_probe executables/macos/DerivedContactErrorFrameProbe
python3 source/tools/calibration/experimental/native-preparation-v33/verify.py --measurements measurements --frame-probe executables/macos/DerivedContactErrorFrameProbe
python3 source/tools/calibration/experimental/native-preparation-v33/verify-contacts.py --measurements measurements --contracts contracts/contracts.json
python3 source/tools/calibration/experimental/native-preparation-v33/verify-isolation.py --measurements measurements
python3 source/tools/calibration/experimental/native-preparation-v33/verify-patch.py --snapshots snapshots
```

`verify.py` passes only raw scalar error to the compiled ARM64 helper. The helper
generates both quaternions from the identity actor/+Z fixture, without receiving
recorded quaternions or expected outputs. Generated quaternions, intermediate
errors and scalar setter values match96/96 observations in both repeats.
The raw Native scalar is a recorded input in this standalone arithmetic check.

In the actual Mac `points` mode, the scalar is independently computed from
replayed separation + Mac skin, then transformed; no recorded error is read.
All480 entry states and96 prepared masses/biases/errors match Native, and all12
first-step positions/velocities match. In `ordinary`, no geometry/error is
replayed: first-contact errors already match96/96, but all12 complete120-step
contacts remain discrepant. Four clipped XY points in m0h0v0 still differ.

For fresh Mac captures, use an empty destination:

```sh
python3 source/tools/calibration/experimental/native-preparation-v33/capture-mac.py --probe executables/macos/rrr3d_physics_spring_probe --measurements fresh-mac --contracts contracts/contracts.json --mode points
```

Modes `ordinary`, `errors`, `control` are also available; default is two repeats,
`--repeat 0` or1 selects one. `control` requires the included v32 ordinary-mask0
baseline in its measurement directory. Native captures require the v30 Windows
baseline in the destination and the existing guest runtime/VC90 CRT:

```sh
python3 source/tools/calibration/experimental/native-preparation-v33/capture-windows.py --measurements fresh-windows --tracer-sha256 20eab6161b13353e383d89b3573e56ad2553f9c8d4fe116343875e420993b25a
```

The wrapper checks Core, probe and tracer SHA before instrumentation. `--case`
selects one fixture. Build the tracer with included `build.cmd` in
`C:\MotorRock\calibration-probes\contact-parent-v30\preparation-v33`; copy the
existing runtime CRT alongside it. The C++ tracer itself identifies DLL by name;
never use it directly against an unverified DLL/version. Native CLI paths are
intended to contain no spaces/cmd metacharacters. Mac frame helper can be rebuilt
with `clang++ -std=c++17 -O2 -Isource/tools/calibration/experimental` and the
included `frame-probe.cpp`; its only dependencies are system C++/C runtimes.

No installed games or canonical production adapter were updated. Full Mac Debug
build and53 CTest passed fresh;165 early isolation cases repeated exactly.
Independent geometry, later contacts, tilted frames, friction, restitution,
dynamic pairs, suspension, long races and AI remain outside accepted parity.
