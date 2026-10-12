# Contact and friction investigation v51

See [the evidence report](../../../../docs/PHYSICS_CONTACT_FRICTION_V51_2026-10-12.md).

This default-off checkpoint closes `body-slide-x` for all 180 declared physical samples. The current retained suite contains 291 scenarios; it is separate from the broad 608-case suite and original contact-report API semantics. The fresh broad audit has 65/608 exact cases and 543 remaining, across 255864 samples and two identical repeats. The retained suite contains additional isolated scenarios, so these counts have different denominators. No engine, mass, tire or spring coefficient is fitted.

Apply the adjacent patch only to the byte hashes in `base.json`, after the full v50 experimental chain. Six new flags are listed there; they require the previous Native arithmetic/frame flags, with `RRR3D_EXPERIMENT_NATIVE_EXACT_SHAPE_COM=1`. `RRR3D_EXPERIMENT_NATIVE_NONUNIT_POSE_MATRIX=1` enables bounded assertion compatibility only with the original angular integration flag. The bound is 3e-5 in squared quaternion length; actual Windows histories reach 1.91e-5. Matrix/rotator arithmetic and physical quaternions are unchanged. Only Jolt's three-float relative-orientation cache key is normalized when it needs a unit representation; Native contact axes remain raw, while unused generic Jacobian placeholders receive valid unit axes.

Friction is restricted to static/dynamic source block PGS contacts, zero restitution and surface velocity, one zero friction coefficient, positive second coefficient, and Native arithmetic mask31. Both tangent axes are still calculated because the zero coefficient axis cancels small float projections. Original general friction, dynamic pairs and API contact-force reporting are separate verification scopes.

`trace-contact.cpp` is a read-only debugger for the unchanged original Windows probe and user-supplied SDK. It requires VC2008 x86; proprietary binaries are not bundled. Transform probes compare current C++ headers with actual Native values under O0/O2. The pose probe observes assertions without taking a trap in the verifier only; runtime Debug assertions remain enabled, including rejection of NaN and large non-unit inputs.

For a downloaded evidence package:

```sh
python3 source/tools/calibration/experimental/native-contact-friction-v51/verify.py --package . --out verification.json
python3 source/tools/calibration/experimental/native-contact-friction-v51/replay.py --package . --out fresh-replay
```

The replay relocates only the provenance path after validating the profile contents. Controls retain exact COM and disable all six new flags. This is an investigation package, not a claim that all physics or an interactive race has passed.
