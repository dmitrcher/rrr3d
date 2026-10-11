# v41: reproduce original solver inertia preparation

The original Windows PhysX 2.8.4 solver builds its global inverse inertia with
scalar SSE float operations at PhysXCore RVA `0x7c5a0`. The public accessor and
the v37 Mac reconstruction keep extended intermediates. These are different
arithmetic paths. The earlier v37 reconstruction matched four observed fixtures,
but did not prove the solver algorithm for other orientations.

A new independent trace of `frame-v25-long-p9-x2-y0` located a difference before
any motion: solver Izz is `0.0016703986329957843`, while the public Windows accessor
and old Mac result are `0.0016703987494111061`. Actor quaternion and the three
principal local inverse-inertia values match exactly. Hardware write tracing
identified the actual preparation routine; notably, its Izz term computes
`(r22*r22)*dZ`, but another term computes `(dX*r20)*r20`, storing each operation
as float. Changing physical coefficients cannot repair that rounding path.

The new default-off `RRR3D_EXPERIMENT_NATIVE_SOLVER_INERTIA=1` reproduces the
observed quaternion matrix, term ordering, and stores. It applies to diagonal
local inverse-inertia tensors in the existing static-contact experiment. A
non-diagonal local tensor retains the previous path until its mass-frame
orientation is independently verified. No engine, brake, tire, suspension,
gravity, or mass coefficients were changed. Windows binaries are unchanged.
The two-file patch is against exact experimental v40 sources, not the production
adapter. The original v37 path remains available with the new flag disabled.

## Verification

- 72 tilted synthetic body-contact fixtures, 120 steps each, two independent
  repeats: **27 exact, 45 remaining**, versus 18 exact in the disabled v40 control.
  All 18 previously exact fixtures remain exact; nine more now match fully.
- Position error improves in 18 cases and worsens in seven; velocity error
  improves in 21 and worsens in five. These partial cases remain unresolved.
  Global maximum position difference is `0.0022348796830243055`; maximum velocity
  difference is `0.04756044447137011`. This is not evidence of complete parity.
- Five detailed fixtures, steps 0–2, two repeats: all **450 solver rows × eight
  compared fields** match independently captured original Windows rows. Their
  full 120-step physical states match too, including the formerly regressed p9
  fixture. The tool rebuilds the comparisons from raw captures.
- 189 force-isolated no-AI scenarios match exactly, in two repeats.
- Hardware traces reconstruct **80 original tensor writes / 720 entries**
  exactly. Both traced 120-step original runs equal the immutable Windows
  baseline. Diagnostic reconstruction consumes recorded inputs only in the
  verifier; the Mac implementation calculates its own inputs at runtime.
- Full Debug app builds, signs and verifies 1200 assets and 14 movies;
  **53/53 CTest tests pass**. This turn did not verify a fresh graphical race.

The complete goal stays active: 45 tilted contacts, broader suspension/body
interaction cases, and full-game GameCar/AI checks remain. Another independently
observed lead is the p7 cache pose: its initial local-normal input differs because
geometry and physical actor matrices differ by two float ULPs. That change has
not been implemented in v41 and must be tested separately.

The historical engine audit remains bounded by v40: original game-DLL torque
matches its source at separately tested PC53 and PC24 precision, but actual game
thread precision at the wheel-command boundary has not been captured. Do not
promote a torque arithmetic policy or tune power based on these contact tests.
