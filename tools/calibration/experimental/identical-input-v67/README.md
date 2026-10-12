Read-only preparation telemetry and verification on IDENTICAL saved original Windows inputs. It is not a physics correction. RRR3D_WHEEL_PREP_TRACE_FILE_V67 plus RRR3D_WHEEL_PREP_TRACE_STEP_V67 records at most four wheels at one requested vehicle counter. Inactive rows expose no uninitialized solver storage.

Run `python3 verify-math.py` in this directory. No game, Jolt installation, proprietary SDK, or capture download is required. Embedded original float32 fixtures verify 262 inertia/mass words, 420 frame/Jacobian words, and 240 hard-stop solver words at -O0 and -O2 with FMA disabled. The generic grouped impulse arithmetic differs on 6/80 identical hard-stop solves; its geometric bias is approximated from the original raw bias, so this does not certify actual Jolt preparation inputs.

Native counter61 equals Mac wheel counter63. Native spring-solve-result occurs BEFORE body velocity application; only its row impulse fields are post-solve. The Native local cache must be used as retained: current world-to-local inversion of a stale anchor is not a valid equivalence test.

V64 center and rear world contact points at that counter match original float32. V66 wheel2 point differs by about 0.000102 source scene units; comparing masses on those different inputs never proved an inertia formula error.

Forty immediate contact transitions also match Native exactly when each contact starts from its own saved body input. Group boundaries are excluded because wheel constraints run between contact groups. No claim of complete parity or full-game validation follows from these subsets.

The guarded installer applies only the recorded V66 source base; source snapshots are all validated before mutation. New flags are off by default.
