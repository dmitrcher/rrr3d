# Per-contact material frames V150

The Mac experimental adapter applied the first triangle's callback material frame to every contact in a mesh manifold. Windows invokes the game callback separately for each contact triangle, even when adjacent triangles share a private collision plane. V150 computes the existing scalar callback arithmetic for each contact's source face and carries its normal, tangents and error frames through Jolt's manifold, material settings and solver preparation. No coefficients or Windows reference binaries change.

`RRR3D_EXPERIMENT_NATIVE_POINT_MATERIAL_V150=1` enables this path on the V149 local-mesh experiment. It defaults off. The SDK extension changes contact data layout: rebuild all Jolt/engine/game/calibration targets together. Do not mix old and new static libraries or promote this experiment to the ordinary installed game.

Actual moving map0 replay now matches every observable float32 physical field through sample357 (358 initial states), including the first body collision at342. Previously the first difference was342. All four point positions, separations, solver quaternions, effective masses, bias terms and lever arms match the original at342. The detailed diagnostics preserve all900 candidate states. Repeated900-step runs on maps0/16/48/64 are identical; first differences are358/244/358/238. Other maps remain open. `live342-proof.json` and `moving-comparison.json` describe the measured scope.

Default-off900 moving states remain exactly V149. All53 CTest checks pass. Both full608-scenario runs match all255864 original Windows float32 physical states and match each other. See evidence/full608-audit.json and full608-manifest.json. Full moving/game parity is not achieved.

`install-guarded.py --source-root SOURCE --jolt-root JOLT` verifies all three overlay files before mutation. Add `--apply` to install onto the exact V149 Source/research SDK hashes in overlay.json. This is not an installer for the ordinary older production tree. The existing V149 local geometry, unresolved convex-part global SAT selection, active-edge/cooking boundaries and remaining moving discrepancies retain their prior limitations.

`capture-native-step358.py` uses the existing read-only V144 observer against the immutable WorldProbeV133 reference. It checks executable/SDK/fixture hashes and preserves the entire original900-state trace. No recorded native results are inputs to the implementation.
