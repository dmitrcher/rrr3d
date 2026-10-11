# Independent Native mesh edge arithmetic v34

The actual Mac probe now independently generates all96 first contact points,
separations and constraint errors in twelve isolation fixtures. All480 observed
first solver states, masses and biases, and all12 first positions/velocities
match the unchanged Windows PhysX2.8.4 probe. No geometry/error replay or output
state reset is used in `ordinary`. All12 complete120-step contacts still differ.

`RRR3D_EXPERIMENT_NATIVE_MESH_EDGE=1` is a strict literal1, default-off candidate.
It adds observed Native plane-normal and colliding-edge normalization, explicit
float32 arithmetic ordering and the observed projection-axis selection. It
requires the previous source-mesh/material-frame/error and five Native normal
row arithmetic experiments. The two-file patch layers after v33; before/after
SHA values and complete two-file snapshots are included. This is not a complete
vendor/source checkout. Installed games and canonical production adapter were
not updated. Tilted/nonzero-friction/dynamic-pair contact parity is unaccepted.

## Actual Native observations

The debugger captures the real Parallels Windows11 reference. The dispatcher at
RVA76fe2 calls geometry at26a3a0, which calls22c8a0 and polygon contacts225c50.
The edge path normalizes plane normal at226e03 and direction at22706c before
227120 division. Breakpoints at2270b5,227120,227165,227237 and22733c observe
direction, crossing and accepted clipped points. Observed first fixture mesh
vertices are ±500; the diagonal segment delta is1000. No coordinate quantization
or expected-coordinate corrections are applied.

The first four accepted internal edges per fixture are modeled by a compiled
ARM64 helper using recorded input vertices/directions, not expected outputs.
All48 calculations match, twice. This standalone check supplies recorded
geometric inputs; the independent actual Mac proof is separate.

The raw-copy350f0 entry copies scratch that is overwritten later. Its scratch
values, padding, pointers and undefined frame fields are **excluded** from all
numerical audits. Full raw trace identity and signed-zero word identity are not
claimed. All24 instrumented Native120-step runs retain the unchanged v30 states.

Immutable runtime SHA256:

```text
PhysXCore.dll c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb
SpringProbe 8c331b7ea35ed6181c0f8dd52cddca451f60c817d381d360cb53852e8c49ed9d
EdgeTracer 867c3d05c466159cc981b18097dd84432006702ae1775e3451effbae670792da
```

## Verify an extracted archive

```sh
chmod +x executables/macos/rrr3d_physics_spring_probe executables/macos/NativeEdgeArithmeticProbe
python3 source/tools/calibration/experimental/native-edge-v34/verify-edge-model.py --measurements measurements --edge-probe executables/macos/NativeEdgeArithmeticProbe
python3 source/tools/calibration/experimental/native-edge-v34/verify-first.py --measurements measurements --contracts contracts/contracts.json
python3 source/tools/calibration/experimental/native-edge-v34/verify-isolation.py --measurements measurements
python3 source/tools/calibration/experimental/native-edge-v34/verify-patch.py --snapshots snapshots
python3 source/tools/calibration/experimental/native-edge-v34/verify-geometry64.py --measurements measurements --geometry-probe executables/macos/NativeMeshGeometryProbe
```

The Mac helper requires ARM64. Rebuild with system clang++:

```sh
clang++ -std=c++17 -O2 -ffp-contract=off -Isource/tools/calibration/experimental/native-edge-v34 source/tools/calibration/experimental/native-edge-v34/edge-probe.cpp -o NativeEdgeArithmeticProbe
```

Fresh Mac captures use an empty measurement directory:

```sh
python3 source/tools/calibration/experimental/native-edge-v34/capture-mac.py --probe executables/macos/rrr3d_physics_spring_probe --measurements fresh-mac --contracts contracts/contracts.json --mode ordinary
```

Two repeats are the default; `--repeat 0` or1 selects one. `control` disables only
the new edge experiment and requires the included v33 ordinary baseline in its
destination directory. Points/errors modes remain explicit diagnostic tools;
their captures are not the v34 independent parity evidence.

Fresh Native capture needs the included v30 baseline in its destination and
the existing Windows guest runtime/VC90 CRT:

```sh
python3 source/tools/calibration/experimental/native-edge-v34/capture-windows.py --measurements fresh-windows --tracer-sha256 867c3d05c466159cc981b18097dd84432006702ae1775e3451effbae670792da
```

`--case` selects one fixture. The wrapper verifies Core/probe/tracer SHA before
running. Build with included build.cmd in the guest folder
`C:\MotorRock\calibration-probes\contact-parent-v30\edge-v34`; copy the existing
CRT alongside it. Do not directly run the version-specific debugger against an
unverified DLL. Native paths are intended to have no spaces/cmd metacharacters.

Fresh Mac Debug build and53 CTest passed;165 earlier cases and12 ground-off cases
repeat exactly. New flag-off physical states and semantic events match v33.
First later differences occur at ticks27,24,7,3 for h0v0,h0v1,h1v0,h1v1 in all
three masses. Their cause is not yet established. The full608-case matrix was
last run at v27 with572 remaining per profile; it was not rerun or accepted here.
Later contacts, suspension, friction/lateral effects, tilted frames, restitution,
dynamic pairs, long race replays and AI remain required by the active goal.

A broader standalone64-pose/306-point check (recorded initial Native inputs,
not physical replay) reduces nonexact points from183 to70. Two previously exact
points regress and seven point distances increase. Thus it is not a complete
geometry fix and must not be promoted as general contact parity. Separation
reference in this old v26 fixture is reconstructed diagnostic, not exact SDK
internal separation. `geometry-probe.cpp` builds with C++17, `-O0
-ffp-contract=on`, using the included SourceMeshManifold.h.
