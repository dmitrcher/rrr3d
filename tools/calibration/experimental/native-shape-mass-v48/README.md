# v48: original shape mass arithmetic and diagnostic stabilization isolation

`RRR3D_EXPERIMENT_NATIVE_SHAPE_MASS=1` reconstructs the original SSE box,
ordinary raycast-wheel cylinder and translation arithmetic. Enable it on the
published v47 experimental SDK/adapter chain, with the original PC53 principal
inertia flag. Shape positions, radii, chassis masses and force coefficients
are unchanged. Both mass experiments remain off by default.

The former mass formula was physically equivalent but only approximately
matched the SDK. Its cylinder multiplication association and factorization
rounded differently. Jolt's translation subtracts an axis square from the
sum of all three squares; this loses smaller terms for raised wheel proxies.
The original multiplies skew matrices instead. The original box uses half
extents and rounded mass/3, while Jolt uses full extents and mass/12. A provider
also normalizes its center through a first-moment/mass round trip.

The standalone C++ implementation matches all **120 actual original shapes**
(1560 float fields, including signed zeros) and **24 compound tensors plus
principal values/axes** (504 fields), in both O0 and O2. The SDK observations
repeat twice and preserve every original untraced physical state. This oracle
covers ordinary wheel flags 64 and the shipped wheel pose. The alternate
flag-32 mass provider and arbitrary shape rotations are not certified by it.

The Windows numerical probe skips neutral game stabilization; Mac previously
performed an inertia/momentum/inverse-inertia round trip every step. This
introduces a separate test harness effect even with damping 1 and no clamps.
The optional `RRR3D_CALIBRATION_ISOLATE_NEUTRAL_STABILIZATION=1` tells only that
diagnostic controller to omit the operation in those conditions. The new
VehicleDriveCommand field defaults to true and is appended after existing
fields, preserving existing aggregate initializers. Real GameCar controllers
retain their stabilization calls, including the original unconditional
setAngularMomentum. The diagnostic switch is not a production game fix.

Fresh current-binary captures retain **72/72 tilted contact cases**, **6/6 old
offset cases**, **6/6 inverted cases**, and **189/189 force-isolated cases**,
each twice. Default-off mass controls equal the actual older v47 binary.
The full Debug app builds/signs with 1200 assets and 14 movies; 53/53 CTest
checks pass. No new GUI race result is claimed.

Acceptance remains pending. The broader 24-case comparison still has **18
physical residuals**. Shape arithmetic alone increases a few trajectory error
maxima; it cannot yet be promoted. Monstertruck Boss position error falls from
0.000024914129746713667 (v47) to 0.000011598759683831497, which remains above
v45's 0.000010690174229377852. Diagnostic isolation lowers it further to
0.000007703045888687966. All 17 spin position errors decrease when the harness
operation is isolated, but their nonfrozen orientation and reported angular
momentum still differ. The first-step quaternion becomes identical across all
17 Mac chassis, locating the next investigation in rotation integration,
pose/API conversion and momentum reporting instead of mass or engine power.

Apply the adjacent four-file patch after v47 and rebuild the Mac probe.
`run_isolation.py --contact-v48-shape-mass --isolate-neutral-stabilization`
selects the full candidate chain. From an extracted investigation archive:

```
python3 source/tools/calibration/experimental/native-shape-mass-v48/verify.py --package .
```

The verifier recompiles against actual native words, checks raw capture
preservation, repeats, retained parity, known residuals, hashes and exact patch
application. Passing it validates the investigation, not candidate acceptance.
