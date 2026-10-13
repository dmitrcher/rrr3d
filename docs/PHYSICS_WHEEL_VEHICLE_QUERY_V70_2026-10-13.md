# Peer vehicle suspension queries — V70, 2026-10-13

The Mac suspension ray collector rejected every vehicle body, and the original full-box query helper admitted only static bodies. Original Windows PhysX allows the upper car's suspension to touch the lower car. In `pair-stock-pole`, Windows starts supporting the upper car at AFTER_SOLVE tick 28 while the Mac V69 car continues through free motion until the body collision at tick 37. This is a missing collision-query behavior, unrelated to AI or engine power.

`RRR3D_EXPERIMENT_NATIVE_WHEEL_VEHICLE_QUERY_V70=1` allows the existing full original box raycast on other dynamic vehicle boxes. The existing body filter continues to exclude the querying vehicle. Static box and triangle handling retains its previous path. The change uses original material, mass, dimensions, suspension, tire and engine values. It does not tune parameters to hide the error. The flag remains off by default, and the tested candidate uses the exact V69 profile plus only V70.

| Observable maximum error | Lower car V69 → V70 | Upper car V69 → V70 |
| --- | --- | --- |
| Position, source scene units | 1.09901240 → 0.0000328691 | 1.09948920 → 0.0000238832 |
| Velocity, scene units/s | 2.18617499 → 0.0000240725 | 2.18617415 → 0.0000226821 |
| Orientation, degrees | 0.08490747 → 0.07968013 | 0.08547097 → 0.07928298 |
| Angular momentum | 0.28401417 → 0.38230505 | 0.28631149 → 0.38531669 |
| Missed wheel contact samples | 0 → 0 | 124 → 0 |
| Suspension distance | 0 → 0 | 0.499282449 → 0.000389099 |

The position and velocity defect is strongly reduced, but angular momentum remains an explicit regression and orientation is not fully matched. The dynamic-body suspension solver uses a different path from the retained Native static-body scalar prototype. This checkpoint is a default-off investigation, not certification of complete physical equivalence or promotion of the normal installed game.

Two complete 608-case runs each retain 255864 states; all physical observations, metadata and engine descriptions repeat exactly. Only the two stock-pole trajectories change; the other 606 trajectories and all 84 previously float32-exact cases remain unchanged.524 cases remain open. All 291 independent gates pass in both control and candidate with two repeats; all 53 CTest checks pass. The full arm64 app builds and verifies its signature, 1200 assets, 14 movies and autonomous dependencies. A GUI race with the experimental profile was not verified.

The 34 pair trajectories use fixed scripted commands, no AI, no stochastic opponent behavior. Most other cases contain one car. The fixed profile uses seed 42 and dt 1/60. Full-run metrics cover entire trajectories from tick0; the stock-pole diagnosis isolates the first divergence at tick 28 rather than comparing already divergent later states. Original Windows executable and DLL hashes remain unchanged, and every new debugger capture matches all 300 original physical samples exactly.

Private trace alignment erratum: the initial two-body counter mapping was incorrect. At wheel counter55, both prepared body velocities match the original Golden AFTER_SOLVE tick29 exactly in float32, so this capture must not be called the first contact at tick28. The directory named `native-corrected` contains valid read-only later-state captures, but its original counter-to-output-tick description is retracted. The first exploratory `native` captures also miss the intended onset and remain excluded. A follow-up at wheel counter51 has the initial upper/lower velocities0/4; contact captures at71 still expose already altered body velocities. Full contact phase/tick alignment remains unresolved and is not certified by simply counting actors. `solver-iteration-entry` snapshots must not be used as a proved dynamic pair transition model. The numerical first-contact onset at tick28 is established independently by the original Golden and full Mac output samples, and all V70 physics/regression results remain unchanged. The next audit must identify the actual changing pair solver state and dynamic support preparation/iteration order. See `physics-v70-native-alignment-errata.json` and the supplemental capture archive.

Full source snapshots and guarded apply/restore are supplied. Installation validates all bases before mutation; repeated apply/restore and rejection of a different base are checked. The normal repository adapter is retained; the research adapter lives in `after/adapter`. The base is the exact V69 research adapter and existing V69 SDK, not an arbitrary clean Jolt checkout.

The frozen CLI can be run with the included relative profile:

```sh
python3 source/wheel-vehicle-query-v70/run-probe.py --probe mac-probes/v70/rrr3d_physics_calibration --out results.jsonl
```

The default four pair trajectories were rerun through this portable launcher and all 1200 physical samples match the broad candidate exactly. Use `--cases all` for the complete matrix. The full original Golden is referenced by SHA256 rather than duplicated; the release does not include proprietary PhysX DLLs or game assets.

The physics goal is active and incomplete. Next priorities are the active dynamic/dynamic body solver, pair contact ordering/material coefficients, and Native preparation/solve of suspension on a moving support; the bump force-report discrepancy 454.33316 also remains.
