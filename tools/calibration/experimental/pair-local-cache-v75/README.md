# V75 paired cache and material transport investigation

Windows PhysX remains the immutable reference. This overlay reconstructs the paired local-contact cache, keeps original point order on reuse, transports the full raw and material quaternions, uses the physical actor matrix for anisotropic direction, and applies the final material normal error in the paired solver. Engine power, masses, geometry, tire coefficients and suspension parameters are unchanged. All new switches are off by default.

Apply the research chain through V74, then run `python3 apply-experiment.py --adapter-root SOURCE --sdk-root JOLT`. `--restore` returns exactly to V74. The four-file installer validates every hash before changing any file; apply, restore, idempotence and rejection of an incorrect last file were tested in disposable directories. Rebuild all targets after applying or restoring.

Use the V73 environment from `evidence/focused/manifest.json`, the six V74 switches listed in its `mode_changes.v74_previous`, and all three additions:

- `RRR3D_EXPERIMENT_NATIVE_PAIR_LOCAL_CACHE_V75=1`
- `RRR3D_EXPERIMENT_NATIVE_PAIR_MATERIAL_TRANSPORT_V75=1`
- `RRR3D_EXPERIMENT_NATIVE_PAIR_ERROR_V75=1`

The manifest records each tested combination explicitly. Disabled additions preserve the full V73 and previous V74 focused histories. Every focused combination was captured twice with exact repeat histories. Fixed scripted commands and seed42 are used, with no AI-controlled opponents. Paired fixtures use two prescribed bodies so that car/car contacts can be tested deliberately.

The pure cache proof carries its own state between frames, without injecting recorded previous caches. It matches all32 original transitions, all144 ordered22-float records, and all reuse decisions in independent cold and warm inputs at O0 and O2. Complete material transport matches18 original solver quaternions; final error transport matches48 original normal errors. Expected values reside only in proof fixtures. Live physics does not read recorded expected outputs or case names.

Actual Mac paired solver inputs match all320 original rows across14 cold/warm frames of five cases, including cached contacts on steps39–41. Signed-zero differences are listed separately from numerical differences. The new cache trace reports actual current state; it is selected with `RRR3D_PAIR_CACHE_TRACE_V75=1` and `RRR3D_CONTACT_NATIVE_TRACE_STEP`. Kernel traces are bounded to the first256 rows.

The focused result is28/34 strict float32 observable trajectories, compared with0/34 for accepted V73 and6/34 for V74. The full608 matrix has112 strict scenarios (previously84), preserving all84 previous exact scenarios, and496 remain open. Both255864-state repeats match exactly; all291 independent gates pass in both modes with two repeats. Overall physics parity remains incomplete. The combined candidate is not globally accepted: the complete trajectory audit found a position regression in pair-stock-weak-second-1 and an orientation regression in pair-stock-weak-second-0. Both have further contact geometry/material discrepancies, recorded in the remaining-input audit. All targets were freshly rebuilt and relinked;53 fresh CTests pass. The signed arm64 game contains1200assets and14movies. A900-tick real-map solo replay and headless resource/physics smoke pass. A separate240-frame rendered menu/race lifecycle smoke passes with six cars and AI; this is not an isolated Windows trajectory comparison. Its exported garage screenshot was inspected. No normal installed-game promotion or complete physics-parity claim is made.

Run `python3 verify-proof.py` to compile and run the portable independent C++ proofs. The evidence directory contains exact original Windows manifests, numerical audits, repeat manifests and validation results. This is a source/evidence checkpoint, not a full game installer.
