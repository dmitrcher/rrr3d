# Original wheel query reconstruction v52

This experiment fixes first-contact geometry before changing spring or tire coefficients. It requires the exact adapter before hash and full experimental v51 chain, plus v52 probe logging. Apply `wheel-ray.patch` only to that state. Both new flags are disabled unless their values are exactly `1`:

- `RRR3D_EXPERIMENT_NATIVE_WHEEL_RAY_ORIGIN`: original unraised wheel origin and world endpoint subtraction, currently restricted to identity vehicle rotation. Other rotations retain the existing path.
- `RRR3D_EXPERIMENT_NATIVE_WHEEL_TRIANGLE_POINT`: scalar float32 triangle intersection and barycentric point construction on original source vertices. Currently restricted to registered static identity mesh bodies. Jolt still selects the candidate triangle.

Original Windows trace inputs include 24 successful rays from seven cases, two actual repetitions per capture, with the corresponding original 600-state histories unchanged. `python3 verify-math.py` compiles the helper at `-O0` and `-O2`; all 216 result words per optimization match the original distance, barycentric coordinates and contact points exactly, including zero signs. No reference values or case names are used by the runtime helper.

Four actual Mac cases have two repetitions for each independent control/origin/point/combined ablation. The final control preserves the published v51 histories. At the first contact of settle, spring-soft and suspension-drop, all four wheel points, normals and suspension lengths match Windows float32 words exactly. The first-step position matches in all three, and velocity matches in settle and spring-soft. Angular momentum and rotation still differ; suspension-drop also retains a velocity difference. No full vehicle parity is claimed.

Fresh control and candidate runs retain all 291 independent gates, with two repetitions each. The rebuilt Debug application passes all 53 CTest checks and packaging/signature verification. These checks do not certify an interactive race or the full 608-case candidate matrix. The last published v51 broad result remains 65 exact / 543 unresolved.

`base.json` records exact source and binary hashes. `capture-hashes.json` records raw local histories and traces. The compact measured inputs and reports are included here; the original proprietary PhysX binary is not included. The original spring creation descriptors are also recorded, excluding unstable body handles. Their existence does not yet validate the complete original spring solver. Prior v17 final-bias reconstruction used tolerance checks and must now be checked against original internal rows before changing it.

See [the Russian investigation report](../../../../docs/PHYSICS_WHEEL_RAY_V52_2026-10-12.md). The overall discrepancy-elimination goal remains active.
