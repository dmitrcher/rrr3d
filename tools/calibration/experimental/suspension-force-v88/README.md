# V88 native suspension force store

PhysX stores final suspension impulse times a separately rounded inverse time step. The old Mac path divided directly by time step; at identical impulse215.91473388671875 this produced12954.8837890625 instead of Native12954.8828125. The independently default-off helper preserves the rounded reciprocal and product. No engine power, friction, suspension stiffness, damping or other coefficient changes.

Full608actualcase trajectories255864states,2exactMacrepeats: **242strict/366open**, versus119strict before. **123newfully exact cases; zero lost exact cases.** Newly closed include settle,wheel-drive,coast,brake,suspension-drop,reverse-drive,mass/inertia/grip variants and multiple shipped-car mobility cases. The comparison uses unchanged physical float32 fields and immutable Windows data; no relaxed tolerances. Error maxima in some still-open trajectories worsen; the complete regression audit records them.

Native final4spring lambdas atphysical30 and actual Native callback input forces at31 reproduce4force words O0/O2; olddivision fails1. Continuous high-load case now exact0through43, first44; stock settle600states completely exact. This is research-profile progress, not complete game physics certification.

All291independentcontrol/candidate gates pass2repeats;53CTests,full signed game build,solo real-map smoke and rendered AI functional smoke pass. AI smoke is not numerical parity evidence. Normal installed game baseline remains unchanged. The source overlay is only2adapterfiles after exact V87; apply-experiment.py validates every base SHA before mutation, supports idempotent apply/restore and deletes the new header on restore. Rebuild required. Full research environment and source snapshots retained.

Run `verify-native-store.py` to derive fixtures from actual Native observer captures and compile/execute actual helper at O0/O2. Run `verify-checkpoint.py --probe PATH` for fresh1200-state reproduction against both retained Mac runs and immutable Windows subset. Profile paths are resolved relative to this directory.

Next physical44 Native spring order changes from descending to ascending wheel construction order; matched Mac prepared masses/Jacobians still traverse descending. Investigate actual static contact/row allocation lifetime and queue ordering. Do not hardcode frame44 or change coefficients. Goal remains active with366open cases.
