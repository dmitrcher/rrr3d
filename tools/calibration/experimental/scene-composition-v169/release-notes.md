MotorRock V169 research checkpoint closes all four currently captured real-map histories: tracks0/16/48/64 each match900/900 original float32 physical states in both repeats. Same serialized scene, one car/no AI, fixed identical commands. The previous first differences420/720 came from actor wake/interaction lifecycle and wheel query order.

The implementation derives actor-owned interaction order including suppressed markers, finite map-plane SAP bounds, pair retirement and first non-retiring allowed manager dispatch. No measured permutation or Windows physical output is used as a runtime input, and no mass/power/grip/suspension parameter is tuned.

Final full608-case gate:608/608 strict float32 histories exact,255864 states each, two repeats identical. All53 CTests pass and2700 defaultOFF states preserve V160.

Two isolated slope regressions exposed a scene-composition mismatch: the original isolated SDK probe has no map death plane. V169 makes that scene contract explicit while ordinary maps retain their plane. Paired600-state negative controls and final608-case comparison are included.

The package contains a combined guarded eight-file Source overlay over verified V160, unchanged V154 SDK prerequisites, frozen Mac numerical probe, own immutable Windows WorldProbeV133 helper, four fixtures, environment and causal/regression evidence. Original proprietary DLLs remain external. Every switch defaults off; this is not the ordinary installed Mac app.

Scope still open: production MotorProgress/controller/wake propagation and FP context, additional gameplay coverage and rendered race verification. This checkpoint does not declare the overall parity goal complete.
