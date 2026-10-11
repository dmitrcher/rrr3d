# v44: separate public triangle normal for the contact callback

The p7/x2/y1 trace first diverged in the contact callback, before any response.
Geometry, actor pose, the private material basis and callback seed were exact.
The Windows callback obtains triangle vertices and calls public
`NxTriangle::normal`: `(verts[1]-verts[0]) ^ (verts[2]-verts[0])`, then
`NxVec3::normalize`. The Mac experiment reused the private collision-plane
normal. These are distinct calculations. On this slope their Z components are
0.9759001135826111 and 0.9759000539779663 respectively. Reusing the plane changed
two callback matrix components, then quaternion W and the decoded solver normal.
No engine, grip, suspension, brake or gravity coefficient can correct that error.

The adapter now independently derives the public callback normal from the first
contributing triangle's transformed vertices. The private collision normal still
feeds the private material basis and raw error frame. Existing scalar public
NxVec3 arithmetic stays unchanged. `RRR3D_EXPERIMENT_NATIVE_CALLBACK_TRIANGLE_NORMAL=1`
enables this candidate; it remains off by default. The isolation runner option
`--contact-v44-callback-triangle-normal` enables it with the complete v43 chain.
The two-file patch applies to byte-exact v43 experimental sources. It also adds
optional material-stage tracing (`RRR3D_CONTACT_MATERIAL_STAGE_TRACE=1` and
`RRR3D_CONTACT_NATIVE_TRACE_STEP`), exposing both normals, poses and intermediate
frames. Diagnostic fields alone preserve all v43 physical states.

## Verification

- Original 72 tilted contact fixtures, 120 steps each, two repeats: **67 exact,
  five remaining**, versus 44 exact with the v44 flag disabled. All previous
  exact fixtures remain exact. Position and velocity improve in the same 23
  fixtures; neither worsens in any fixture. Commands, coefficient records and
  physical states repeat; disabled controls equal v43.
- **189 force-isolated fixtures and all six expanded COM/shape fixtures remain
  exact in two repeats.** The expanded disabled controls equal v43. AI, wheel
  rays, motor, brake, tire forces and damping are disabled; effective coefficient
  scales remain one.
- At the new p7/x2/y1 first geometric pass, all **17 public callback matrix and
  local-quaternion components match Windows**, including all four callbacks.
  All **20 solver rows × eight compared fields match**, replacing 37 differences.
  Windows public telemetry updates its tick after simulation; the verifier uses
  the private first-pass point count to exclude callbacks from the next pass
  logged with the previous tick. Full 120-step physical equality is checked
  separately. Original traced Windows physical states remain unchanged.
- Six prior detailed fixtures at steps 0–2 retain **475 exact solver rows ×
  eight fields**. Actual stored cache frames, transverse errors, anchors and
  normals remain exact in both repeats.
- Full Debug app builds and signs, verifies 1200 assets and 14 movies;
  **53/53 CTest checks pass** (16.82 seconds). No fresh graphical race or full-game
  AI behavior has been certified by this checkpoint.

Residuals are all nearly inverted pitch 3.14:

| Fixture | First different tick | Max position difference | Max velocity difference |
| --- | ---: | ---: | ---: |
| p11/x0/y0 | 3 | 0.00030043271809301273 | 0.007802910088146683 |
| p11/x0/y1 | 45 | 0.00025891344165282205 | 0.0021433645053048193 |
| p11/x1/y0 | 0 | 1.0909999999977166e-11 | 1.1920000058852594e-7 |
| p11/x1/y1 | 4 | 0.0022348796830243055 | 0.04756044447137011 |
| p11/x2/y1 | 17 | 0 | 1.200000001588819e-7 |

A fresh immutable Windows p11/x1/y0 private trace confirms a new lead: the third
contact row already has different constraint penetration/error and scaled bias
before the first differing impulse. This is not fixed by v44. The goal remains
active. The planar fixtures do not establish equivalence for arbitrary curved
meshes, full suspension, tires or game AI. Production promotion waits for broader
verification and elimination of remaining discrepancies.

## Reproduce

Apply the previously published v43 experimental source/SDK chain, then apply
`tools/calibration/experimental/native-callback-triangle-v44.patch`.
The companion base manifest records exact before/after hashes. Rebuild the probe
and Debug app. The candidate directory includes full changed source and baseline
copies, capture tools, patch and raw-data verifiers. In the extracted audit package:

```
python3 source/tools/calibration/experimental/native-callback-triangle-v44/verify.py --measurements measurements --probe bin/Mac/SpringProbe
python3 source/tools/calibration/experimental/native-callback-triangle-v44/verify-cache.py --measurements measurements
python3 source/tools/calibration/experimental/native-callback-triangle-v44/verify-patch.py --out patch-check.json
```

`manifest.json` hashes every bundled file. The package is an experimental probe
and audit checkpoint; it requires the v43 prerequisites to rebuild the full game.
No measured states, fixture identifiers or expected outputs enter runtime physics.
