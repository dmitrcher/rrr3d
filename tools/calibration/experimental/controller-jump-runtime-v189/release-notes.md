Adds bounded runtime verification of the original GameCar JumpProgress on actual PhysX actors without starting a VM. The Mac diagnostic exercises the existing V187 fix; production physics and the ordinary game profile are unchanged.

- 530 isolated method/step inputs match twice between actual original method execution under CrossOver and Source GameCar/Jolt.
- The original method and Mac reproduce two saved Windows SDK airborne trajectories,120 steps each, twice. A fresh raw SDK harness also matches three saved controls twice.
- Disabling V187 exposes336/530 differences.
- A temporary precision guard fixes only the owned observer's derived time labels. Before/after physical outputs are unchanged.

On Apple Silicon macOS13+, extract the archive and run `python3 verify.py /absolute/output/directory` from its folder. It checks all file hashes before execution. No game assets or VM are needed.

The530 expected method records are CrossOver compatibility evidence, not new native Windows GameCar gold. Scalar contact flags in that matrix are synthetic. Complete OnFixedStep, real contact transitions, ordinary races, AI/collisions and visual/menu/loading parity remain open. This is an experimental diagnostic, not a replacement game build. Proprietary Windows/SDK files are not distributed.
