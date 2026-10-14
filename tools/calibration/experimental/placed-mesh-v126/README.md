# Placed source meshes and real-world reference bridge audit (V126)

Experimental, off by default. Enable `RRR3D_EXPERIMENT_NATIVE_PLACED_MESH_V126=1` with the complete V123 parity profile/SDK chain. This is not full game physics certification.

The source adapter previously merged original meshes by surface and baked vertex transforms. The original PhysX reader keeps separate static actors, transforms rays into local geometry and transforms the interpolated hit back to world. `NativePlacedMeshV126.h` reconstructs the original actor/shape arithmetic from WorldDescription input; no measured reference poses or states enter the runtime. Source triangle identity and material/decoration owner remain available. Explicit original body-contact generation uses the physical placed matrix as well.

Actual source-input actor and shape pose reconstruction was checked against all 110 track0 meshes. The production helper's four local queries match 60 nonzero measured words in O0/O2. Mesh ray origin/direction sum order and stored inverse translation are derived from original Core disassembly, rather than borrowed from the different box query.

The bridge audit found V124 omitted `NX_AF_LOCK_COM | NX_AF_CONTACT_MODIFICATION`. These flags are set by original `DataBase::AddPxBody` and `GameCar::GameCar`, and retained by the immutable original Windows calibration probe. Correcting the bridge changes its initial mass-space inertia. Older V124 world trajectories are superseded for parity; static mesh-pose observations remain useful. The original Windows game, SDK and 608-case reference were not changed.

For **SDK-only raw actuator replay**, the existing `RRR3D_CALIBRATION_ISOLATE_NEUTRAL_STABILIZATION=1` now omits GameCar's neutral momentum get/set call and zero velocity impulse calls that would activate only the Mac car. Actual impulses remain at 720 and 780. Ordinary GameCar/standard replay and the default raw mode retain their original calls.

The earlier partially isolated track0 candidate matched every observable through tick115; the first116 divergence was traced to Windows asleep vs Mac awake. Native `isSleeping()` read-only logging confirms this and preserves all900 physical states. The Source replay was calling `addLinearVelocity(0)` every tick, whereas the Windows bridge called the impulse API only twice. The latest fully isolated API mode matches all observables through tick78: Mac sleeps at79 while Windows sleeps at116. This exposes a sleep-model residual previously masked by Mac-only zero impulse calls. Do not alter inertia coefficients or sleep thresholds based on the previous asymmetric harness.

Three other maps repeat exactly across seeds, but their initial quaternions differ between the bridge startup and Mac reset. This must be resolved before later parity is interpreted. Also, the current Windows bridge remains asleep across raw motor commands until the explicit velocity impulses; it does not yet represent actual GameCar wake/control behavior. Do not claim its900-step torque stages test original gameplay acceleration.

The full608 matrix and default-off controls are independent acceptance gates. The installed ordinary app remains unchanged. Source experiment acceptance, complete scene loading fidelity, body contact order, sleep/wake, actual GameCar controls and all-map game integration remain open.

Latest validation: full608 cases,255864 physical states, two repetitions, exact retained parity. The final default-off replay also preserves every one of900 prior physical samples and drive commands. Complete real-game validation and the sleep correction remain open; this experiment is not promoted to production defaults.

The native bridge reader uses existing Fixture.h/WindowsReferenceGeometry.h/ParityTrace.h and original SDK headers/libs; native-world-build.cmd documents the local Windows build layout. Evidence paths in manifests identify the development workspace. The large raw/capture archives remain in the workspace, compressed; they are not included in Git.

The source files and patch are stored under this experiment's `after/` directory, for the complete research chain. The ordinary repository adapter uses an earlier SDK/API and is deliberately not replaced by this research overlay. Apply only to the before hashes in source-changes.json after the V123 chain, including the V124 optional replay hooks.
