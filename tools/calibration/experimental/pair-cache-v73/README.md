# V73 original pair box face persistence

When the two freely rotating cars first touched, Mac recomputed the smallest face overlap each frame. Original Windows passes the previous face into box SAT and biases it by 0.999. At tick39, A0 overlaps by 0.0533032417 and B0 by 0.0533003807; the Windows cached A0 remains selected. Mac without this cache switches to B0, reversing the contact geometry and separation sign.

This adapter-only experiment stores the prior selected face per continuous dynamic pair. Body IDs include their sequence number; a missed step invalidates reuse. It uses the original helper's existing cache priority. No fixture IDs, measured outputs, engine power, mass, tires, springs, skin or shape dimensions are changed. Enable RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_CACHE_V73=1 with the unchanged V72 profile/flags. It is off by default.

Apply the previous research chain through V72, then use apply-experiment.py --adapter-root SOURCE --sdk-root JOLT. All file hashes are checked before writing; --restore restores exactly V72. The V71/V72 SDK remains unchanged. Normal main adapter is not this research base. Rebuild all targets.

Two full runs of 608 scenarios (255864 samples each) match one another. Two stock head-on trajectories improve; all 606 others are unchanged, as are fixture/engine metadata. Position, velocity, orientation and angular-momentum maxima do not worsen in any case; wheels are unchanged. Strict equivalence stays 84 / 608, with 524 open. 291 independent gates pass both modes/two repeats, fresh cache-only focused captures reproduce the candidate exactly, all 53 freshly relinked CTests pass. The game bundle builds/signs with 1200 assets and 14 movies, but gameplay with this experimental profile has not been verified and this is not the installed normal game.

Original Windows probe/DLL/reference remain immutable; all8 warm original captures preserved 300 reference samples exactly. Four original warm SAT/ordered-contact batches are exact in actual C++ at O0/O2. Compile test-native-sat.cpp with -std=c++17 -ffp-contract=off and -I pointing to the V72 after SourceBoxManifold header.

The diagnostics directory preserves rejected paired-solver integration and newly proven preparation modules. Its integrated kernel has small trajectory regressions and is excluded from this candidate. Next: integrate the Native pair material frame and both body tensors on identical inputs, then address moving wheel-support and bump normal-force residuals. No full parity claim is made.
