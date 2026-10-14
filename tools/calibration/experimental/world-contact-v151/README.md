# Converter and world contact output V151

The previous experimental adapter used a different float32 association when converting the raw contact frame to actor-local coordinates. It also transformed body corner contacts through body-to-mesh-to-world, while the original polygon wrapper uses the original body world matrix directly. These two independent errors caused the first map0 difference at358.

`RRR3D_EXPERIMENT_NATIVE_CONVERTER_FRAME_V151=1` selects the converter's cross-first vector products and Z,Y,X scalar dot. Other quaternion products retain their distinct arithmetic. `RRR3D_EXPERIMENT_NATIVE_WORLD_CONTACT_V151=1` passes the original world box matrix only for output positions from that box; clipping keeps its relative transforms and normals. Both flags default off. No engine, suspension or friction coefficients change, and the Windows reference remains immutable.

On the actual moving replay, all four point positions, separations, solver quaternions, effective masses, biases and levers match Windows at358. Running each flag independently isolates its own cause; enabling both gives full physical-state equality at358. See live358-proof.json. Detailed diagnostics preserve all900 candidate states.

Repeated moving maps0/16/48/64 first differ at359/249/358/238, versus358/244/358/238 in V150. The corresponding exact-state totals are359/412/374/239 out of900 per map; these totals include later reconvergence and must not be confused with the initial exact prefix. Full moving/game parity remains open. Default-off900 map0 states remain exactly V150. All53 CTest checks pass. Full608-scenario repeated regression status is recorded separately.

The next map0 cause at359 is separate: the original generates five points from two convex parts/normals. The current adapter rejects mixed normals and falls back to generic Jolt contacts. Source's current triangle SAT skips separating tests on inactive edges, admitting raw triangle4 that the original excludes. Offline all-axis SAT produces exactly the original candidate set, raw5/30/31. Candidate filtering, part order, per-contact raw normals/cache frames and suppression of duplicate pair overrides need a general implementation; simply deleting the mixed-normal check is not sufficient.

The guarded three-file installer accepts only V150 Source and checks the unchanged V150 research SDK hashes. Rebuild engine, game and calibration targets after applying. This is an experimental overlay, not an installer for the older ordinary production tree. The ordinary installed game is unchanged.
