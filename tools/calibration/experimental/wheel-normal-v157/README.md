# World-space ray normal V157 — full validation pending

Original Core14e2a4..14e393 rotates a raw triangle cross product and then normalizes the result in world space. The old adapter normalizes locally and then rotates. The operations differ on rounded actor matrices. Rotation accumulates Z,X,Y for X/Z and Y,Z,X for Y; normalization stores XYZ squared length, float length, reciprocal and multiplied components, as Core10001590.

RRR3D_EXPERIMENT_NATIVE_WORLD_RAY_NORMAL_V157=1 selects the corrected operation for registered placed mesh wheel queries, with the complete V156 research profile. It defaults off; other shape paths retain their existing operation. No coefficient, command, timestep, seed or original Windows change.

The actual map 64 / sample 238 ray input, vertices, barycentric coordinates and local point match bitwise. The production helper matches original physical matrix and pre/post normal words in both read-only captures; both real Source wheel normals match, and diagnostic output preserves all 900 states. Query SDK counter 238 corresponds to physical 238; prepared wheel rows use counter 239. Initial query 239 diagnostic was deliberately retained as misaligned evidence, not used to infer a physics change.

The first observer version appended 2 offsets but retained a 26-iteration setup loop. Its pre/post hooks did not run; only matrix-at-entry and original 900-state preservation were valid. The fixed observer derives count from the array, installs 28 hooks and captures the required events; both full histories remain original. See observer-limit.txt and world-normal-proof.json.

Both repeated moving maps 0 and 48 now match all 900 original float32 physical states. Map 16 first differs at 420; map 64 moves 238 to 240. Public outputs alone do not establish complete physics or rendered game parity. The ordinary installed game is unchanged. 53 CTest checks pass; flag-off 900 map0 states equal V156. Frozen full 608 x 2 validation is running, not yet passed.

At map16/420, prepared spring masses/Jacobians/inverse-Jacobians match; Native order is 0,2,3,1 while Source is 3,2,1,0. These observed orders are evidence only, not implementation inputs. Trace general manager/queue/allocation lifecycle before making an order correction. At map 64/240, first body contact has two Source rows; a new original body-contact capture is pending.

The two-file guarded overlay requires exact V156 Source and V154 SDK plus pinned prerequisites; it changes the adapter and adds a helper without changing SDK layout. Rebuild all dependent targets. The full goal remains incomplete.
