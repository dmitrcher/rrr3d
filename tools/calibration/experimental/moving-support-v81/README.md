# V81: original longitudinal slip subtraction, offline proof

The prior Mac adapter computes rolling minus forward velocity. Original PhysXCore RVA14b72e..14b78b computes forward minus rolling, normalized by max(abs(rolling),1). These opposite signs were previously hidden by zero-slip controls. The first remaining V80 difference at stock-pole1 physical sample29 is +9.09494702e-13 Windows versus -9.09494702e-13 Mac.

The helper reproduces original radius*axle, negation, raw addition and normalization using separate float32 stores. Forty independent input/output records from five conditions, repeated twice, match120 raw words at both O0/O2. Conditions include moving support at29/32, forward drive, reverse drive and braking. Ten captures preserve4800 original physical states and original EXE/DLL/tracer hashes. Captures are read-only debugger evidence. No recorded values are consumed by live Mac physics.

This correction is NOT integrated. Active physics remains verified V80. The remaining tiny body differences from sample32 must be investigated separately, without changing masses, powers or tolerances. verify-math.py compiles the independent helper against fixture words from any working directory; Xcode Command Line Tools are required. workspace-capture-native-slip.py retains local paths and is an audit reproduction record.
