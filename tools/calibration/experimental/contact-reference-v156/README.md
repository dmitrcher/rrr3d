# Concave shared-edge contact axis correction V156

Mac treated a concave shared mesh edge as a convex contact-axis candidate. On original moving map 0 / sample 482 this selected a different reference polygon and one contact where Windows creates three; the complete pair had6 instead of 8 contacts. The old registration removed only coplanar shared edges. V156 additionally removes geometrically concave shared edges from contact-axis candidates. V152 still tests every cross axis for separation. No shape/vehicle coefficients, Windows reference, timestep, commands or random inputs change.

RRR3D_EXPERIMENT_NATIVE_CONCAVE_EDGES_V156=1 enables the new path with the complete V155/V154 research profile. It defaults off. The sign test uses a neighboring triangle's opposite vertex and the native plane, not triangle/map IDs or observed masks.

Actual map 0 / sample 482 now has all 8 original positions/separations, solver quaternions, masses, biases and levers. All40pre-response linear/angular states match; all 900 diagnostic samples equal plain replay. Two complete awake-drive map 0 runs match all 900 original float32 physical states, versus482 initial states previously. Source fixture export equals original fixture and commands remain unchanged, one car/noAI.

Maps 16/48/64 still first differ at420/358/238, exact totals420/374/239. The new flag off preserves all 900 previous V155 map 0 states.53 CTest checks pass. Both frozen V156 repetitions pass all 608 scenarios and all 255864 original float32 physical states, and match each other; see evidence/full608-audit.json. Predecessor155 independently passes both608-scenario / 255864-state gates.

Actual metadata's low 3 flags are active edges, per SDK NxTriangleFlags/NxTriangleMeshShape::getTriangle. On raw 32-triangle geometry the previous algorithm matches 16 original masks; V156 matches 28. Four slightly convex nonplanar joins remain unproved and uncorrected, including the original active-edge angle threshold. No general cooking claim.

The combined four-file overlay includes V153 shape placement, V154 retained friction, V155 revision2 edge cross and V156 concave classification. It accepts exact V152 Source and V150 research SDK plus pinned prerequisite headers, validates every input before any mutation and rebuilds all dependent targets together. Installer 4 then 0 and unknown-prerequisite rejection without mutation are verified. SDK data layout is unchanged.

Complete moving/game parity remains open. The ordinary installed game is unchanged; original GameCar MotorProgress/full player input and rendered race behavior are not proved by SDK-only one-car replays. Investigations on map64 now bind identical ray inputs, vertices, barycentric values and local points; world normal differs by 1 ULP, and raw-cross rotation followed by world normalization is the next candidate.
