# Pair box contact and static friction investigation — V71, 2026-10-13

The Mac source-box manifold override explicitly rejected two dynamic vehicles. Their collision therefore used inset Jolt geometry, a different four-point reduction and the generic Jolt iteration path. Original Windows PhysX uses the complete source box, its original contact points and a different contact iteration sequence. This defect starts at the first collision, with identical input poses/velocities, rather than engine power or AI behavior.

`RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_MANIFOLD_V71=1` admits dynamic vehicle pairs into the existing source box manifold and block-PGS path for the independently tested side-face axes. It permits up to64 existing SDK source points; the vertical weak-peer fixture actually has8 Native contacts, previously rejected by the old four-contact guard. No mass, power, tire, dimensions, spring/damper or angular limit is tuned.

`RRR3D_EXPERIMENT_NATIVE_PAIR_STATIC_FRICTION_V71=1` preserves independent static and dynamic material coefficients. Native side contacts have static U/V0.08/1.64 and dynamic0.08/1; the isotropic pole has static0.86/0.86 and dynamic0.54/0.54. Exceeding the static limit triggers the dynamic clamp; passing the static test retains the target. The tested profile is the exact V70 baseline plus both V71 flags. Both flags remain off by default.

| Maximum observable error against unchanged Windows | V70 | Guarded V71 |
| --- | ---: | ---: |
| Same-head primary position, scene units | 0.045774623 | 0.0000007773 |
| Same-head primary velocity, scene units/s | 0.010629338 | 0.00000119003 |
| Stock-head primary position | 0.412060839 | 0.003001704 |
| Stock-head primary orientation, degrees | 48.615892 | 0.237658 |
| Stock-oblique peer position | 0.192579804 | 0.000000015 |
| Stock-oblique peer orientation, degrees | 61.557488 | 0.000045148 |
| Stock weak-peer primary position | 0.156214773 | 0 |
| Stock weak-peer primary orientation, degrees | 48.531772 | 0.000029913 |
| Vertical weak-peer primary position | 0.188162025 | 0.000000060 |

These are errors over entire trajectories from tick0, not only final positions, and scene units are not asserted to be metres. Zero position error does not mean every velocity, rotation or wheel field is float32 exact.

Two complete608-case runs each retain255864 states. Physical observations, fixture metadata and engine descriptions repeat exactly.578 trajectories are completely unchanged from V70; only30 pair trajectories change. Position maxima improve30/0worsen, velocity28/0, orientation8/0 and angular momentum8/0. Wheel contact, suspension distance and normal-force error maxima are unchanged in all608. All84 previously exact float32 cases remain exact;524 are still open. All291 independent gates pass with two repeats in both control and candidate; all53CTest checks pass. The full arm64 app builds, verifies its signature,1200assets,14movies and autonomous dependencies. A full GUI race with this experimental profile has not been verified, and this is not promotion of the normal installed game.

## Authoritative first-step diagnostics

Previous V70 private trace counter-to-tick claims were retracted. The private integrator counter counts active body integrations, so sleeping peers make a fixed conversion unreliable. V71 tracers incrementally read complete `calibration.sample` lines from the original probe journal, which closes each record before the next step. The number of completed AFTER_SOLVE samples is the current solver tick. The original Windows probe and PhysX DLL are unchanged.

`MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71=38` captures the first head collision with Native bodies at velocities0/8. Its internal actor counter is61, not the previously guessed71/75. The peer-wheel first contact is physical tick28 with actor counter51 and velocities0/4. All five contact scenarios and the wheel scenario were captured twice; each full300-state physical history remains exactly equal to the original Windows Golden. EXE/DLL hashes before and after match.

The supplied standalone scalar reconstruction resets both bodies and the contact row from each saved Native input. It compares only immediate adjacent rows within the same contact group, excluding every group boundary.190/190 transitions match all12 float32 body velocity/spin words exactly. Substituting a dynamic-only threshold reproduces184/190, directly exposing six incorrect transitions on identical inputs. This proves the reconstructed Native calculation on Native inputs; it is not a claim that Mac preparation/iteration is fully identical.

Run the proof without either game or SDK:

```sh
python3 source/pair-contact-v71/verify-pair-contact-transitions.py --out pair-proof.json
```

## Rejected variant and remaining work

The unrestricted source-box candidate improves most side scenarios but worsens pole-oblique position error0.261 ->1.456. Native uses Z15.6999998 for the original contact plane; the PhysX3.4 helper chooses the opposite incident/reference plane at15.7666645 and reverses the X contact order. The guarded mode1 retains the preceding path for A2/B2 source face axes instead of certifying this unverified geometry. Side contacts encountered later in a pole trajectory can still use the verified path.

Mode2 is the explicitly rejected unrestricted diagnostic candidate (`RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_MANIFOLD_V71=2`), with separate retained measurements. Its original pole/head/eight-contact candidate histories were reproduced exactly for900 states after adding the mode1 guard. Do not deploy mode2 as a correction. The scalar proof is not an excuse to tune coefficients to cancel a geometry/ordering error.

Next: recover the original Z-face reference/incident selection, then port paired force-first arithmetic and preparation using identical Native inputs. Original Native combines contact and friction impulse before applying it to each body; the current generic pair path applies normal and friction components separately. The remaining stock-head error, moving-support suspension angular error and bump normal-force report454.33316 remain explicitly open. No AI is used in the fixtures: fixed commands, fixed dt1/60, seed42, one vehicle for most cases and two scripted vehicles for pair cases.

## Sources and binaries

The four-file guarded installer requires the exact V70 research adapter and existing V69 SDK checkpoint, not the normal main adapter or a clean Jolt tree. All bases are validated before any mutation; apply/restore, idempotence and wrong-last-file rejection are verified. Research snapshots remain separate from the repository's normal adapter. Rebuild after installation.

The frozen Mac CLI uses a bundled relative profile:

```sh
python3 source/pair-contact-v71/run-probe.py --probe mac-probes/v71/rrr3d_physics_calibration --out results.jsonl
```

The default four-case launcher was verified against1200 physical observations from the broad/focused candidate; `--cases all` runs608. Windows frame-tracer C++ sources, VC2008 build script and tools are included; they debug the existing original Windows probe, not a modified PhysX DLL. Binaries are release assets rather than source-control files. Original Golden SHA256: `4bcc66c8f4f28ccb77a07759175573cc56d72035636642617fae4e75addecc1f`, retained in the [V59 reference release](https://github.com/dmitrcher/rrr3d/releases/tag/physics-practical-v59-investigation-2026-10-12). Neither proprietary DLLs nor duplicate game assets are included.

The full physics parity goal remains active and incomplete.
