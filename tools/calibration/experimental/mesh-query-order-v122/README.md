# Source mesh query order V122

The accepted isolated matrix matches607 of608 complete Windows trajectories, up from604. The600-step Marauder suspension drop, Dirtdevil suspension drop and xCar wheel drive now match completely. One body-contact fixture remains open; whole-game and rendering parity are not certified.

At Marauder step146, Jolt chose triangle1 at the shared ground edge while the original scalar query accepted triangle0. Existing productionV52 intersection arithmetic reproduces the actual original barycentrics and point from identical ray/vertex words. Choosing triangles before that arithmetic gave a different contact point and changed spring/tire lever arms. Original direct mesh traversal keeps the first hit on equal distances (RVA1f28ef..1f290d).

Default-off `RRR3D_EXPERIMENT_NATIVE_MESH_QUERY_ORDER_V122` scans retained source triangles for static identity mesh candidates, uses original scalar intersections and strict nearest-hit replacement, and retains the actual selected Jolt subshape ID for normal/material lookup. No coefficients, state injection or case/frame/wheel rules are used. SDK ID enumeration is an additive geometry identity helper.

This is a full scalar scan experiment; performance on large meshes and the original large-mesh BVH tie order still require validation. It retains Jolt candidate-body discovery, so it does not certify situations where Jolt rejects every triangle of a body that the original query would accept. The ordinary installed game is unchanged.

Apply or restore with `apply-experiment.py --adapter-root PATH --sdk-root PATH [--restore]`, then rebuild. All source identities are checked before mutations. `verify.py --probe PATH` reruns complete original trajectories and flag-off controls. Arithmetic fixtures check16 actual immutable original ray records at O0/O2. Live captures verify16 selected triangle IDs/vertices, prepared rows and480 spring/tire incoming/result rows. Diagnostic signed zeros are recorded separately; no raw ABI equality is claimed.

Validation:both255,864-state matrix repeats are identical; all604 prior strict trajectories remain exact,605 complete histories unchanged,three new600-step model histories match. No raw/canonical maximum worsens. All53 project checks pass in each profile,291 independent gates pass,solo600 and rendered240 functional checks pass. Portable verification reruns12,180 original-equivalent states and1,800 flag-off V121 controls.
