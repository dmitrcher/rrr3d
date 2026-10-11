# v46 read-only principal inertia investigation

Source base is a188aacc7936a24f38d84d3cadfb275d5daf4ba5 (published v45).
No runtime source or coefficient is changed in this checkpoint.

The old mass helper used Jolt float eigenvalue decomposition and accepted a
relative approximation. A separate Mac executable reproduces that exact mass
helper and emits its dense matrix. The original Windows utility is fed the same
float matrix: five cases match, while case 5 gives source Z inertia
1436.4140625 against Jolt 1436.4139404296875.

Independent original actor tracing at 0x98220 and 0x982f1 verifies **all 54
actual input float entries** equal that Mac calculation, in both repeats. The
caller is 0x13a0d8, the original actor creation path. Each traced run preserves
all six cases' 120 physical states against the original untraced v45 baseline.
The defect is decomposition precision in these six fixtures, not aggregation.

Static original disassembly at 0x98220 copies floats into doubles, calls the
Jacobi implementation at 0x97680, and rounds principal moments/axes to floats
on return. It uses at most 100 sweeps. Its observed constants at RVAs 0x3315a8,
0x3315b0 and 0x3315b8 are 9.999999682655225e-21,
9.999999949504854e-13 and 0.5. An exploratory double Jacobi model matches all
six principal moments and all 54 returned axis matrix entries. This model's
convergence expression still needs actual-register verification and wider tests;
it is not installed into the runtime.

An apparent earlier discrepancy for case 0 X inertia is explained by the public
inertia accessor's reciprocal roundtrip: original internal 389.5772705078125
becomes public 389.5772399902344. The actual original dense input is exact.
Do not adjust mass to the rounded public accessor output.

The two Windows diagnostics compile with VC2008 and the original local SDK;
SDK headers and DLLs are not bundled. build.cmd produces DiagonalProbe.exe,
build-trace.cmd produces TraceMass.exe. Copy original loader/core/CRT alongside
the executables. input.txt contains six row-major dense input matrices:

```
DiagonalProbe.exe input.txt result.txt
TraceMass.exe mass.jsonl <v45-SpringProbe.exe> physical.jsonl list:pose-v45-inverted-0,pose-v45-inverted-1,pose-v45-inverted-2,pose-v45-inverted-3,pose-v45-inverted-4,pose-v45-inverted-5
```

The tracer requires exact PhysXCore SHA-256
c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb.
Original binary files on disk remain unchanged; debugger breakpoints only
observe arithmetic. measurements includes raw records, losslessly compressed full physical captures,
an untraced reference, and export/source hashes. Run `python3 verify.py` to check
every measurement hash, all actual matrix inputs, the exploratory model outputs,
and both full physical runs against the independent untraced reference.

Next: reproduce the double algorithm's stopping/rotation operation order in a
separate candidate, verify principal moments for every shipped chassis, then
repeat contact/isolation and broader dynamic tests. The full parity goal remains
active, and production is not promoted.
