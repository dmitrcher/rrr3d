Next work after V173

Previous goal turn is progress: actual Windows PC24 context observed twice; original torque association fixed and verified36/36 twice; actual Source importer/player/controller command now matches original DLL at first throttle step120;53 tests and7200 raw-map states preserved. Goal is not complete.

1. Compare original DLL CarMotorDesc::CalcRPM at production-proven PC24 against actual Source OnFixedStepDrive on identical signed axle speeds and all gears, including neutral, idle and maxRPM clamp. Preserve Windows. If it matches, close it without changing unrelated parameters.
2. Build an original GameCar MotorProgress/TransmissionProgress measurement using actual constructors and wheel wrappers, or capture its actual input state and output values at fixed-step boundaries. Do not emulate the Windows controller formula to create its reference. Inspect PDB-resolved original CarWheels and CarWheel construction; there must be a real first driven wheel for GetWheelRPM/GetWheelContactData.
3. Match car equipment explicitly. Actual original Player::ApplyCarFunctions resets motor maxTorque and sums installed MobilityItem::CarFunc maxTorque; Source OriginalSlot also sums. The isolated map fixture has base1000 whereas the observed ordinary garage has1700. This alone is not a platform discrepancy; inspect matched loadouts before changing values.
4. Compare full controller outputs, then actual ordinary-game no-AI trajectories. V173 curve proof alone does not close MotorProgress, steering, stabilization or production race parity.

Current active Source is V169+V171+V173, default-off candidate flags; unchanged V154 SDK. Installed ordinary Mac app is older and not promoted. Exact file hashes and guarded installers are in the checkpoints. Frozen V169 reference remains untouched.

No observation/capture/test jobs remain running once this checkpoint is finalized. The owned Windows MR process6840 was started by V172 and left at the pause menu after captures; revalidate PID/path before interacting or attaching. Caffeinate68828 remains on; preserve user preference.

Do not rerun the unchanged608-case gate just to generate activity. Run it when related physics code changes or before a new accepted physics checkpoint. Only raw map/controller regressions were repeated for V171/V173.
