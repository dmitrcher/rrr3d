# Original suspension-length reporting precision V116

Validated investigation checkpoint; overall goal remains incomplete. The original Windows probe subtracts float contactPosition and radius in x87, then stores a double for reporting. Mac previously stored a float before reporting. Nine-digit decimal output around a float midpoint can then produce a false float-word difference when reparsed. The actual correctly rounded Windows lengths already matched Mac on every step of both investigated 600-step cases.

The diagnostic field now retains double precision when `RRR3D_EXPERIMENT_NATIVE_SUSPENSION_LENGTH_REPORT_V116` is enabled. The flag is off by default; its disabled path retains the V115 float store. The solver still uses its existing float coordinates. All consumers must be rebuilt after widening the public diagnostic field.

Actual Source distance and radius words match the original API getter results in both control/candidate profiles, two repeats, throughout both 600-step cases. The derived double values also match exactly. Every other observable in all 608 trajectories is decimal-exact unchanged. Original reference files, executable and DLL remain unchanged; no tolerance, coefficients or reference states enter runtime.

Apply using `apply-experiment.py --adapter-root PATH --sdk-root PATH`; `--restore` restores the recorded V115 sources. Every file is checked before mutation. `verify.py --probe PATH` runs fresh isolated histories and original API word checks against packaged read-only evidence. Whole-game and render parity remain unproven; this package does not replace the ordinary installation.

Complete matrix: 596/608 strict, 12 open; all 594 previous strict cases retained. Two reporting differences close; physical movement is unchanged. Both 255,864-state repeats match exactly. Every field other than diagnostic wheel length is decimal-exact preserved across all 608 cases. No audited maximum error worsens. All 53 project checks pass under both profiles; 291 independent gates and solo/rendered functional checks pass.
