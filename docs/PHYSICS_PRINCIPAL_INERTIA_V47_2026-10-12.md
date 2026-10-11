# v47: principal inertia investigation

The v46 lead is reproduced with the original actor's actual mass computation.
Original 0x98220 converts float dense matrices to doubles, calls 0x97680 Jacobi,
and rounds principal values/axes once. Source axis association is retained.
NativePrincipalInertia.h reconstructs that path using explicit double operations.
It contains no measured constants, fixture names or replacement expected outputs.
The generic SDK algorithm constants are taken from its arithmetic instructions.

Actual stopping threshold is `(0.5*sum(diagonal²)+sum(upper_off_diagonal²))*epsilon`,
where epsilon is 9.999999949504854e-13. The earlier exploratory v46 model used
twice this threshold. All 50 actual thresholds per repeat now match exactly.
The cutoff is 9.999999682655225e-21; at most 100 sweeps occur. The original calls
are cyclic in source axes, and the returned eigenvalues are not sorted.

`RRR3D_EXPERIMENT_NATIVE_PRINCIPAL_INERTIA=1` enables the candidate on the full
v45 adapter/SDK chain. The dense aggregation is unchanged. It stays off by
default. `run_isolation.py --contact-v47-principal-inertia` selects it with that
chain. Original game and SDK files on disk are unchanged.

## Evidence and scope

- All **24 actual original matrices** (six inverted offsets, all 17 shipped
  chassis, one raised-wheel case) yield **288 exact output float fields** in a
  standalone C++ reconstruction: three principal moments and nine axis entries.
  O0 and O2 builds agree with the original. The native traces repeat twice and
  preserve full untraced physical state histories for all 24 cases.
- Mac **6/6 inverted fixtures now match all120 steps**, in both repeats.
  The original case-5 residual at tick50 is closed in this candidate. All
  **20 actual solver rows × eight fields** at that step match the independent
  original private trace; previously 64 field comparisons differed.
- Mac retains **72/72 old tilted contacts**, **6/6 previous offset cases** and
  **189/189 force-isolated scenarios**. Default-off controls equal the actual
  older v45 binary in both repeats for all three comparison scopes.
- Full Debug app builds and signs with 1200 assets/14 movies. **53/53 CTest**
  checks pass (12.56 seconds). No fresh GUI race is certified.

## Acceptance remains pending

The broader Mac physical comparison still has **18 residual cases**. The
`monstertruckBoss/spin-mixed` maximum position error increases from
0.000010690174229377852 to 0.000024914129746713667. This reproducible regression
prevents acceptance as a production correction. Current capture comparisons
report it explicitly; the package verifier requires it to remain visible.

Fourteen of the 24 Mac dense tensors already differ before decomposition.
Tracing each original box/wheel mass provider locates **62 differing shape
inertia tensors among 120 actual shape records**. Thus improving decomposition
alone cannot finish the requested parity. The complete goal remains active.

Next reconstruct original box integrals at 0x1370e0, cylinder integrals at
0x136bd0 and translation at 0x135e60, then verify every actual shape entry,
compound tensor and principal value before recoupling dynamics. Avoid adjusting
engine power, chassis mass or force coefficients to compensate for arithmetic.

## Reproduce

Apply published v43 SDK/adapter, v44 and v45 adapter patches, then adjacent
native-principal-inertia-v47.patch. The adjacent base manifest records exact
source hashes; before/ contains the byte-exact v45 adapter. Rebuild the Mac probe.
The capture tools require the original SDK/core/CRT locally and preserve original
executables; build-trace.cmd compiles the private Windows observer. The separate
shape-tracer observes each mass-integral provider before compound aggregation.

From the investigation archive on macOS, run:

```
python3 source/tools/calibration/experimental/native-principal-inertia-v47/verify.py --package .
```

The verifier checks all bundled hashes, the actual original stopping thresholds,
compiles the generic decomposition at O2 against original matrices, compares
both Windows traces to their untraced runs, and checks the Mac physical/control
captures. Passing this audit validates the investigation data and its stated
limitations; candidate acceptance remains pending upstream mass correction.
