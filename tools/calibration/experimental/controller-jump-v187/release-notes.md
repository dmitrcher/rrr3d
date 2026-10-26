Experimental Mac arm64 diagnostic for the original GameCar JumpProgress fixed acceleration. Run `python3 verify.py --output verification` in the extracted folder.

Verified: three recorded Windows SDK trajectories (120 steps each, two Mac repeats), 360 original-source command gates and seven Jolt body masses/lifecycles. The 608-case baseline, 7200 map states and prior 720 clutch cases remain exact. Pending-sleep and motion-reset failures were reproduced and corrected.

The zero-gravity-scene check is total-acceleration equivalence. Native GameCar JumpProgress runtime, real contact transitions and full race/AI/visual parity remain open. The Windows VM was not started. This diagnostic leaves the ordinary installed game profile unpromoted. Pre-V187 Jolt StateRecorder streams are incompatible with this experimental build.

Source and scoped evidence: https://github.com/dmitrcher/rrr3d/tree/e2c8c228c45e04c58bc99ba83d0b71747019e35e/tools/calibration/experimental/controller-jump-v187
