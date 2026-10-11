Experimental v44 callback triangle-normal reconstruction.

Requires the complete v43 source/SDK chain, documented in
`docs/PHYSICS_CONTACT_V43_2026-10-12.md`. Apply the adjacent two-file patch to
that exact source; before/after hashes and copies are provided. Production
adapter and force coefficients are unchanged. Enable
`RRR3D_EXPERIMENT_NATIVE_CALLBACK_TRIANGLE_NORMAL=1`, or use the isolation
runner `--contact-v44-callback-triangle-normal` to enable the full chain.

67/72 tilted contacts match Windows in two repeats; five remain. All 189
isolated scenarios and six expanded COM/shape scenarios remain exact.
See `docs/PHYSICS_CONTACT_V44_2026-10-12.md` for scope and limitations.

Run verify.py, verify-cache.py and verify-patch.py as documented. Capture scripts
require a rebuilt Mac probe, Windows reference JSONL, profile and environment
manifest. `capture-mac.py` accepts --all or --cases, optional --trace and
--trace-step, and preserves v43 in its disabled control. For expanded captures,
pass --flags INITIAL_COM,ACTOR_ORIGIN,ACTOR_OUTPUT,SHAPE_TRANSLATION,MASS_DOT_ORDER,FROZEN_CM_EXACT.

The material-stage trace includes raw local/world quaternions, material basis,
callback seed and before/after axes, local quaternions, solver quaternion and
public callback normal. It is opt-in and does not consume reference results.
