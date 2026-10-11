# Native physical pose v42 (experimental)

The original Windows solver uses a normalized internal pose for geometry and
cache transport, while its material callback can still see the saved API pose
of a frozen body. Reproduce each separately; do not tune physical coefficients.
The frozen-rotation restoration also has to preserve integrated center of mass.

Enable the full chain with `run_isolation.py --contact-v42-physical-pose`.
The three new default-off flags are `RRR3D_EXPERIMENT_NATIVE_GEOMETRY_POSE`,
`RRR3D_EXPERIMENT_NATIVE_CACHE_POSE`, and
`RRR3D_EXPERIMENT_NATIVE_FROZEN_CM_RESTORE` (value `1`). Prerequisite v41 and
earlier flags remain necessary. Production has not been promoted.

The three-file patch `../native-physical-pose-v42.patch` reconstructs the adapter,
new header, and six added fixtures from the exact `before/` files. Hashes are in
`../native-physical-pose-v42-base.json`. `verify-patch.py --out RESULT.json`
checks reconstruction byte for byte. ContactFrameScalar.h is included only so
that the standalone formula helper can be rebuilt.

Capture filenames retain v39 labels for tool compatibility. The ordinary long
capture enables all three new flags; its control disables them and must equal
v41. `verify.py` checks two repeats, capture hashes, 189 isolation scenarios,
72 long fixtures, all eight effective coefficient scales equal one, 12 original
native traces unchanged, and all eight compared fields in 475 detailed solver
rows across six fixtures and steps 0–2. `verify-cache.py` separately checks actual
cache frames, transverse errors, anchors and normals from both repeats.

`pose-probe.cpp` reconstructs the observed geometry quaternion-to-matrix SSE
formula. Build it with clang++ -std=c++17 -O2 -ffp-contract=on and the candidate
physics header include path. `verify-geometry.py --measurements DIR --probe EXE`
checks 240 original matrices / 2880 entries and unchanged physical states.
Recorded native inputs are diagnostic verifier data, never adapter inputs.

`trace-contact.cpp` adds a read-only geometry matrix breakpoint to the existing
native tracer. `build.cmd` uses VC2008 x86. `capture-geometry-windows.py` checks
the immutable original probe and PhysXCore SHA256 before capturing. Original
SpringProbe.exe: 8c331b7ea35ed6181c0f8dd52cddca451f60c817d381d360cb53852e8c49ed9d.
PhysXCore.dll: c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb.
Tested geometry tracer: 28b488858ba87cfe4bc468a3bbd1082217c1a75d8ca639f5ead3ff4321695180.

The expanded probe adds six no-AI frozen contacts with offset centers of mass
and shape origins. Build the normal windows-spring-probe.cpp with the new shared
IsolationProbeCases.h using build-spring-probe.cmd against the original SDK.
Use a separate guest folder; leave the immutable probe intact. Expanded exe:
d905fe99f4a835917562b3e4dd9c9b0fdf513ab8ebf75ad5750679462c44dbef.
`capture-expanded-windows.py` first checks all original 72 fixtures still equal
the immutable reference, then captures the six new fixtures twice.
`capture-expanded-mac.py` holds geometry/cache flags on while toggling only COM
restoration. `verify-expanded.py` checks this ablation, repeatability, hashes,
and all original Windows states. Its control equals the earlier pre-COM run.

36/72 original long cases and 2/6 expanded cases are exact. The other 40 cases
remain unresolved, as do broader suspension/body and actual full-game/AI checks.
This is an audit checkpoint, not a game installer or a claim of complete parity.
See docs/PHYSICS_CONTACT_V42_2026-10-12.md for measured limits.
