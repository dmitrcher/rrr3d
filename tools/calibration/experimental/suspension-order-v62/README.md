Default-off investigation over the exact active v60 sources. Apply this guarded snapshot, then the guarded creation-order-v64 snapshot; rebuild all SDK consumers after each change.

V61 changes quaternion inverse translation and material/callback frames. V62 adds material coefficients computed before the user callback and bounded, opt-in wheel/contact force telemetry. Neither changes engine power, mass, springs, grip or other vehicle parameters. The coefficients reproduce the original material preparation instead of fitting an observed friction value.

The original all-hard-stops-before-all-springs V62 prototype is REJECTED as a general rule. It greatly improves bump-crossing and curb-side, but worsens xCar suspension-drop: position maximum 0.00000821 -> 0.0425162. The full 608-case audit retains all 65 exact cases, with 543 still open. Compared with v60, maximum position error improves in 264 cases, worsens in 47, and is unchanged in 297. Independent 291-case gates and 53 build checks pass; passing those gates does not certify the remaining broad matrix.

The new Native landing traces independently show per-wheel stop/spring interleaving. Native object creation indices also explain why a later-created stop appears before an older spring group. See creation-order-v64 for the replacement hypothesis and its separate measurements. The rejected V63 retained-contact-only hypothesis is recorded there as failed data.

These are one-car engine fixtures with fixed input and no AI, plus separately scripted two-body cases in the broad matrix. They do not establish full-game Windows/Mac parity. Experiments remain off by default and ordinary installed games are not replaced. Raw physical history, source snapshots and manifests accompany the research archive; prior Native Golden is retained unchanged.

Contact-only reconstruction in physics-v62-native-forward-solver-audit.json omits wheel rows at group boundaries. Its boundary errors are not evidence of a contact arithmetic bug.
