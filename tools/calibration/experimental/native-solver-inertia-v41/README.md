# Native solver inertia v41 (experimental)

Default-off refinement of the v40 experiment. See
`docs/PHYSICS_CONTACT_V41_2026-10-12.md` for evidence and remaining differences.
Enable with the isolation runner's `--contact-v41-solver-inertia`, which enables
the prerequisite v40 chain. The direct flag is
`RRR3D_EXPERIMENT_NATIVE_SOLVER_INERTIA=1`. Only diagonal local tensors use the
new arithmetic. Geometry, caching, coefficients and the original Windows physics
are unchanged. Synthetic fixtures have no AI, wheel forces or friction.

`../native-solver-inertia-v41.patch` applies to the two exact baseline files in
`before/`. `../native-solver-inertia-v41-base.json` supplies before/after hashes.
The full candidate sources are under `src/`; production has not been promoted.
`verify-patch.py --out RESULT.json` checks exact patch reconstruction.

Capture tools retain v39 output filenames for compatibility; the manifest calls
the candidate `native-solver-inertia-v41`. `ordinary` enables the new flag;
`control` disables it while keeping all v40 flags. The disabled run must equal
v40 states. `verify.py` checks capture hashes, repeated physical and diagnostic
states, all effective coefficient scales equal one, 189 isolated scenarios, and
72 long fixtures. It regenerates the three detailed solver comparisons.

`trace-contact.cpp` is a read-only diagnostic debugger derived from the v37
tracer. At the first solver iteration it watches four solver-body tensor writes
with Windows hardware write breakpoints. `build.cmd` builds it with VC2008 x86.
Use the capture script only against the verified original reference folder:
SpringProbe.exe SHA256
`8c331b7ea35ed6181c0f8dd52cddca451f60c817d381d360cb53852e8c49ed9d`,
PhysXCore.dll SHA256
`c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb`.
The tested tracer SHA256 is
`9b44b47a6b6deb74f0525a937a60a4ed1fb2868a21dfd1ec1d1cdcb7089b80cf`.
It writes no persistent changes to those original files. `capture-windows.py`
checks hashes and all resulting physical states against the immutable baseline.

Compile `probe.cpp` against the candidate physics header directory, e.g. with
clang++ -std=c++17 -O2 -ffp-contract=on. `verify-internal.py --measurements DIR
--probe EXE` compares the reconstructed formula with the observed original
writes, checking both original physical runs remain unchanged. Recorded inputs
belong only to this diagnostic verifier and are never imported by the adapter.

27/72 long cases match; 45 remain, with some partial regressions. This package
is an audit checkpoint, not an installer or a claim that all physics matches.
