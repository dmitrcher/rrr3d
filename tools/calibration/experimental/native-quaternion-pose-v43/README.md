# Native quaternion pose v43 (experimental)

Original PhysX composes COM/shape translations with quaternion-vector arithmetic,
which differs from Jolt unit-quaternion matrices when float quaternion norm is
not exactly one. Reproduce initial COM, actor origin and public actor output.
Compose shape-to-COM locally and transform once: two world translations give
another rounding path. Preserve integrated COM exactly after frozen restoration.
The original normal-response mass projection also sums Z,X,Y (RVA 0x84633).

Enable `run_isolation.py --contact-v43-quaternion-pose`, with the complete v42
prerequisite chain. Six new flags (value 1), prefixed RRR3D_EXPERIMENT_NATIVE_:
INITIAL_COM, ACTOR_ORIGIN, ACTOR_OUTPUT, SHAPE_TRANSLATION, MASS_DOT_ORDER,
FROZEN_CM_EXACT. They default off; production is not promoted.

The three-file patch and base hashes are in ../native-quaternion-pose-v43.patch
and ../native-quaternion-pose-v43-base.json. Adapter baseline is exact v42;
SDK baseline is the existing experimental NativeContactArithmetic.h. Its path
Jolt/Physics/Constraints/ConstraintPart is relative to the Jolt source root,
while src/Rock3dEngine paths are relative to the game root. Full files and exact
before files are included. Additional physics headers are formula-helper build
dependencies only. `verify-patch.py --out RESULT.json` reconstructs all three.

`verify.py --measurements DIR --probe EXE` checks 72 old contacts, 189 isolation
scenarios, disabled v42 controls, repeated states, effective coefficient scales
and detailed old solver rows. `verify-cache.py` checks their actual cache data.
`verify-expanded.py` checks all six new 120-step COM/shape fixtures and exact COM
restoration. `verify-expanded-rows.py` and `verify-expanded-cache.py` check four
new detailed fixtures at steps 0–2. `verify-ablation.py` checks the six cumulative
stages on the final binary, including all repeated physics diagnostics.

Build formula-probe.cpp with clang++ -std=c++17 -O2 -ffp-contract=on and the
candidate physics header include path. `verify-native.py --measurements DIR
--probe EXE` reconstructs 896 original shape-to-COM translations, 2048 original
normal masses and eight initial COM values exactly. Native physical states must
remain equal to the immutable expanded Windows references. Recorded inputs
are used only in the verifier; implementation loads no recorded states.

Windows tools reuse the exact v42 reference fixture probe and read-only geometry
tracer. Source/build.cmd are included. Use VC2008 x86 and the original SDK/CRT.
Original immutable SpringProbe SHA256:
8c331b7ea35ed6181c0f8dd52cddca451f60c817d381d360cb53852e8c49ed9d.
Separate expanded SpringProbe:
d905fe99f4a835917562b3e4dd9c9b0fdf513ab8ebf75ad5750679462c44dbef.
PhysXCore.dll:
c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb.
Geometry tracer:
28b488858ba87cfe4bc468a3bbd1082217c1a75d8ca639f5ead3ff4321695180.
`capture-geometry-windows.py --case pose-v42-frozen-N` captures the selected
expanded fixture twice and checks original physical states. Pass the expanded
reference and guest folder C:\MotorRock\calibration-probes\contact-pose-v42.
`capture-expanded-windows.py` checks the 72 old references and captures six new
ones. Neither tool replaces the game or the immutable original probe.

44/72 old contacts exact; 28 remain, with some partial regressions.
6/6 new COM/shape contacts exact in two repeats; 189 isolation scenarios exact.
The audit package is not a game installer or evidence of full game/AI parity.
See docs/PHYSICS_CONTACT_V43_2026-10-12.md for measured scope and limits.
