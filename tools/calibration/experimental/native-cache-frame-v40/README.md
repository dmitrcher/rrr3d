# Persistent raw world cache frame v40

The two-file patch applies only to the SHA-verified v39 experimental adapter
and NativeContactCache header. It does not apply directly to production.
Before/after sources and SHA values are included beside the patch manifest.
Enable RRR3D_EXPERIMENT_NATIVE_CACHE_FRAME=1 with the entire v39 chain.
run_isolation.py --contact-v40-cache-frame enables the complete chain.

The candidate persists the raw world frame used by point-cache transverse
errors. It keeps the reconstructed callback/material frame separate.
All physical tuning scales remain 1. No recorded values are fed into the Mac
implementation. Debug capture remains independently simulated.

72 long cases × 120 steps × two repeats: 18 exact, 54 remaining, two additional
exact cases and no lost exact cases. Partial cases still include numerical
worsenings: inspect the complete report rather than assuming a universal gain.
189 isolation cases and 53 project checks pass. Four detailed cases have
360 exact solver rows across steps 0–2. Local cached normal differences in
p7 remain explicitly visible, even though its complete physical trace is exact.

capture-mac.py records candidate and disabled controls. The output filenames
retain the v39 schema so compare-solver.py can also verify existing archives;
the capture manifest identifies the v40 candidate and active environment.
verify.py checks force isolation, actual profiles, repeatability, controls,
complete states, manifests and solver rows. verify-cache.py checks actual
Mac cache inputs against independently recorded original Native raw points.

Production remains default-off. Full-game engine/wheel boundary and broader
suspension, tires and AI checks are not proved by these isolated body contacts.
