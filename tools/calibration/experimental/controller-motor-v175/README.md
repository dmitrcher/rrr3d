# V175: original MotorProgress plus TransmissionProgress

Verification over unchanged V173 Source and V154 Jolt SDK. No new production runtime patch or parameter tuning is introduced. Original Windows functions remain the reference. The V173 torque-order flag is required for complete agreement.

## Real original controller objects

The harness calls original Shapes::RegisterClasses, the ordinary game's required shape-factory startup function. Original GameCar constructor owns the actual CarWheels collection; original CarWheels::Add creates its real CarWheel and shape wrapper, and original SetLead registers the first driven wheel. The SDK actor and wheel are borrowed using PDB-resolved fields; original WheelShape::GetNxShape verifies the binding. Borrowed pointers are detached before the original destructors run. No fake collection, fake vtable or reimplemented Windows controller supplies the reference.

Actual original GameCar::MotorProgress and TransmissionProgress are called in ordinary OnFixedStep order, with fresh zero torque/brake/RPM output variables as in the original caller. Every case explicitly restores the initial gear, move command, torque, automatic-gear flag, torque scale and physical input state. PC24 is selected after SDK preparation to reproduce the production context observed in V172.

The SDK's documented NX_WF_AXLE_SPEED_OVERRIDE isolates the wheel-speed input. The harness simulates/fetches preparation, queries real wheel contact and asserts the actual axle word equals the requested word before calling the controller. This override makes simulated motor/brake forces ineffective; therefore these are controller-output comparisons, not physical acceleration or tire-force comparisons. Native outputs are never sent into Source as expected values: only measured velocity/axle/contact state and motor setup enter the separately saved Source input files.

## Results

13512 cases per repeat cover both no-contact/contact states, maxTorque1000/1700, automatic/manual transmission, neutral/reverse/all forward gears, all four move commands, positive/negative/deadband-adjacent speeds, signed axle speeds and RPM limits, scale0.6/1/1.4, and float32 neighbors of downshift/upshift boundaries. Compare exact float32 signed speed, motor torque, brake torque, RPM and final gear.

With V173 enabled:13512/13512 exact twice,27024 checked results. Without V173:1704 differences each repeat, all solely motor-torque words; speed, brake, RPM and gear still match. Each repeat includes784 upshifts and580 downshifts;6756 cases have real wheel contact and6756 do not. Original DLL/PDB and SDK/input/executable hashes stay unchanged around each capture. Both Native and Source repeat identically.

`evidence/accepted/isolation-verification.json` strengthens the initial matrix: contact and air fixtures run in independent SDK scenes, each asserted to contain exactly one dynamic body, with no AI. Both13512-case original histories, including measured inputs and controller results, are identical to the earlier histories. The unchanged Source executable/input/output captures are reused after their hashes and exact Native histories are checked; they are not rerun just to duplicate evidence.

## Rejected setup attempts

The initial Add and standalone-wheel-constructor attempts hit an original collection exception before any controller sample. An owned stack observer located Collection<px::Shape>::Add<WheelShape>; the original startup class registration was missing. These test processes were explicitly terminated, their incomplete results retained and never accepted. After registration, early prototypes still did not fix axle speed because the SDK requires the override flag. Their actual getter values exposed the bad setup; they are excluded too. Accepted tests assert the input, rather than assuming the setter worked.

## Scope and next work

This closes the tested MotorProgress/TransmissionProgress scalar-output matrix. It does not establish the full original OnFixedStep, WheelsProgress, stabilization, steering pose changes, JumpProgress, equipment/Player torque-scale application or ordinary race trajectory parity. Next construct four original wheels with matching positions/dynamics and compare those effects before/after their actual original methods against Mac. Preserve one controlled car per scene and exact input records. Then compose the ordinary-game single-car trajectory and wider car/equipment/contact coverage.

No608-case or7200-map-state gate was rerun in this verification-only checkpoint: project Source and SDK did not change, and the V173 guarded verifier still reports zero changes. The previously accepted physics gates remain frozen. Original SDK headers, DLLs, PDBs and compiled probes are not included in this source checkpoint.
