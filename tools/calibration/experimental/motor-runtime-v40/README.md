# Original Windows DLL motor audit

`Symbols.exe` reads the original PDB and lists function RVAs. `MotorRuntime.exe`
loads the existing Debug game DLL and calls its original CarMotorDesc constructor
and CalcTorque, rather than compiling a copy of the formula. It constructs one
private motor description in the helper process and sets only its torque and SEM.
No running game, wheel command, or physical coefficient is changed.

The RVAs and descriptor layout are specific to original Debug Rock3dGame.dll:
SHA256 `4483e8f402edb5c4639609dcf3fd805386e80f1d25b1c4829a8236b40a6b8a15`.
Check that SHA before execution; do not apply the RVAs to another DLL.
Constructor RVA 0x492380; CalcTorque RVA 0x4926b0, identified using its PDB.
The original function body uses x87 multiplication and stores float only at return.

Build with VC2008 x86 using build.cmd. Keep both original VC90 CRT and DebugCRT
available beside the helper, as in the existing VM probe directory. Run twice.
The output has 72 inputs at PC53 and the same 72 at PC24. The helper explicitly
changes its own x87 precision, not the precision of the running game.

PC53 reproduces all 72 previous Windows source-expression helper rows, with
27 differences against the current Mac collapsed expression. PC24 reproduces
all 72 Mac sequential source-expression rows, with 15 collapsed differences.
A double product matches all 72 PC53 rows; this is conditional numerical
validation, not a selected production arithmetic policy.

Actual game-thread precision after Direct3D initialization and GameCar → wheel
command tracing remain pending. The initial full-game tracer attempts captured
zero target calls and timed out. Do not count them as game validation.
No engine-power coefficient or GameCar production calculation is changed in v40.
