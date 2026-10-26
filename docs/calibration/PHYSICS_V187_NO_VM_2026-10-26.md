# V187 checkpoint, 2026-10-26

Experimental correction: original GameCar JumpProgress uses fixed (0,0,-20) acceleration, while the Mac adapter reused scene gravity and nominal mass. The guarded Source/Jolt overlay adds an explicit command marker and mass-independent per-update acceleration. The field is cleared on force/motion reset and serialized with pending forces; whole-project rebuild is required. Pre-V187 Jolt StateRecorder streams are incompatible; no ordinary application consumer was found.

Evidence under tools/calibration/experimental/controller-jump-v187:

- PDB/PE identity and bounded LLVM disassembly corroborate original source semantics. No new Windows GameCar runtime call was made; Ghidra's default import timed out and the session was closed.
- Three recorded SDK trajectories x120 steps, repeated on Mac, match all float32 observables. Zero-scene free fall is an explicit total-acceleration equivalence test, rather than a direct original GameCar runtime capture.
- 360 original-source command gate cases; 51 wake cases repeated with identical physical/energy traces; seven Jolt body masses with two substeps, force/motion reset and state restore.
- Independent review found the pending-sleep and motion-reset bugs. Both received numerical failing controls and fixes.
- Actual Session marker smoke and 900-step deterministic Source event replay pass.
- 608/608 general cases (255864 states per Mac repeat), raw7200 map states and V186 clutch720 per repeat remain exact; default/enabled53 CTest pass.

The diagnostic verifies a bounded correction. The full physics/gameplay/AI/visual goal remains active, and the ordinary installed profile remains unpromoted. The VM was not started, and historic versions were not removed. See the checkpoint README/NEXT for remaining runtime/contact evidence gaps.
