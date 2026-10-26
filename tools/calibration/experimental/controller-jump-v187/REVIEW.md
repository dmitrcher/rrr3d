# Independent bounded review and controls

Reviewer inspected original sources, actual proposed snapshots and test scopes. Two actionable findings were reproduced before correction:

- P1: a pending native-energy sleep could override Jolt activation. wake_tick24 was the only failing wake case; already-inactive ticks25..50 were covered too. The corrected path calls automatic wakeEnergyV128 before activation.
- P2: ResetMotion directly cleared old fields while retaining the new acceleration. A real Jolt integration after ResetMotion reproduced unwanted velocity. ResetMotion now clears the added field.

Reviewer rechecked both final fixes and overlay hashes, found no further important lifecycle issue in this bounded scope, and retained the StateRecorder compatibility and missing native runtime caveats. Final numerical and regression reruns are the acceptance gate.

Initial harness-only crash: JobSystemSingleThreaded's default constructor does not Init its free list. macOS crash evidence pointed to FixedSizeFreeList::GetStorage from CreateJob; jobs(2048) resolved this. It was not used as production-failure evidence. The owned stalled debugger and helper were terminated; no user application/VM was controlled.
