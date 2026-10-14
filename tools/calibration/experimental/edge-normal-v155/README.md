# Unfused edge-plane cross V155 revision2 — validation pending

Original Core226d5a..226dae rounds scalar multiplications and subtractions separately before edge-plane normalization. The prior generic PxVec3::cross expression can contract to ARM fused operations. Offline actual map0/478 inputs reproduce the exact Native normal with separate rounding and the old Source normal with fused arithmetic.

The candidate changes only the native edge-plane cross. RRR3D_EXPERIMENT_NATIVE_EDGE_CROSS_V155=1 enables it with the complete V154 research environment; it defaults off. The original generic expression executes before guarded overwrite, preserving baseline compiler behavior. Revision1 used a conditional expression and failed the corrected default-off control; it was rejected. A separate first control also omitted V154's retained flag and is recorded as invalid.

Revision2 default-off900 states equal V154. All53 CTest checks pass. Both moving-map repetitions first differ at482/420/358/238, versus478/420/358/238. All20 actual478 rows match original geometry, mass, bias, levers, quaternions, linear and angular states; all900 diagnostic states equal plain replay. No coefficient or original Windows changes.

Full608x2 runs of frozen155 are still in progress; this folder is an investigation snapshot, not a validated release. See NEXT.md for live job handles. Ordinary installed game unchanged; complete moving and rendered game parity is not achieved.

At next map0/482, Source produces6 contacts versus8 in Windows. Five exact point/separation tuples are shared; Source's first triangle12 contact differs and Windows has3contacts for cookedface22 instead. Inspect SAT axis/reference-polygon selection before changing force response. Map16/420 has zero body-contact rows on both sides; wheel/tire/integration remains to inspect.

Guarded installer requires exact V154 Source/SDK prerequisites, applies one header and preserves SDK layout. All after/ files and overlay hashes are revision2.
