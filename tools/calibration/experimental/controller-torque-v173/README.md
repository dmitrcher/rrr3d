# V173: preserve original SEM and torque multiply order

Research candidate over V171 Source and unchanged V154 Jolt SDK. Five owned Source files. `RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173=1` enables the new arithmetic for controllers carrying original serialized motor factors; default OFF. Ordinary installed app is not promoted. Windows remains unchanged.

## Cause

The Mac importer folded SEM*1.15 into torqueEfficiency before the controller multiplied maximumTorque*gear*gearDiff*torqueEfficiency. Original CarMotorDesc::CalcTorque multiplies maximumTorque*gear*gearDiff*SEM*1.15 in sequence. V172 directly observes24-bit x87 precision in actual Windows CalcTorque and MotorProgress, so V170's PC24 reference is now applicable.

## Change

Preserve the original serialized SEM separately in VehicleDescription, pass it through OriginalPlayer to GameCar::MotorDescription, and perform the four original float32 multiplications in order under the new flag. Retain the old efficiency for legacy/direct descriptions without original-factor metadata. Do not divide a rounded efficiency to reconstruct SEM. No power, mass, gear ratio, tire, suspension, time-step or seed tuning is included. Legacy raw fixture serialization stays unchanged because it drives wheel torques directly.

## Evidence

* `comparison.json`: all24 original V170 cases match actual compiled Source GameCar::OnFixedStepDrive twice. OFF differs9/24 each repeat.
* `evidence/extended-comparison.json`: six maximum-torque values x all six gears,36 cases per repeat, all exact twice versus actual original DLL at PC24. OFF differs15/36. Expanded Native capture includes both PC24 and PC53,72 observations per repeat; the pre-existing four parameter sets reproduce all48 prior observations. Reverse torque sign is normalized for the pure CalcTorque comparison. This is a torque-component proof, not a full controller proof.
* `evidence/gamecar-event-replay/comparison.json`: actual importer -> Player -> GameCar -> session bridge. At step120, imported base1000 motor/gear1/full throttle produces original Windows word1172625912; old Mac word1172625911 differs by one float32 unit. Two corrected900-step runs repeat exactly. Scene words, inputs, impulses and V171 wake-event ticks are preserved. Flag OFF matches the complete V171 enabled replay,900 states and900 commands.
* `evidence/raw-regression-comparison.json`: four existing real-map raw-actuator scenarios x900 states x2 repeats,7200/7200 exact versus immutable Windows. Only affected regressions are repeated; no new608-case run is claimed.
* `ctest.log`: all53 tests pass. `installer-proof.json`: five-file guarded installation, then no-op; unknown prerequisites rejected before mutation. Actual current Source and SDK also pass the installer verification without changes.

The ordinary Windows garage screenshot shows an equipped motor1700 while the isolated Source replay imports base1000. These are different loadout contexts, not evidence that one platform's engine was deliberately weakened. The extended curve includes both values. Full-game comparisons must explicitly match equipment and upgrade application before interpreting power differences.

## Remaining work

Compare actual original MotorProgress/RPM/gear/brake/steering behavior on identical actor states and equipment. The torque curve and wake transport are now individually proven; their agreement does not establish complete MotorProgress or race trajectory equality. Then run ordinary single-car game paths, controlled multibody/AI scenarios and promote a verified user-facing build with visual/loading-log and release checks. Keep the goal active.
