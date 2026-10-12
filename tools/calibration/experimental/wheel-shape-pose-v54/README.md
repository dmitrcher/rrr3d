# v54: original private wheel pose and ray endpoints

Default-off investigation on the **exact v53 after-source**, with the v51/v52 prerequisites. Enable `RRR3D_EXPERIMENT_NATIVE_WHEEL_SHAPE_POSE_V54=1` together with the recorded v53 candidate environment. The ordinary adapter remains unchanged in the repository. This checkpoint is not a claim of full physics parity.

The original wheel query composes actor and local wheel transforms, converts the resulting quaternion into a matrix, then constructs and subtracts world-space endpoints before normalizing the ray. Rotating an actor suspension vector directly produces different float32 values. The original actor pose at the first post-contact step is current; the earlier pose-lag hypothesis is not supported by this capture.

Seven capture points, **14 actual Windows runs**: settle counters20/30/60, soft spring38, suspension drop38, longitudinal/lateral slopes25. Tracing preserves each full600-state reference history. Defined fields repeat exactly, excluding pointers, uninitialized buffers and unrelated stack/register words. **35 compositions,84 endpoint getters,28 normalized query rays;1841 float32 output words** match at `-O0` and `-O2`. Duplicate getters are not counted as independent query rays.

At settle tick21 and soft/drop tick39, all four world contact points, normals and suspension lengths now match Windows exactly. The full physical state still first diverges at those same ticks. The shape tangent construction and lateral tire solver remain open. The Mac callback also applies `abs(sideways)` while Windows reports signed lateral slip in the demonstrated case; this is evidence for the next investigation, not an unverified coefficient change.

Fresh control/candidate runs preserve **all291 previously exact physics gates**. The complete broad608-case audit,255864 states per run, still has **65 completely exact /543 open** cases. All608 first divergence ticks remain the same as v53. Both actual Mac repeats match after stripping log timestamps/process metadata. The full signed arm64 Debug game builds and **53/53 CTests** pass; interactive gameplay is not certified here.

Scope: original fixed local wheel quaternion(.5,.5,.5,.5), wheel flags64 (bit0x20 clear), original uncompressed identity static mesh candidate geometry from v52. Original triangle selection, tie-breaking, all alternative wheel flags and tangent/steering processing are not established by this evidence.

```sh
python3 verify-math.py --out /tmp/wheel-pose-v54-math.json
python3 apply-experiment.py --adapter-root /path/to/rrr3d
# rebuild both probes/game in the existing v53-compatible build
python3 apply-experiment.py --adapter-root /path/to/rrr3d --restore
```

The installer rejects any other source base before mutation. Install, repeated install, exact restoration and incompatible-base rejection were checked. The helper requires no SDK headers or original DLLs; neither is distributed in this checkpoint. The experiment changes arithmetic and query geometry, not engine power, mass, spring/damper or tire coefficients. See `physics-v54-corrected-contact-result.json` and `physics-v54-broad-progress.json` for exact limitations.
