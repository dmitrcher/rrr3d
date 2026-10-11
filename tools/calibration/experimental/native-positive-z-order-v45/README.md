Default-off v45 original upper local-Z polygon order experiment.

Requires the complete v43 source/SDK chain and v44 patch. Apply the adjacent
three-file patch to the recorded baseline; full before/after copies and hashes
are included. Enable RRR3D_EXPERIMENT_NATIVE_POSITIVE_Z_FACE_ORDER=1, or use
run_isolation.py --contact-v45-positive-z-face-order for the full chain.
Production adapter and force coefficients are unchanged.

72/72 original tilted contacts match in two repeats. 189 isolated and six old
COM/shape cases remain exact. Five of six new inverted offset cases match; one
residual is traced to original local principal inverse inertia. The goal remains
active. See docs/PHYSICS_CONTACT_V45_2026-10-12.md for evidence and limitations.

The four verifiers in the docs reconstruct from raw data. Capture tools accept
explicit probe, reference, profile and environment paths. capture-expanded-mac
captures the new six cases; capture-old-expanded-mac preserves the previous six.
Pass --flags INITIAL_COM,ACTOR_ORIGIN,ACTOR_OUTPUT,SHAPE_TRANSLATION,MASS_DOT_ORDER,FROZEN_CM_EXACT.
Their disabled controls retain v44 and remove only the positive-Z order flag.
capture-mac uses --all or --cases and supports --trace/--trace-step.

Focused geometry/inertia tracers read actual original calls at a selected
MOTORROCK_NATIVE_TRACE_STEP (50 here). Source/build files are bundled; Windows
SDK, VC90 CRT, exact original PhysXCore and separate v45 fixture probe are needed.
The latter adds fixture definitions only; original Windows executables and
simulation implementation are not replaced. Captures prove tracing leaves
all120 native physical states unchanged.
