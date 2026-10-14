# V129: original Windows startup orientation audit

Goal remains active and incomplete. This is a standalone research model; no installed game or V128 physics adapter is changed.

The actual Windows Rock3dGame.dll PDB identifies BaseSceneNode ctor, SetDir, SetUp, GetRot and GetWorldRot. The observer loads that unchanged DLL, constructs a real parent-free node (including virtual bases), calls the original methods with source fixture directions, and reads resulting direction/up/matrix/quaternion words. This does not observe a running rendered race, parent transforms, original spawn placement, or the subsequent NxActor import.

Four map directions (0,16,48,64) plus 64 generated nonzero horizontal vectors: 68 directions, PC24 and PC53, two identical captures. Original DLL/PDB/system D3DX hashes remain unchanged. All outputs for these inputs are equal between precision modes. The system D3DX DLL is used only locally and is not distributed.

Disassembly confirms the original ExtractRotation uses D3DXQuaternionRotationMatrix, unlike later GLM migration source. Live D3DX dispatch selects SSE rsqrt estimate plus one explicit Newton refinement for normalization and x87 matrix-to-quaternion conversion. The portable model reproduces this arithmetic using the independently audited Windows ARM rsqrt emulation. It computes outputs from input directions only, without reference tables or per-track conditions.

Portable O0 and O2 each match all 68 cases, each of 26 words (direction3/up3/matrix16/quaternion4), exactly. Both native repetition and PC groups agree. Tests use clang with floating-point contraction disabled. The model is limited to nonzero horizontal direction and parent-free startup; zero/vertical/fallback paths and other architectures are not certified.

Run from the workspace root: `python3 work/startup-orientation-v129/verify-portable.py` and `python3 work/startup-orientation-v129/verify-expanded.py`. Windows observer build/capture scripts require existing original game/VS2008/Parallels installation.

Next: derive actual GameObject::SetWorldRot / NxActor setter import and starting pose, then replace the synthetic atan2 startup in paired experimental probes with the certified path. Re-run all four real maps and retain V128 isolated regressions. Never fit Mac to the synthetic bridge or force measured initial quaternions.
