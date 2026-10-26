# Clutch integration V186

Original Windows StabilizeForce preserves local Z momentum while the clutch is locked with zero strength. A nonzero request replaces local Z with strength * NxActor::getMass() once, then clears the strength. This happens after conversion to local momentum and before conversion back to world momentum.

The Mac Session previously consumed oil-contact strength immediately and multiplied by nominal mass. V186 carries the one-shot request through the real GameCar command and Session adapter to the stabilizer. Public actor mass comes from the actual stored inverse mass, matching measured Windows float word 1148846079 (999.9999389648438 for nominal 1000). There is no fitted coefficient.

RRR3D_EXPERIMENT_GAMECAR_CLUTCH_V186=1 enables the change; it defaults off. The backend also requires explicit real-game command provenance. An added negative test initially found that injected fields could bypass both boundaries; the corrected implementation passes both controls.

## Verified scope

| Check | Evidence | Interpretation |
| --- | --- | --- |
| Real GameCar -> Jolt world | 720 no-contact inputs, two identical repeats | Exact recorded Windows q/omega and post-integration pose/momentum/position; baseline matched only 360 |
| Production arithmetic header | 2 inertia fixtures x 1440 cases, O0/O2, two repeats | Matches original StabilizeForce; contact booleans are scalar fixtures |
| Persistent GameCar | 26 steps x 720 fixtures, two identical repeats | Body reseeded to recorded inputs every tick; verifies one-shot consumption, not an uninterrupted Windows trajectory |
| Flag and provenance boundaries | 720 inputs per boundary | Injected fields have no effect when flag is off or provenance is absent |
| Actual Session oil path | Application --physics-smoke-test, V186 on | No immediate momentum queue; first callback receives strength, second receives zero; Source expiry tested |
| General physics baseline | 608 scenarios, 255864 states per repeat, two repeats | All float32 physical observables match immutable Windows capture |
| Recorded map regression | 4 tracks x 900 states x 2 repeats | 7200 states match Windows; one car, SDK replay scope |
| CTest | 53 tests, default and V186 environments | Source regression tests |
| Source game event replay | 900 steps, two identical enabled repeats | Functional Session bridge; does not establish full Windows race parity |

All Windows observations in this continuation were already recorded. The VM was not started. The original golden capture is unchanged. No historical versions were deleted.

## Reproduction

Apply install-guarded.py to the documented V185 Source/Jolt base. Every prerequisite is checked before mutation. The installer test verifies six changes, zero on reapplication, and rejection of an unknown prerequisite without mutation. Build the existing CMake project, then run the build/run-world, command-boundary, build/run-lifetime and production-helper scripts. Reference identities are recorded in evidence; large bodies retained locally are indexed separately.

The world probe isolates stabilization: motor, braking, gravity and wheel contact forces are zero, no AI or other cars are present, dt=1/60 and float32 inputs are explicit. The lifetime probe resets the body between steps while retaining the controller. These tests have different scopes.

## Still open

The full physics parity goal remains active. These results do not prove native clutch timer duration, uninterrupted original GameCar::OnFixedStep, real wheel-contact gates, jump behavior, shifted-center/edge cases, ordinary race behavior, AI or visual parity. New Windows runtime observations will not be collected while the user's no-VM constraint applies. Missing observations remain explicit gaps; a Mac-generated reference cannot establish Windows parity.
