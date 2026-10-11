# v45: original upper local-Z polygon order

The five remaining original tilted contacts were nearly inverted. The p11/x1/y0
private trace located the first error in the third contact's penetration, before
response. A standalone reconstruction of the existing edge formula matched all
four original accepted edges. Actual Mac stage tracing then revealed the missing
input distinction: the same upper-face vertices were traversed in reverse.
Reversing a segment is algebraically equivalent, but subtraction from the other
endpoint changes float rounding. One penetration differed by one float step.

Original upper local-Z traversal uses corner indices **1, 0, 3, 2** in the
wrapper's two-axis corner table. The earlier v23 upper-face winding experiment
used **0, 1, 2, 3**. The new candidate overrides that earlier experiment for the
positive local-Z face only. Other faces and the edge formula are unchanged.
`RRR3D_EXPERIMENT_NATIVE_POSITIVE_Z_FACE_ORDER=1` enables it; the isolation runner
option `--contact-v45-positive-z-face-order` enables it with the complete v44
chain. It stays off by default and changes no force coefficient.

Optional `RRR3D_CONTACT_EDGE_STAGE_TRACE=1` with
`RRR3D_CONTACT_NATIVE_TRACE_STEP` records actual edge inputs, normalized vectors,
plane, coefficient, ratio, before/after points and distance. It is diagnostic
only. The three-file patch includes this adapter/header tracing and six new
shared inverted COM/shape fixture definitions. No SDK file changes in v45.

## Verification

- **72/72 original tilted contacts now match Windows**, all 120 steps in two
  repeats. The disabled v44 control matches 67/72 and equals the published v44
  physical data. All five residuals are closed, with no lost exact cases or
  increased position/velocity errors.
- The independent actual Mac stage trace matches **all 15 fields of each of
  four accepted original p11/x1/y0 edge crossings**, twice. All **40 first-pass
  solver rows × eight fields** now match, replacing 109 field differences.
- Six prior detailed fixtures at steps 0–2 retain **475 exact solver rows ×
  eight fields**. Actual stored cache frames, transverse errors, anchors and
  normals remain exact in both repeats.
- **189 force-isolated scenarios and all six existing COM/shape scenarios
  remain exact**, twice; disabled controls for the old expanded fixtures equal
  v44. Full Debug app builds and signs, verifies 1200 assets and 14 movies;
  **53/53 CTest checks pass** (16.36 seconds).
- Six added `pose-v45-inverted-0..5` fixtures reuse the existing independent COM
  and shape offsets with pitch 3.14 and slope Y 0.1. Both platforms compile the
  same shared definitions. **Five are exact; one remains.** All six improve in
  position and velocity against the disabled v44 algorithm. The new separate
  original Windows probe preserves all 72 immutable original fixtures in two
  fresh runs. The original game, immutable probe and PhysXCore on disk are unchanged.

No AI, wheel rays, motor, brake, tire, friction or damping forces occur in these
fixtures. Effective coefficient scales remain one. Runtime geometry consumes no
fixture names, measured states or expected outputs.

## New residual and next action

`pose-v45-inverted-5`, with shape offset (0.2,0.1,0.1) and COM (0.2,-0.3,-0.4),
first differs at tick 50. Maximum position difference is
0.000007212835780869999; maximum velocity difference is
0.00006287544035638493. The disabled control had position 0.012295778056372362
and velocity 0.16053391413224116. This improvement does not constitute equality.

Focused private Windows tracing at steps 49–50 preserves the untraced physical
states over all 120 steps in both repeats. The geometry matrix matches Mac at
step 50. The first differing solver field is inverse inertia ZZ; it then changes
angular velocity after response. A second focused tracer reads the original
solver preparation's actual quaternion, local principal inverse inertia and
output tensor. The **local Z inverse-inertia coefficient already differs**:
Windows 0.0006961781182326376, Mac 0.0006961781764402986. X and Y agree. The
existing global SSE rotation is therefore not the first error. Next, inspect
principal mass/inertia preparation and its earlier approximate eigenvalue path.
No mass correction is accepted or encoded by this checkpoint.

The goal remains active. Closing the old tilted suite does not certify arbitrary
meshes, dynamic rotation, tire/suspension/brake/engine coupling, collisions,
full-game race behavior, AI or the broader calibration matrix. Production has
not been promoted and no fresh graphical race is certified by this checkpoint.

## Reproduce

Apply the full published v43 source/SDK chain and v44 adapter patch, then the
adjacent `native-positive-z-order-v45.patch`. The base manifest records exact
before/after hashes. Rebuild the Mac probe/app. The audit package contains the
new separate Windows fixture probe, two focused tracer binaries and their source;
using them requires the original SDK/VC90 CRT and exact PhysXCore hash recorded
in the Windows export evidence. `MOTORROCK_NATIVE_TRACE_STEP=50` selects the
focused trace window; private breakpoints and numeric logging do not supply
simulation values to either runtime.

From the extracted package:

```
python3 source/tools/calibration/experimental/native-positive-z-order-v45/verify.py --measurements measurements --probe bin/Mac/SpringProbe
python3 source/tools/calibration/experimental/native-positive-z-order-v45/verify-cache.py --measurements measurements
python3 source/tools/calibration/experimental/native-positive-z-order-v45/verify-next-lead.py --measurements measurements
python3 source/tools/calibration/experimental/native-positive-z-order-v45/verify-patch.py --out patch-check.json
```

The package is an experimental probe and audit checkpoint, not a game installer.
`manifest.json` hashes every bundled file and identifies its exact source commit.
