# Independent PhysX 2.8.4 contact probes v12

Build `build-windows.cmd SDK-prog-directory output-directory` using VC2008 SP1,
with the original PhysX 2.8.4 runtime and Microsoft.VC90.CRT in the output directory.
Run `MotorRock-ContactSkinProbe.exe output.jsonl all` there.

The 132 probes retain the original four wheel shapes (raised away from surfaces),
source LOCK_COM and contact modification, skin .025, solver iterations 4,
Z-up variable steps and adaptive force disabled. Friction is explicitly disabled
in the probe materials to isolate penetration/normal response. These are SDK
experiments, separate from the production reference stand with original friction.

60 stationary frozen probes vary pitch, body skin .01/.025/.1 and normal/thin
body geometry. The measured final target is wall plane minus full-box support
along its normal plus the sum of scalar skins. The Mac target regression test
uses this independently measured oracle. Transient solver equivalence is not
established by matching the final target.

72 bias probes vary initial penetration and velocity for primitive box and
front-facing triangle walls, with frozen and free rotation. Deep initial overlap
reveals different position relaxation, velocity response and angular integration;
these residuals remain open. Frozen/free Nx first-step linear responses match,
but their angular momentum and orientation do not. This does not prove frozen
rotation makes Nx constraint inertia zero.
