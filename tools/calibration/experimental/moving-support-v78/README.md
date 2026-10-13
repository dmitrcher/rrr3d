# V78: recovering original contact creation order

The generic original endpoint sort is now reproduced in Python and C++ from immutable PhysXCore.dll RVA `0x327e0`. It uses unsigned encoded coordinate keys, a min/max type tie comparison, median-of-three partitioning and selection sort below six items. Shape IDs do not break ties. Its unstable swaps matter: four equal-height upper wheel minima end up in a different order from wheel creation.

The implementation accepts endpoint records only. It contains no fixture name, wheel permutation, physical parameter adjustment or recorded Windows body state. This is an independently verified research helper; it has not been connected to live Mac physics.

## Verification

Run from this directory on Mac:

```sh
python3 verify-v78.py --build /tmp/motorrock-v78-sort-proof
```

Python3 and Xcode command-line Clang++ are required. Six isolated original scenarios, each captured twice, yield 36 sort operations. Python and C++ at O0/O2 reproduce all 1,440 raw output words per implementation. Address-free initialization arrays repeat exactly. All 3,600 original physical samples from this six-scenario batch match the immutable Golden capture, and original probe/DLL file hashes remain unchanged. The scenes contain two deliberately configured physical bodies and no AI.

Separately, two captures of `pair-stock-pole-1` at physical tick28 verify the entire observed onset chain: initialization sort predicts upper wheel boundary order; the moving lower support crosses those boundaries in that order; adjacency insertion preserves the order; created-pair bitset traversal exports the same order. Predicted bounds IDs are `[8,10,7,9]`, corresponding to wheel indices `[1,3,0,2]`. These lists are outputs of the proof, not implementation inputs. V77 already established that the subsequent descending generic-constraint traversal is the cause of the first differing spring/body state.

## Layout and scope

`captures/sort-matrix` contains all twelve final initialization traces, their original physical histories and hash manifest. `captures/endpoints-onset` contains both final tick28 endpoint/pair-storage traces and physical histories. Logs are losslessly gzipped; unused allocator memory in debugger snapshots is not used by assertions. The sort tracer records initialization before any physical sample is emitted; tick28 uses the count of closed source samples. Bounded original DLL excerpts document the recovered operations. Debugger source and capture scripts are included for audit, with the original probe expected under the documented Windows path.

A tick-filtered exploratory initialization capture was empty because the sort occurs during scene initialization before the normal simulation callbacks. Final initialization traces capture the sort directly and confirm zero emitted samples. Only final verified captures are included here.

Remaining work is to reconstruct endpoint merge/update and adjacency lifetime as general persistent state, connect that state to contact-manager/row creation in a default-off Mac profile, then verify the first differing tick, complete trajectories and full regressions. No broad sweep was rerun for this helper-only change. The accepted V76 research result remains **116/608 strict, 492 open**. Live adapter and SDK are unchanged, installed game was not promoted, no new game installer was built, and the goal remains incomplete.
