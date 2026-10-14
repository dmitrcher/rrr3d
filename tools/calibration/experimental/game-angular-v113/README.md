# Original angular momentum API and game stabilization V113

Validated investigation checkpoint; overall goal remains incomplete. Four-file default-off adapter/harness delta over verified V112; VehicleDriveCommand angle fields preserve double controller precision, so rebuild all consumers. Windows and Mac fixture parameters remain unchanged; the Mac probe removes premature float narrowing. Ordinary game float properties keep their input value.

The game path uses the original public momentum getter, general actor-matrix inverse, local clamp/damping and world reconstruction. The setter uses the original scalar inverse tensor and Z,Y,X projection order, including its determinant-based finite magnitude guard. Identity diagonal mass frames use the new arithmetic; unsupported mass frames retain the existing path. Original Shoemake XYZ Euler conversion rounds the sum of squares before sqrt. No reference states, case/frame rules or coefficient fitting enter runtime. New flag: RRR3D_EXPERIMENT_NATIVE_GAME_ANGULAR_V113.

Production header O0/O2 matches8actual original setter tensor/velocity and8stabilization moment checkpoints, plus6Euler angle/quaternion checkpoints. Independent live Source at14points matches all10incoming pose/inverse-inertia/velocity words, world momentum, installed quaternion and setter velocity, together with every full120-step original history. Original EXE/DLL and probe histories are unchanged.

Complete physics and whole-game/render equivalence remain incomplete. Portable verification covers8280fresh original-equivalent states and120flag-off V112-preserved clamp states.

Complete matrix: 590/608 strict, 18 open, all586previous strict retained. Four new complete120-step game angular histories match Windows;604histories unchanged. Two255864-state repeats exact. No audited full-run maximum worsens.53project checks pass both default and candidate profiles;291independent gates,solo600/rendered240 functional checks pass.
