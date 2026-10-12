Default-off hard suspension stop frame/inertia diagnostic. RRR3D_EXPERIMENT_NATIVE_HARD_STOP_FRAME_V66=1 applies the prior Native wheel frame and scalar inertia operations to hard stops, which previously used generic Jolt preparation. This hypothesis is NOT accepted as a correction: at the coupled border solve the two effective masses still fail to match Native, and the second mass error grows.

17 focused control/candidate scenarios, two repeats each: xCar/suspension-drop position maximum improves 0.00000821064 -> 0.000000135932, but podushka/engine2 worsens 0.00106010 -> 0.00219045 and other trajectories regress. Native mass comparisons are retained without claiming equivalence. No full 608-case matrix was run for this candidate.

All 291 independent float32 gates pass in both modes, two repeats; all 53 build checks pass. Those checks do not certify the unsupported coupled hard-stop preparation. Ordinary game promotion remains withheld; Native parameters are not fitted or changed.

The initial free helper failed compilation because Wheel state is protected. It was corrected to a lambda within VehicleConstraint before any physical run. The first gate attempt failed because the older retained baseline had been compressed; no passing result is claimed for that failed attempt. The repaired comparator reads .gz directly and hashes decoded original capture bytes. The subsequent complete gate run is the retained verification.

The mass captures use slightly diverged body poses, so their difference alone does not prove an arithmetic or anchor defect. Next work must reconstruct the original hard-stop preparation on identical Native inputs, separating carried state error from a stop-specific defect. All experiments remain off by default.
