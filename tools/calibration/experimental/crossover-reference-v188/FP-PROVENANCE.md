# Original game PC24 provenance audit

Actual MR.exe runtime evidence exists in outputs/controller-fp-v172/runtime1.jsonl.gz and runtime2.jsonl.gz. They are completed attachments to original PID6840/session1/thread6820, observing GameCar::OnFixedStep, MotorProgress entry/return and CarMotorDesc::CalcTorque. Both x87_control and fxsave_control are127 (0x007f). Run1 also observes the constructor. runtime0 has zero hits and is not evidence.

The source/executable match the saved V172 build receipt: source541b6b1ae17fd457736f7d572d5feb9dfaf72b89e4306688be470e51fc920e00; EXE50feb3683552aa15206fcfa1fdf45bbc1a54b6db7a95b1be4a4684d75e07f2fa. Audited fp-debugger.cpp:32–36 reads FLOATING_POINT/EXTENDED contexts and changes only EIP. No precision setter. MR f939a66f6d4fc1ad83d101d241b4f6b6e219e359551cda487b277d7079d0c9be; GameDLL4483e8f... and PDB818ba6d... match existing identity.

Supporting original source: D3D9RenderDriver.cpp uses hardware/software vertex-processing flags without FPU_PRESERVE in both CreateDevice paths. World.cpp initializes graphics before PhysX. This supports the cause but is secondary to runtime observation.

Limits: one ordinary race/process, two attachments, not independent launches/all builds/drivers. Production StabilizeForce entry/internal PC was not directly recorded; PC24 there is strongly supported by same-thread flow. This does not establish CrossOver arithmetic equivalence. Existing PC24 Source experiments are not reverted on the basis of this different compatibility environment.
