# Exact COM investigation v50

See [the evidence report](../../../../docs/PHYSICS_EXACT_COM_V50_2026-10-12.md).

`RRR3D_EXPERIMENT_NATIVE_EXACT_SHAPE_COM=1` is an experimental default-off fix for the target-minus-child-plus-child COM rounding. It yields 290/291 exact current fixtures, with `body-slide-x` still different. No force coefficient is adjusted. This is a checkpoint toward the active full-parity goal.

The adjacent patch applies to the before hashes in `base.json`. Ordinary shape serialization remains byte-identical. Optional exact-COM records use an extension that requires the patched reader. The SDK serialization probe was compiled against the actual old implementation and the current implementation separately.

`RRR3D_CONTACT_ITERATION_ROWS=1` adds read-only per-row normal/friction telemetry with `RRR3D_CONTACT_ROWS=1`; use `RRR3D_CONTACT_NATIVE_TRACE_STEP` to filter. Captures prove it does not alter physical histories. The Windows tracer requires the original user-supplied SDK/probe and VC2008 x86; proprietary DLLs are not bundled.

Run the verifier and fresh replay with the downloaded evidence package as described in the report. Verification does not imply a GUI race, the broad historical suite, or all physics discrepancies have passed.
