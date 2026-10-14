# Shape placement and retained friction V153/V154

Two independent arithmetic corrections improve moving-map agreement with the immutable Windows PhysX2.8.4 reference. Neither changes vehicle coefficients, commands, fixtures, timestep or random inputs.

V153 fixes the final shape-to-COM translation in Source. The old helper reused the material-error rotation's dotYZX. SDK NxQuat::rot/transform uses dotXYZ. With the new helper, actual map0 sample465 has all four original contact positions/separations, solver quaternions, effective masses, biases and levers exactly. This is supported by the public SDK formula and the actual private465 runtime inputs/output; the old public ShapeOrigin probe is not an oracle for this private cached transform. Other quaternion translation contexts are unchanged.

V154 fixes only the projection of the previous retained friction impulse onto both tangent axes. Original Core1e8215..1e824a sums Z,Y,X; the old SDK path reused point-velocity dotXZY. Current point-velocity projection remains X,Z,Y. Offline evaluation with identical captured inputs reproduces the original response, and all10 actual map16/249 pre-response linear/angular states now match. No tensor, normal, clamp, friction coefficient or inertia changes are included. SDK data layout is unchanged.

Both flags default off:

- RRR3D_EXPERIMENT_NATIVE_SHAPE_CENTER_V153=1
- RRR3D_EXPERIMENT_NATIVE_RETAINED_DOT_V154=1

Use the complete research environment in evidence/full608-manifest.json, including V152 mixed contacts and its prerequisite modes. Isolated flag combinations beyond the documented controls are not supported profiles.

Repeated moving maps0/16/48/64 first differ at478/420/358/238, versus465/249/358/238 in V152. Exact totals are478/420/374/239 out of900; later reconvergence means totals are not exact-prefix lengths. Source153 alone first differs at478/249/358/238. Every replay contains one car, no AI opponents, the same original fixture and commands, and900 samples. These SDK-only replays do not prove ordinary rendered GameCar race behavior.

With both new flags off,900 map0 states remain V152. With only SDK154 off,900 states remain V153. All53 CTest checks pass. Both full608-scenario repetitions match each other and all255864 original float32 physical states; see evidence/full608-audit.json. This does not establish complete moving/game parity.

The guarded installer verifies the exact V152 Source base, V150 research SDK base and prerequisite header hashes before any Source or SDK mutation. It applies three files, is idempotent, and rejects unknown prerequisites without changing either tree. Rebuild the SDK, engine, game and probe together. The older ordinary installed game is unchanged.

Remaining work includes moving-map first differences478/420/358/238, general cooking/convex validation and within-flat traversal, and rendered game integration checks. The goal remains incomplete.

The public SDK shape formula is primary source for V153, independently exercised in cached contact465. Original Core binary disassembly is primary source for retained projection V154. Read-only observer captures preserve both full900-state Windows histories; no recorded Native state is an implementation input. The experimental after/ files contain algorithms, not trace lookup tables.
