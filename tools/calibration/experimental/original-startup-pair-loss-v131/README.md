# Original startup and pair-loss wake investigations (V130/V131)

Goal incomplete. Two default-off source flags: `RRR3D_EXPERIMENT_ORIGINAL_STARTUP_V130` and `RRR3D_EXPERIMENT_NATIVE_PAIR_LOSS_WAKE_V131`. Apply this two-file overlay to the verified V128 experimental source base with `install-guarded.py --source-root PATH --apply`; hashes protect both inputs and outputs. Ordinary repository adapter and installed game are not replaced. SDK files are unchanged from V128.

V129's source-only original D3DX startup calculation is now used in Mac reset for nonzero horizontal directions. The independent Windows bridge invokes actual unchanged game DLL/PDB methods: BaseSceneNode constructor/SetDir/SetUp/GetRot followed by px::Actor constructor/SetPos/SetRot on the real SDK actor. Virtual bases are constructed correctly; borrowed NxActor is detached before actual destructors/free. This checks the parent-free setter path, not rendered original Race spawn placement or parent transforms. Original game/SDK/PDB/fixture hashes stay unchanged.

Four source map fixtures (0,16,48,64), one car, no AI, same raw wheel commands/step/geometry, 900 steps, twice each platform. After startup correction, exact prefixes are 900/787/781/95. In map64, Mac incorrectly slept at95. Read-only original energy/wake observers preserved all900 public states and identified default wake requests during potential-pair removal (Core caller1694c after energy32;13ff2 before energy33). The Mac model now retains removed geometric overlap pairs for that solve and requests default wake afterward. It uses runtime body IDs/geometric overlaps, without map IDs/frame rules/measured state injection.

Final V131 map results, full F32 body/wheel state plus awake status, two repeats: map0 all900 exact; map64 all900 exact; map16 first difference787; map48 first difference781. Remaining two differences follow vertical impulse780: first differing body velocity, angular momentum and quaternion; wheel observables still match. Read-only Native versus final Source energy transitions for map64: all111 transitions/counts/energy/threshold/wake/pending exactly match, twice.

This is SDK-only replay: torque commands do not automatically wake the original resting actor. Motor phases120..659 are asleep in these fixtures; this is not a GameCar acceleration/control verdict. Actual game driver/motor FP context, stabilizer/wake API integration, compound/sensor/death-plane/dynamic decoration/island cases and actual installation remain open.

Build succeeded. Final default-off53 CTest checks pass and900 old physical samples/900 drive commands are preserved. The first completed full608 frozen capture with V131 enabled matches all255864 original F32 states; second capture is still running at this checkpoint, so repeatability is pending. Do not promote on the map result alone.

Failed/superseded first observer cleanup and pre-lock-fix map/matrix runs are excluded. Final Windows bridge process completes releaseScene/closeCooking/releaseSDK/return. The final source lock scope ends before wake API calls.

Next: inspect actual private solver/integrator states at map48 step781 and map16 step787, preserve all earlier exact histories, complete full regression, and validate the production GameCar path.
