# V74 paired contact preparation — investigation, not accepted parity

Windows/PhysX 2.8.4 remains the immutable reference. This checkpoint reconstructs paired contact material axes, both inverse inertia tensors, both lever arms, and effective masses from current Mac simulation inputs. No engine power, car mass, shape dimensions, tire coefficients, springs, or fixture-specific outputs are changed. All new switches are off by default.

The independent force-response mass calculation matches 320 original solver rows (960 mass words), at O0 and O2. Fresh two-body lever transport matches 64 pairs; retaining tick39 local points and reconstructing them at tick40 matches another 16 pairs. These expected values are test fixtures only and are never loaded into the live physics adapter.

ARM fused multiply-add also caused two coordinate discrepancies in the vertical eight-point box contact. Default compiler behavior reproduces the live discrepancy; explicit product storage before addition restores every ordered geometry word at O0/O2. This establishes the observed arithmetic equivalence; the clipping instruction order has not been independently disassembled in this checkpoint. This correction is gated by RRR3D_EXPERIMENT_NATIVE_PAIR_CLIP_V74.

Actual Mac kernel inputs now match original Windows numerically in all 120 first-contact solver rows across five cases, plus 40 rows at tick39 for the two head-on cases. Raw signed-zero differences are recorded separately. The vertical case's maximum position difference improves from 1.7900000004e-7 to 7.9999999958e-9 source world units compared with the mass/preparation candidate, and its first differing physical sample advances from38 to39. Clipping changes only the two vertical weak-peer trajectories in the focused candidate comparison; 32 are unchanged. All34 trajectories repeat exactly. Disabled switches reproduce V73 and each preceding candidate's complete samples, metadata and contact history.

The combined candidate is NOT accepted globally:6/34 focused trajectories are strict, and some full-trajectory maxima remain worse than V73. In particular stock head-on position/orientation residuals remain. No new full608 parity claim or installed-game promotion is made. All targets were rebuilt/relinked and all53 fresh CTests pass; the signed arm64 game bundle builds with1200assets/14movies, but gameplay with this candidate is unverified.

Next: paired local-point persistence. Original Windows tick40 does not run fresh point transport; using retained tick39 local points reproduces both original levers exactly. Mac still reconstructs fresh local points, changes point order and contact masses. Restore the original reuse decision/order and persistent material frame without reading recorded expected states. Moving wheel-support angular-momentum and bump normal-force residuals also remain open.

Apply the previous research chain through accepted V73 first. Then run `python3 apply-experiment.py --adapter-root SOURCE --sdk-root JOLT`; `--restore` returns exactly to V73. Every input/snapshot hash is validated before any file is written. Rebuild all targets after applying or restoring. The normal repository adapter is not this experimental base.

Use the unchanged V73 candidate environment (`evidence/clip-focused/manifest.json` environment). For the combined diagnostic add all of:

- RRR3D_EXPERIMENT_NATIVE_PAIR_SOLVER_V73=1
- RRR3D_EXPERIMENT_NATIVE_PAIR_FRAME_V74=1
- RRR3D_EXPERIMENT_NATIVE_PAIR_INERTIA_V74=1
- RRR3D_EXPERIMENT_NATIVE_PAIR_LEVERS_V74=1
- RRR3D_EXPERIMENT_NATIVE_PAIR_MASS_V74=1
- RRR3D_EXPERIMENT_NATIVE_PAIR_CLIP_V74=1

`RRR3D_PAIR_GEOMETRY_TRACE_V74=1` emits independent actual geometry inputs; `RRR3D_CONTACT_NATIVE_TRACE_STEP` selects the completed sample tick. `RRR3D_NATIVE_PAIR_TRACE_FILE_V73` records actual solver inputs for the first256 rows. These are diagnostics, not injected reference data. Check binary flag presence before labeling a capture.

Run `python3 verify-proof.py` for portable independent C++ proofs. The evidence directory contains numerical reports, exact original Windows capture/hash manifests, repeat manifests, and fresh build/test logs. This checkpoint contains source and proofs; it is not a normal game installer.
