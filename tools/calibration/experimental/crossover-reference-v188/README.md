# V188: bounded no-VM Windows-probe qualification

This checkpoint changes the test infrastructure and records evidence. It does not change Mac physics or promote the experimental game profile. Parallels is not started. The complete physics parity goal remains open.

CrossOver 26.3.0 runs owned, unchanged Windows observers and original PhysX/Game modules in a dedicated task prefix. Existing user bottles stay separate. Microsoft/NVIDIA installers, binaries, DLLs, headers, PDBs and the prefix are local dependencies and are not published.

## Results

| Control | Exact result | Interpretation |
| --- | --- | --- |
| Original V14 SDK observer, known 608 scenarios | 255864 states per repeat, two identical physical repeats, all 608 exact | This specific recorded SDK matrix can run without the VM. It does not certify new scenarios automatically. |
| Original V133 map0 observer with native D3DX | 900/900 states, twice, metadata equal | Same one-car fixture, original startup/control setter and raw actuators. No AI, complete GameCar fixed step or rendered race. |
| V133 maps16/48/64 with native D3DX | 0/900 per map, twice | First mismatch is initial quaternion, before physics. Reject these runs as Windows-equivalent references. |
| Original StabilizeForce, four free wheels | 885/1440 physical cases | Same recorded incoming pose/velocity/mass/inertia/control word. Runtime is not equivalent for this method. |
| No-active-hook box control | 841/1440 physical cases | Breakpoint removal changes 0/1440 physical outputs within CrossOver. Native release CRT also changes none. |
| PC53 diagnostic after actor creation | 789/1440 outputs change from PC24; 642/1440 match Windows PC24 | Precision controls do affect execution; changing precision is not a parity fix. |

The original map0 mismatch disappeared after loading the already-owned native D3DX9_43.dll. The other maps still differ, so the startup route is not universally qualified. Exact initial fixtures must be established before solver differences are interpreted.

## Stabilization arithmetic finding

The original module's matrix-inverse checkpoints provide 1380 active-branch records. All incoming public states and all nine matrix cofactors agree. CrossOver matches rounding after each addition in 1380/1380 determinant calculations. The saved Windows run matches retaining the register sums until the final float store in 1380/1380; the two models disagree in 690 cases. This is a bounded instruction-sequence replay against actual checkpoints. It does not prove a particular processor or translator defect.

The existing V185 Mac helper already follows the saved Windows arithmetic. Do not replace it with the CrossOver rounding sequence. Original production PC24 provenance is independently supported by the saved V172 real-game observations; see FP-PROVENANCE.md.

## Exact comparison and rejected work

Map traces contain decimal physical numbers and are compared as IEEE float32. Stabilization traces contain uint32 bit words and must be compared as integers. A new temporary comparison mistakenly converted word integers to float and reported 1308/1440; that result was invalidated and replaced by exact integer comparison (885/1440). compare-stabilization.py preserves a regression assertion for this failure mode. Golden recordings and Source physics were not changed.

The first native-release-CRT attempt did not actually load native release CRT: its report records that failure. Later sidecar-manifest controls verify native release and debug CRT in loader logs and still do not close the physical difference. Padding controls modify only the owned observer, not the original game/SDK binary; their historical embedded source stamp is superseded by the explicit derived-binary patch receipt.

## Reproduce and review

- run-sdk-broad.py and compare-sdk-broad.py reproduce/compare the original known608. Large full traces remain local; their hashes are indexed for publication.
- run-map-suite.py runs four preserved fixtures twice and intentionally exits nonzero when any physical or startup comparison fails. Its report preserves both accepted and rejected results. It compares decimal map fields only, not integer-word traces.
- compare-stabilization.py compares physical uint32 words directly. Checkpoint locals are diagnostic, not counted as physical acceptance.
- audit-determinant.py replays only the recorded determinant instruction sequence; no future state or per-case corrections are fed into the game.
- preserve-controls.py losslessly archives this task's closed captures/logs and records hashes. It does not delete historical work.

Native dependencies were extracted from already-owned media. The extracted original MSVC15.00.30729.01 and SDK6.0A successfully built a fresh x86 STL/CRT smoke program, then that program ran with native MSVCR90 and expected version/pointer-size/vector output. This qualifies compiler activation in the task prefix, not physics. The initial smoke staging omitted the language-resource subdirectory; after copying the already-extracted VC/bin/1033 files, compilation succeeded. All original installer/media contents remain unchanged. The owned read-only ISO mount was detached after extraction.
