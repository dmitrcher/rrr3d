# Moving wheel support: V77 causal investigation

The first differing physical state of `pair-stock-pole-1` is tick28. The original Windows pending-wheel contact queue creates tire rows first, then spring rows in wheel order `[1,3,0,2]`; generic constraints traverse descending creation IDs, yielding springs `[2,0,3,1]`. The current Mac traversal is `[3,2,1,0]`.

Changing only traversal of the same original prepared rows exactly reproduces all20 live Mac spring impulse, linear-velocity and angular-velocity transitions at this onset. Original traversal continuously reproduces all60 Native body transitions (720 raw float32 words), without resetting bodies between rows. This establishes an ordering cause in this fixture. **Captured permutations are diagnostic interventions, not implementation rules. No physics implementation or parameters changed.**

## Reproduce the numerical proofs

```sh
python3 proofs/verify-math.py
```

Clang++ and Python3 are required. Same-input generic scalar solver and both-body application:120 recorded operations across ticks28/29,1920 raw float32 words exact at O0/O2. Two original repeats have identical address-free arithmetic fixtures. The header matches the existing V53 helper; the body application in the proof explicitly follows original scalar operation ordering. This does not claim all live Jolt application paths are proved at arbitrary inputs.

`captures` contains four final kernel histories and two contact-manager creation histories, all losslessly gzipped, plus Mac onset wheel diagnostics. Every physical capture preserved all300 original Golden samples. The immutable original SpringProbe/PhysXCore hashes were checked before/after each capture batch. Source tick uses closed AFTER_SOLVE samples; Mac private wheel step29 is empirically aligned to physical tick28 via prep pose equal to AFTER_SOLVE tick27.

The debugger labels `spring-solve-result` capture scalar impulse result **before body application** (RVA1e51f3); new `spring-solve-applied` captures after both-body application (RVA1e534e). Do not use the earlier result event as an after-application body snapshot.

## Remaining implementation work

Recover the original broadphase created-pair order, then implement general pending-contact ordering and validate focused trajectories,608 complete cases,291 independent gates and gameplay. Stack chain: manager ctor14170 <-18bbef <-18c135 <-18c62b <-18c73a <-18cba6 <-1acffd. Driver1acfdb consumes original created pairs in16-byte records; its backend API virtual +0/+4 counts created/deleted pairs, +8 fills created pairs. The queue itself is appended at14c3db and processed twice at173e95; generic springs traverse linked objects in descending creation order at7f8b4.

Full-suite accepted research result remains116/608 strict with492 open. No new broad sweep was run because no live physics code changed. The goal is incomplete; installed game was not promoted and no new game installer was built.

`causal-audit.json` also describes early exploratory captures not included here: an inherited V52 created-object handle lookup was invalid for these backend spring objects and was removed from final tracers. Only descriptor, wheel-backend, generic solver and final manager captures are used for conclusions. `disassembly` contains bounded excerpts of the immutable original DLL, not a complete DLL dump.
