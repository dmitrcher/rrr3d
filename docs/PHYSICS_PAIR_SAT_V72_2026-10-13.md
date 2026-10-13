# Original box face selection arithmetic — V72, 2026-10-13

Near-equal Z-face overlaps in the original Windows PhysX2.8.4 are not equal after its actual floating-point stores. The previous helper rounded each operation as float32 and chose the opposite incident/reference face. That changed the contact plane and point order before the solver ran. Windows remains the immutable reference; no mass, power, spring/damper, tire, dimensions or friction constant is fitted.

Original x86 box SAT at RVA0x229ee0 uses a mix of SSE float32 and x87 retained intermediates. For each A-face radius, the final `abs(C_i2)+1e-6f` is copied with FSTS, but the x87 stack retains the value before that memory rounding. B-face radii reload the rounded coefficients. In the cold pole fixture A2 overlap is0.06666696071624756 and B2 is0.06666690111160278, so the original selects face6/B2, not A2. Both original and Mac input order is peer first, primary second. Original input cache is0: a stale cached face does not explain the mismatch.

`RRR3D_EXPERIMENT_NATIVE_PAIR_SAT_V72=1` uses explicit float32 stores and retained double intermediates for these face-axis calculations. Original epsilon and cache multiplier0.999 are preserved. It applies only to dynamic vehicle pairs admitted by V71. V71's Z-face fallback remains when V72 is disabled; single-vehicle/static and mesh paths keep their preceding implementation. All experiment flags remain off by default.

The use of double for retained intermediates is validated against the recorded inputs below; it is not a universal emulation proof for arbitrary x87 80-bit inputs. Cross-axis separation and polygon clipping otherwise retain the existing implementation. Full physics equality remains a separate, incomplete goal.

## Identical-input evidence

The read-only Windows debugger captures SAT entry, all six overlaps before cache bias, and selected face, synchronized by complete AFTER_SOLVE journal records. Seven contact fixtures each have two repeats, including cars tilted about their local axes; a pre-contact pole frame36 is retained twice and correctly contains no box collision. All16 captures retain their entire300-state physical history exactly equal to the independently saved Windows Golden. Original EXE/DLL hashes before and after are unchanged. A capture must remove its own previous physical output before starting, because a stale300-line journal would otherwise look like a completed run to the synchronizer.

Python reconstructs all14 six-overlap batches, all six projected distances and all nine absolute rotation coefficients exactly. The actual standalone C++ helper at both O0 and O2 matches all these float32 words, the chosen face, and every contact point, normal and signed separation in the original order, including signed zeros. This fixes the previously rejected unrestricted Z-face geometry through arithmetic, with no forced face index or point reordering.

```sh
python3 source/pair-sat-v72/verify-native-sat.py --out native-sat-proof.json
python3 source/pair-sat-v72/verify-cpp.py --out native-box-cpp-proof.json
```

The second command uses Xcode clang on Mac and the exported standalone header; it needs neither Jolt nor PhysX. `--header` can select a separately installed header.

## Complete-trajectory results

Each focused fixture runs from tick0 for300 steps, fixed dt1/60, seed42, identical scripted inputs, no AI. The two machines in pair tests are deliberately scripted collision partners; random opponents do not participate. Four changed trajectories improve maximum position and velocity error; the other30 pair trajectories are preserved exactly. Disabled V72 preserves the complete V71 focused observations, metadata and contact histories. Both enabled and disabled modes were run twice and repeat exactly.

| Maximum position error, original scene units | V71 | V72 |
| --- | ---: | ---: |
| pair-pole-0 | 0.00127800041013 | 1.44040507387e-09 |
| pair-pole-1 | 0.00128170040895 | 1.44040507387e-09 |
| pair-pole-oblique-0 | 0.26018909816 | 1.25200000001e-06 |
| pair-pole-oblique-1 | 0.260176176252 | 1.34085942597e-06 |

Scene coordinates are not asserted to be metres. Errors cover the entire trajectory, not only final frames. Tiny remaining errors still fail strict float32 equivalence; matching contact geometry does not certify solver preparation or its impulse arithmetic.

Two complete608-case runs each retain255864 states and repeat exactly. 604 complete trajectories, all unrelated fixture metadata and engine descriptions are unchanged from V71; only4 pair trajectories change. Position and velocity maxima each improve4/0worsen; orientation, angular momentum and wheel contact/suspension/normal-force maxima are unchanged in all608. All84 previously exact float32 cases remain exact;524 are still open.

Closed V72 logs and the superseded V71 broad run0 are losslessly compressed: 34 files, 911834985 bytes saved (0.849 GiB), every decompressed logical SHA verified. Current V72 broad run0 stays raw. One-shot compaction scripts must not be rerun.

All291 independent isolation gates pass fresh control/candidate runs twice. All53CTest checks pass. The full arm64 game builds and validates its signature,1200assets,14movies and autonomous dependencies. A full GUI race with the experimental profile has not been verified; the normal installed game is not promoted. The frozen Mac CLI and portable relative profile reproduce all1200 physical states of the four changed trajectories when launched from /tmp.

## Reproduction and remaining work

The two-file guarded installer requires the exact V71 research adapter and its existing SourceBoxManifold header, with the prior V71 SDK unchanged. Main repository sources intentionally remain separate from research snapshots. Every before/after SHA is recorded; all bases are checked before any mutation. Apply, restore, idempotence and rejection of an incorrect last header are verified. Follow the prior research dependency chain and rebuild after installing.

```sh
python3 source/pair-sat-v72/apply-experiment.py --adapter-root /path/to/rrr3d --sdk-root /path/to/existing-jolt
python3 source/pair-sat-v72/run-probe.py --probe mac-probes/v72/rrr3d_physics_calibration --out results.jsonl
```

Use `--cases all` for the608-case matrix. The default four cases test the affected front/rear collisions. Windows VC2008 tracer source/build script is included; it instruments the existing original SpringProbe, never patches the reference DLL file. Neither proprietary DLLs nor duplicate game assets are distributed. Golden SHA256 remains `4bcc66c8f4f28ccb77a07759175573cc56d72035636642617fae4e75addecc1f`; obtain it from the [V59 reference release](https://github.com/dmitrcher/rrr3d/releases/tag/physics-practical-v59-investigation-2026-10-12).

Next: port original paired force-first application and preparation using exact saved Native body/row inputs. The generic Mac pair path still applies normal and friction components separately. Stock-head residual, V70 moving-support wheel angular error, bump normal-force report and other retained mismatches remain open. The earlier V71 rejected-mode2 result remains historical evidence; V72 adds the verified face arithmetic before admitting those contacts.

The overall physics parity goal remains active and incomplete.
