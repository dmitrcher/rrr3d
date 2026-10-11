# v43: original COM/shape quaternion translations and normal-mass projection

The original frozen actor has distinct public and prepared physical poses. With
an offset center of mass, this distinction affects translation as well as the
rotation separated in v42. A private original Windows trace located the first
error before motion: COM X is 0.4844561219215393, while Jolt's unit-matrix result
is 0.48445621132850647. The prepared physical actor origin is
-0.0000002384185791015625, while the old Mac geometry used zero.

Original NxQuat::rot evaluates
`(v*(w*w-0.5f) + cross(qv,v)*w + qv*dot(qv,v))*2`, with observed float stores.
Reproduce the original initial COM, prepared actor origin, and public actor
output independently. Shape-to-COM translation must be composed locally and
transformed once; separately translating actor origin and shape position in
world space introduces more rounding. No force or power coefficients can repair
these coordinate-transform errors.

A sixth fixture with offsets on all three axes exposed another earlier error:
the original normal-response projection at PhysXCore 0x84633 sums Z, X, Y.
The v32/v37 experimental mass path summed X, Y, Z. Its limited earlier fixtures
happened to produce the same result. Changing the projection order closes this
fixture's first 60 steps and eight more complete original tilted contacts.

The last residual in the new fixture was solely position at tick 60: integrated
COM Y -1.3255208730697632 became -1.3255207538604736 after actor-origin subtraction
and addition during frozen restoration. The v42 COM-preserving strategy was
mathematically correct but still lost a bit. The new strict path repairs the
actual integrated COM through the body position step and refreshes shape bounds,
without recalculating mass properties. All 720 COM-restoration records in each
final expanded repeat preserve the integrated center exactly; eight previously
rounded restores are corrected.

## Default-off candidate and verification

Six new RRR3D_EXPERIMENT_NATIVE_ flags are INITIAL_COM, ACTOR_ORIGIN,
ACTOR_OUTPUT, SHAPE_TRANSLATION, MASS_DOT_ORDER and FROZEN_CM_EXACT. The isolation
runner's `--contact-v43-quaternion-pose` enables them plus the complete v42 chain.
The three-file patch changes only the experimental adapter, a new scalar helper
header and the existing experimental SDK NativeContactArithmetic.h. It is
against the exact recorded v42 baseline, with before/after hashes. Production
has not been promoted; engine, brake, tire, suspension and gravity scales stay 1.

- 72 original tilted fixtures, 120 steps each, two repeats: **44 exact, 28
  remaining**, versus 36 exact in the disabled v42 control. All previous exact
  fixtures remain exact. New full matches: p1 x0 y1, p2 x2 y1, p3 x1 y1,
  p4 x0/x1 y1, p6 x1 y1, p7 x1 y1 and p9 x2 y1.
- Position improves in 19 cases and worsens in seven; velocity improves in 20
  and worsens in five. Global maxima still are position 0.0022348796830243055
  and velocity 0.04756044447137011. Partial regressions remain unresolved.
- **6/6 expanded COM/shape fixtures match all 120 steps in both repeats**;
  the v42 control matches 2/6. Effective coefficient scales equal one, with no
  AI, engine, wheel-ray, tire, brake or damping forces in these fixtures.
- Cumulative final-binary ablation stages, two repeats each, enable initial COM,
  actor origin, actor output, local shape composition, mass projection order and
  exact COM restore in that order. Exact counts are **2, 2, 3, 5, 5, 6**. Every
  disabled control equals v42; repeated full physics diagnostics match too.
- Independent native formula reconstruction: **896 shape-to-COM translations,
  2048 normal-response masses and eight initial COM values** match exactly.
  These use observed original cache-miss and initialization records, not all
  native internal calls. Tracer initialization hits are capped at 256 per run.
  Eight traced native physical runs remain equal to the immutable Windows data.
  Recorded inputs are diagnostic verifier data only; the adapter computes its
  own runtime values and consumes no measured states or fixture identifiers.
- Six old detailed fixtures at steps 0–2: all **475 solver rows × eight fields**
  match. Four new detailed fixtures at those steps add **70 exact rows**. Actual
  stored cache frames, transverse errors, local anchors and normals also match
  in both sets and both repeats. The comparisons rebuild from raw captures.
- **189 force-isolated scenarios match**, twice. Full Debug app builds and signs,
  verifies 1200 assets and 14 movies; **53/53 CTest tests pass** (20.90 seconds).
  No fresh graphical race or full-game AI behavior was verified in this turn.

The complete goal remains active: 28 tilted contact cases, broader suspension,
body interaction and full GameCar/AI tests are unfinished. The earlier engine
precision uncertainty remains bounded by the v39/v40 audit; v43 does not promote
an engine arithmetic policy. Partial contact regressions prevent production
promotion of this experimental chain.

The separate audit ZIP contains raw measurements, SHA256 manifests, source
snapshots, patch, verifiers, Mac probes and the unchanged separate v42 Windows
reference/tracer helpers. It is not a game installer. Final Mac probe SHA256:
2d896e84e8cbb7960b152d15c60d8a2c45579ddc6237417a75777c533c2d24bc.
