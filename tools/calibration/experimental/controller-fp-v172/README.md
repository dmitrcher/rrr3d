# V172: actual Windows game floating-point context

The previous V170 torque observations differed between explicitly selected x87 PC24 and PC53; production precision was unknown. V172 observes the actual unchanged Windows MR.exe in the active Parallels desktop session. `prlctl exec 'Windows 11' --current-user ...` runs in session1; the default command session is SYSTEM/session0.

The owned x86 debugger uses original Rock3dGame.pdb symbols and temporary software breakpoints. It restores each instruction and the original instruction pointer before continuing, and restores unhit breakpoints before detaching. It does not change arithmetic parameters or the control word. Disk DLL/EXE/PDB files remain unchanged. These are observations at function boundaries, not instrumented replacement implementations.

Two completed attachments to the same actual race independently recorded x87 and FXSAVE control words127 (0x007f) at GameCar::OnFixedStep, GameCar::MotorProgress entry and return, and CarMotorDesc::CalcTorque entry. Precision-control bits8..9 are zero:24-bit precision. Rounding-control bits10..11 are zero:nearest. The first attachment also observed the actual GameCar constructor with that control word. The first completed MotorProgress call returned motor0, brake400 and RPM1000. Evidence and original hashes are in `evidence/comparison.json` and the compressed raw runtime logs.

The ordinary race contains AI. It is used solely to observe the executing thread's FP context; this is not a trajectory comparison and does not replace the isolated no-AI tests. Both attachments completed and detached; no debugger job remains live.

The initial launch attempt hit the240-second observation limit with zero observations while the original single-race setup displayed a missing Profile/skirmish.xml assertion. It detached with all breakpoints restored and is retained as a negative result. Selecting the original assertion's Ignore button allowed original default-profile setup to proceed. No profile data or Windows binary was rewritten by the observer. The successful measurements then used the attach mode with a600-second bound.

Build: `python3 native/build.py` from the research workspace. Observe an existing owned test process with `prlctl exec 'Windows 11' --current-user cmd.exe /c 'C:\MotorRock\calibration-probes\controller-fp-v172\FpDebuggerV172.exe C:\MotorRock\calibration-probes\controller-fp-v172\runtimeN.jsonl PID'`. Without PID the observer launches its own game. It deliberately captures only the first matching calls. Original game files and symbols remain external to this source checkpoint.

Result: select the already measured PC24 reference for the next torque-order repair. This closes the precision ambiguity; ordinary controller/RPM/transmission trajectory parity remains open.
