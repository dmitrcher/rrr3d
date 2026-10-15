Research checkpoint: the Mac steering controller now reproduces original Windows body-pose arithmetic and actor inertia storage in six isolated fixture families (270 cases, two identical repeats each).

The fixes preserve original rear-pivot matrix operations and quaternion setter semantics, pass physical steering velocity through a separate channel, and preserve the original actor-axis inertia frame instead of applying a second eigen decomposition. No engine power, mass, tire or spring coefficients were fitted.

Validation: 540 exact steering poses, all 608 baseline physics scenarios (255864 states per repeat, two repeats), 7200 raw map states, and 53 CTest checks. Original Windows game/physics module hashes remain unchanged. Invalid earlier observer attempts are documented and excluded from acceptance.

The attached Mac arm64 diagnostic contains the compiled probe, exact experimental profile, compact Windows measurements, a verifier and dependency licenses. Expand it and run `python3 verify.py --output verification` from its folder. This is a research diagnostic; it does not install or replace the normal game.

Full parity remains open: nonzero stabilization, jump/contact gates, complete original game update and ordinary race acceptance are still required. Experimental flags remain off by default.
