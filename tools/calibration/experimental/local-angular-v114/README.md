# Original local angular acceleration transform V114

Validated investigation checkpoint; overall goal remains incomplete. Two adapter files over verified V113, with new flag RRR3D_EXPERIMENT_NATIVE_LOCAL_ANGULAR_V114 off by default. The immutable original local-vector rotation uses a norm-sensitive scalar quaternion formula. Jolt uses a Rodrigues expression assuming unit length. For the original slightly non-unit pose these are numerically different. Original scalar rotation is applied to acceleration before multiplying by dt; coefficients and pose values remain unchanged.

Eight actual original transform results match production header O0/O2. Actual original API-entry/angular-integrator states also establish add-delta then drag order, retaining existing actor damping. Eight live Source captures match full120-step original histories and the same-input delta/integrator outputs. Private signed-zero differences are reported separately; raw ABI equality is not claimed. No reference states or case/frame rules enter runtime.

Portable verification covers8400fresh Windows-equivalent states and120flag-off V113-preserved states. Overall physics and complete-game/render equivalence remain incomplete; ordinary installation unchanged.

Complete matrix: 591/608 strict, 17 open, all590previous strict retained. One new complete120-step air-pitch history matches Windows;607histories unchanged. Two255864-state repeats exact. No audited full-run maximum worsens.53project checks pass both default and candidate profiles;291independent gates,solo600/rendered240 functional checks pass.
