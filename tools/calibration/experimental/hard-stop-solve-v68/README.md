Default-off hard-stop arithmetic investigation. RRR3D_EXPERIMENT_NATIVE_HARD_STOP_SOLVE_V68=1 uses the original scalar JV accumulation and separate velocity/geometric impulse terms. It preserves the original 0.7 position coefficient and every vehicle parameter. Hard state is reset at preparation and geometric bias cleared before the final unbiased pass. Restricted to static opposing bodies, zero steering, and ordinary suspension force points.

Seventeen single-car fixtures, four variants and two repeats each, separate V64 control, V66 control, V68 arithmetic alone, and V66+V68. Disabled behavior preserves both saved controls exactly. Independent saved Native inputs verify 240 float32 words at -O0/-O2; all 291 independent gates and 53 CTest checks pass.

V68 alone improves xCar landing maximum position error 0.00000821064 -> 0.00000004244985 and curb 0.000281268 -> 0.000223127, but podushka/engine2 worsens 0.00106010 -> 0.00210769. The large border error remains. This variant has not been promoted and no full 608-case matrix is claimed for it.

SDK snapshots depend on the exact read-only V67 preparation telemetry checkpoint. All source files are validated before the guarded installer writes any file. Ordinary game flags remain off. See the separate V69 static-friction correction for the dominant coupled border error.
