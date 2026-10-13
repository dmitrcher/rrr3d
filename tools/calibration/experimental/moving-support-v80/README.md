# V80: original box ray query and contact point reconstruction

The default-off `RRR3D_EXPERIMENT_NATIVE_WHEEL_BOX_QUERY_V80=1` switch restores
the source box-ray calculation for wheels contacting dynamic vehicle boxes.
Static queries retain the previously verified path. It uses the full V79
research profile, with unchanged engine, mass, spring, tire and material inputs.
The source does not read Windows captures or dispatch on fixture names.

Windows PhysXCore's actual virtual raycast target is RVA1ae390. It transforms
the ray into box coordinates with separate origin/direction dot associations,
calls the candidate-plane intersection at RVA1ea750, then transforms the local
face point back to world coordinates. Mac previously interpolated the world
ray directly using a slab intersection. Those mathematically related operations
produce different float32 contact points when the box rotates.

The reconstructed algorithm preserves the original clamped face coordinate,
strict candidate comparisons, negative-one sentinel, inside rejection and
1e-5 side-bound tolerance. The two constants were read from the immutable DLL.
Forward projection preserves the original axis-specific operation associations.

## Verification

* Native original captures at physical samples28/29, two repeats, preserve all
  original300-state histories and executable/DLL hashes. The pure helper matches
  16 query records and272 raw output words at O0 and O2 without tolerances.
* Actual live Mac sample29 points and normals match all24 original words, in
  two repeat captures; Windows data is only an offline comparison oracle.
* The complete stock-pole0 trajectory now matches all300 physical samples.
  Stock-pole1 angular-momentum maximum error decreases from0.0002523085 to
  about6.503e-11. Its first full-state difference remains sample29: the sign of
  longitudinal slip, +9.09494702e-13 versus -9.09494702e-13. Body observables first
  differ later: angular momentum/rotation32, velocity35, position49.
* Fresh full608-case runs each contain255864 samples and repeat exactly.
  117cases are strictly exact,491 remain open; all116 V79 exact cases are retained.
  606 complete trajectories are unchanged, two improve; seven maximum-error
  metrics have no regressions. All291 independent gates and53 CTest tests pass.
* Full signed arm64 game rebuilt; solo real-track headless and separate rendered
  six-car functional launches exit cleanly. These are not full-game Windows
  numerical or graphical parity claims.

The prototype addresses dynamic vehicle box queries only. The pure records cover
the two first-contact frames, not every possible ray geometry. The broad matrix
does not establish universal SDK compatibility. V79 lifecycle limitations still
apply. The first remaining slip difference led to an independently source-proven
reversed subtraction in the previous Mac implementation; that correction is
not included here. Overall parity remains incomplete.

## Reproduce

Apply two guarded files with `apply-experiment.py --adapter-root PATH --sdk-root
PATH` over the exact V79 research overlay chain, then rebuild. `--restore` restores
the prior source. Every hash is checked before mutation. Root release sources
are a different base; no Jolt SDK files change in this checkpoint.

`run-probe.py --probe PATH --out NEW_PATH` launches the frozen explicit profile.
`verify-checkpoint.py --probe PATH` verifies retained evidence and freshly reruns
the four focused cases,1200 states. `proofs/verify-math.py` builds independent
C++ arithmetic checks using Xcode Command Line Tools. Both portable verifiers
work from another directory. The workspace-reproduction scripts preserve local
workspace paths for audit. This is a research checkpoint, not a new default
installed game release.
