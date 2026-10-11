# v35 independent cached contact points/errors

This candidate is layered after the v34 adapter/SDK experiments. It accepts
only the measured flat, no-AI, zero-friction/restitution dynamic/static normal
contacts. All189 isolation cases now pass; 70/72 broader tilted contacts remain,
with21 position-error and27 velocity-error increases vs cache-off. Flags remain
default off. Full game physics parity is not accepted.

From the extracted audit directory on ARM64 Mac:

```sh
chmod +x executables/macos/rrr3d_physics_spring_probe executables/macos/NativeContactCacheProbe
python3 source/tools/calibration/experimental/native-cache-v35/verify.py --measurements measurements --cache-probe executables/macos/NativeContactCacheProbe
python3 source/tools/calibration/experimental/native-cache-v35/verify-patch.py --snapshots snapshots
```

The first verifier checks all12 full120-step contacts, both repeats, cache-off
physical equality with v34, all189 isolation cases and160 later Native solver
rows. It also invokes the compiled helper on120 recorded-input cached tangent
vectors and checks both Native instrumentation variants leave all120 physical
states unchanged. Tilted/control captures must retain70/72 residual cases.

Rebuild the standalone arithmetic helper:

```sh
clang++ -std=c++17 -O2 -ffp-contract=off -Isource/tools/calibration/experimental/native-cache-v35/src/Rock3dEngine/header/physics source/tools/calibration/experimental/native-cache-v35/cache-probe.cpp -o NativeContactCacheProbe
python3 source/tools/calibration/experimental/native-cache-v35/verify.py --measurements measurements --cache-probe ./NativeContactCacheProbe
```

The copied tested headers require SourceMaterialFrameCandidate.h and
ContactFrameScalar.h provided beside them. Raw tangent helper inputs are7
numbers (quaternion4, displacement3); it returns2 tangent components. It reads
no case names or expected outputs. Ordinary Mac captures use independently
computed geometry, errors and state, without replay.

The six-file patch/base manifest lives one directory above. Apply Jolt/* paths
inside the Jolt SDK, src/* paths inside the engine checkout. Snapshots contain
only these changed files, not a complete repository. Full tested after-files
are also provided here. This does not change the canonical production adapter.

`capture-mac.py --probe PROBE --measurements NEW_DIR --contracts cases.json
--mode ordinary` captures both12-case repeats, with the v34 chain and cache flag.
Use `--mode control` and provide the v34 ordinary baseline file for comparison.
For whole-ladder gating use `run_isolation.py --contact-v35-cache`; Windows
captures keep the original Native probe and physical effects disabled.

Build trace-contact.cpp using build.cmd in a fresh later-v35 guest folder.
For the write-watch build, copy trace-cache-watch.cpp as trace-contact.cpp in a
separate cache-watch-v35 folder. Both need the original v30 guest runtime with
verified SpringProbe/Core/VC90 hashes. Core, loader and VC90 DLLs are not bundled.
`capture-windows.py` checks the original probe/Core and supplied tracer hash;
it requires the v30 reference-row baseline in measurements. Instrumentation
never substitutes physical states. Internal pointer/padding values are not
parity criteria.
