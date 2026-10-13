# MotorRock physics V73: persistent box face

Windows remains the unchanged reference. Controlled two-body contact fixtures contain no AI or randomized commands.

The recovered original contact cache passes the previous face into SAT and uses its 0.999 priority. Preserving that face on Mac reduces the primary stock head-on maximum position error from 0.0030017038 to 0.0000053868 original scene units (~557x), and the orientation error from 0.23765756 to 0.00666170 degrees (~36x). Peer improves too. No power or material tuning was introduced.

Both full 608-case repeats are identical, with255864 states each. Exactly 606 trajectories remain unchanged and both stock head-on trajectories improve; no measured maximum regresses. Strict float32 equivalence is still84 / 608, leaving524 open. 291 independent gates and 53 fresh CTests pass. The signed arm64 game bundle builds; experimental full-game race validation is pending.

[Source, guarded installer and proof summary](../tools/calibration/experimental/pair-cache-v73/README.md). The candidate environment enables only the new cache on top of V72. The separately saved solver integration remains rejected due to regressions. Its scalar kernel is proven 190 / 190 on original inputs; current stock warm inputs differ in frame, levers and tensors. Existing Native preparation formulas reproduce14 solver quaternions and28 tensors on actual original inputs at O0/O2. Live integration of these formulas is the next step.
