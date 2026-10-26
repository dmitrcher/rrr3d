# Next work without VM

1. Keep V186 and V187 experimental; never replace the immutable Windows references or infer their missing values from Mac output.
2. Audit recorded original WheelsProgress contact data against Source controller input phase. The complete original OnFixedStep clears flags, then obtains wheel contacts, stabilizes/steers, then calls JumpProgress. Compare actual SDK completed-contact phase, not an assumed current/previous-frame label.
3. Reuse recorded real-wheel contact fixtures for front/rear/both/none steering gates where available. Keep scalar contact tests distinct from real contact evidence. Shifted COM creation and edge/gimbal behavior remain open.
4. Original source establishes timer constants/order, but native continuous GameCar step sequences are still unrecorded. Continue static/source contract checks while keeping the VM stopped; do not claim native runtime parity from those checks.
5. Ordinary installed application/profile stays unpromoted until full gameplay, contacts/AI and visual comparisons are verified. Goal remains active.

Read-only host inventory also found CrossOver.app and Whisky.app already installed. Investigate whether an isolated, headless compatibility prefix can run the owned original Windows SDK probes without a VM. This route must first reproduce recorded golden controls; do not assume compatibility execution is reference-equivalent or modify existing user bottles.

Confirmed host inventory: CrossOver CLI reports 26.3.0 (build26.3.0.39832) and work/controller-stabilize-v185/native/StabilizeProbeV185.exe exists. The observer hardcodes C:\MotorRock\game-debug and requires original DLL/PDB/SDK dependencies. Create/use only a task-owned compatibility prefix and stage hash-verified files there; existing user bottles remain untouched. No Windows executable has been run through CrossOver in this checkpoint.
