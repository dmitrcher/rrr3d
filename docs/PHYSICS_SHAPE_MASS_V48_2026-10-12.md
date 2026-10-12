# Shape mass audit v48

The original mass providers were reconstructed from PhysXCore box 0x1370e0,
cylinder 0x136bd0 and translation 0x135e60. All 120 observed individual
shape records now match the standalone Mac C++ reconstruction bit for bit,
including mass, center and every tensor element. Their compound tensors and
the v47 principal decomposition match all 24 original cases as well.

This audit identifies arithmetic errors in the older approximate mass
implementation, not a need to adjust engine torque or chassis parameters.
The active windows-source-v14 profile retains source actuator scales one.
The physically equivalent formulas nevertheless rounded differently:
cylinder association, box half-extent calculations and cancellation in the
parallel-axis translation. The implementation preserves original float stores
and generic SDK constants; it contains no fixture identifiers or measured
replacement tensors. The 120-shape oracle covers ordinary flag-64 wheels;
other wheel flags and arbitrary local rotations require further native traces.

A second issue is asymmetric diagnostic isolation. The Windows probe omits
neutral GameCar stabilization, while Mac performed its momentum round trip.
An explicit diagnostic controller switch now allows identical omissions.
The gameplay command defaults to applying stabilization, preserving the
original GameCar's unconditional momentum setter. This is a test harness
control, not a reason to remove source game behavior.

The current rebuilt probe repeats 24 broader cases, 72 tilted contacts, six
old offsets and 189 progressively isolated scenarios twice. The old v47
binary is an independent default-off control. All previously exact contact,
offset and isolation states are retained. The six inverted fixtures remain
exact. The 18 broader physical residuals remain visible; a few error maxima
increase with mass arithmetic alone, so production acceptance is deferred.
Neutral diagnostic isolation reduces position errors for all 17 spin chassis,
and exposes a common first-step rotation difference. Next trace original
angular integration, physical quaternion versus API quaternion/matrix, actor
origin recovery and angular momentum reporting.

The full Debug app is built, signed and asset-checked (1200 assets, 14 movies).
53/53 CTest checks pass in 12.44 seconds. No new GUI or full-game parity claim
is made. The candidate archive contains probe binaries and evidence, not a
replacement game installer. The unlimited goal remains active.

See tools/calibration/experimental/native-shape-mass-v48/README.md for the
flags, numerical error values, patch dependency and reproducible verifier.
