# Native contact lifetime V89 investigation

This contains additional read-only Windows observations, not a new physics fix or a game build. The accepted Mac source/probes remain V88: 242/608 complete float32 trajectories exact, 366 open. Research switches remain default-off. The normal installed game is unchanged.

For one fixed solo `mobility/devildriver/engine3/lateral` scenario, 18 original Windows observer captures preserve all 10,800 immutable Golden states. Original SpringProbe.exe and PhysXCore.dll hashes are checked before and after every capture batch. PhysX calculations and car parameters are unchanged.

Two independent normalized lifecycle captures agree. Wheel contacts initially dispatch 0,1,2,3. The queue is empty at physical ticks 38–43. At 44 four contact managers and four springs are created with zero prior spring handles; the wheel queue dispatches 3,2,1,0. The Windows solver then traverses springs 0,1,2,3, while V88 Mac still traverses 3,2,1,0. The separate retained Native/Mac prepared-row proof matches all 28 corresponding mass/Jacobian/inverse-Jacobian float32 words.

The lifecycle observer captures authoritative physical ticks 35–45 in one process, avoiding a separate full race for each observed tick. Selection of a diagnostic window affects logging only. No frame number, fixture name, recorded Native state or wheel permutation has been introduced into game physics.

The generic remove breakpoint at RVA91310 was not reached; the actual cleanup path, static mesh pair publication order and allocator/node recycling still require source validation. Recreating contacts is confirmed; these captures alone do not establish a complete replacement ordering algorithm. See `captures/lifecycle-audit.json` for explicit limits and the next causal test.

Observers are Windows x86 debugger helpers built with the existing Visual Studio 2008 toolchain in Parallels. They insert temporary debugger breakpoints and read memory, without replacing the original executable/DLL on disk. `workspace-reproduction` scripts are provenance for the existing development workspace and depend on its immutable Windows Golden file and existing probe paths. They are not standalone installation scripts. Captures are already gzip-compressed; physical files contain all 600 samples, not an onset-only excerpt.
