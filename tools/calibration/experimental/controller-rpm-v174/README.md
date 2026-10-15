# V174: RPM component agrees without another runtime change

Original unchanged DLL CarMotorDesc::CalcRPM is resolved from the original PDB and called on the motor owned by an original-constructor-created GameCar. Calls use PC24, directly observed in the actual Windows game in V172. Compare with the actual compiled Source GameCar::OnFixedStepDrive, not a replacement formula.

4893 finite input cases cover neutral and all six gears, positive/negative axle speeds, idle, maximum-RPM clamp, three motor profiles (3500/600/1.25,7000/1000/3.42,9000/1300/5.7 for maxRPM/idle/differential) and neighboring float32 input words around maximum and downshift boundaries. `cases.words` contains only test inputs. Both platforms read the same bytes; original and Source output words agree4893/4893 twice, each platform repeats identically. Windows DLL/PDB, executable and input hashes are checked before/after. No Source physics or game code changed in V174.

The wheel-symbol records additionally establish the PDB argument counts for the original CarWheels::Add overloads: the no-explicit-argument method has count1 including this; the descriptor overload has count2. V175 uses the former to construct genuine original owned wheel objects.

Run `native/build.py`, `build-source.py`, then `capture.py` from the research workspace. Native SDK headers and original proprietary binaries remain external. This component proof does not by itself prove full MotorProgress, transmission or a race trajectory. Those require their own checks.
