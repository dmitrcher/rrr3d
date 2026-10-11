# Engine formula / historical parameter audit

The standalone C++98 helper copies the original GameCar five-factor torque
expression and the current port's collapsed efficiency expression. It does
not run GameCar, simulate PhysX/Jolt, test gear progression, or fit power.
Windows VC2008 x86 (/MD /O2) and Mac arm64 (-O2 -ffp-contract=on) each run
72 rows twice. Results identify a formula coverage gap; they do not prove
the arithmetic of every compiled game call site. All eight actuator scales
are one in the current probe and 11 profiles recorded in the game trace.
Old fitted profile remains an explicitly selected legacy experiment.
