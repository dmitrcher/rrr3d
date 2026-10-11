# v42: separate physical geometry, callback and cache poses

The original Windows engine can expose two actor rotations in the same frozen
body step. Its internal geometry uses a normalized physical quaternion, while
its material callback sees the saved API quaternion. The old Mac path reused one
representation. A read-only original PhysX trace confirmed the distinction:
for p8 at step zero, geometry qY is 0.12467475980520248 and qW is
0.9921977519989014; callback qY is 0.12467474490404129 and qW is
0.9921976327896118. The off-diagonal matrix value is respectively
0.2474040389060974 and 0.24740397930145264. This explains the earlier p7 cached
normal mismatch without changing grip, power or suspension coefficients.

The default-off experiment now constructs the geometry and cache rotation using
the original scalar SSE operation ordering from the prepared physical quaternion.
The material frame still uses the saved public pose. Three new flags are enabled
by `run_isolation.py --contact-v42-physical-pose`: NATIVE_GEOMETRY_POSE,
NATIVE_CACHE_POSE and NATIVE_FROZEN_CM_RESTORE (all prefixed RRR3D_EXPERIMENT_).
All prerequisite v41 flags remain necessary. The patch applies to the exact v41
experimental adapter and shared fixture header, adding NativePhysicalPoseV42.h.
The production adapter has not been promoted.

Six new fixtures vary center-of-mass offset and box shape origin while keeping
rotation frozen. They expose a much larger previous error: restoring rotation
with SetRotation preserved actor origin and displaced the integrated center of
mass after each solver update. The new restore computes actor origin from the
saved orientation and current integrated COM, keeping COM fixed. This correction
is independently toggled with geometry/cache corrections held on.

## Measured verification

- 72 original tilted contacts, 120 steps each, two repeats: **36 exact, 36
  remaining**, versus 27 exact in the disabled v41 control. No previously exact
  scenario is lost. Nine new complete matches: p0 x0/x1/x2 y0, p2 x0 y0,
  p3 x0 y0, p8 x1 y0, and p10 x0/x1/x2 y0.
- Position error improves in 12 partial cases and worsens in four; velocity
  improves in 14 and worsens in three. Global maxima remain position
  0.0022348796830243055 and velocity 0.04756044447137011. These cases remain open.
- Six detailed fixtures at steps 0–2: **475 solver rows × eight compared fields**
  match the original native captures, with identical repeats. Actual stored
  cache frames, transverse errors, local anchors and normals also match for all
  six cases at all three steps, closing the formerly observed p7 normal defect.
- Independent original geometry tracing: **240 matrices / 2880 entries** are
  reconstructed exactly. The traced physical runs equal the immutable reference.
  The reconstruction helper uses recorded inputs for verification only; the Mac
  adapter calculates its own runtime pose.
- The separate expanded original Windows probe preserves all 72 old physical
  scenarios in both repeats. Only fixture definitions were added; the original
  immutable probe and original PhysXCore.dll remain intact.
- New COM/shape tests: **2/6 exact, four remaining**. The largest position error
  falls from **6.442520161607582 to 0.0000020682706259793067** in game coordinates.
  Maximum remaining velocity error is 0.0000028775857938730797. The COM-only
  disabled control reproduces the earlier geometry/cache-only candidate exactly.
- 189 force-isolated scenarios match exactly in two repeats. All eight effective
  calibration coefficient scales equal one. Fixtures have no AI, vehicle engine,
  brake, wheel-ray or tire forces that could hide body-contact defects.
- Full Debug app builds and signs, verifies 1200 assets and 14 movies;
  **53/53 CTest tests pass** (12.38 seconds). No fresh graphical race or actual
  full-game AI behavior was verified in this checkpoint.

The complete goal stays active. The next lead is first-step COM/actor-origin
rounding in the four offset-COM fixtures, followed by the remaining 36 long
contacts and broader suspension/body and full-game checks. Synthetic body tests
do not prove the complete vehicle, torque or AI pipeline. The historical engine
precision uncertainty remains as described in the v39/v40 audit; engine power
and tire/suspension parameters were not adjusted in v42.

Reproducible raw captures, SHA256 manifests, default-off source snapshots,
three-file patch, and verifiers are distributed in the separate v42 audit ZIP.
Its Mac probe SHA256 is
7533b16db4eac98b7a6154dcf3a1ed9446cc306faf1f61e8278f09845cbeb681.
This package is an experimental audit checkpoint, not a game installer.
