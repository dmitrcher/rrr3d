# v39 contact-step diagnostic checkpoint

No adapter or solver physics changed. See the isolation progress document for scope.
`solver-comparison.json` compares all second-step solver rows against original native
Windows point traces; `projection-audit.json` records the independently reproduced
projection rounding difference. Source v38 and its release remain unchanged.

Compile `projection-probe.cpp` twice, with C++17, `-O0`, the tested v38 physics
header include directory, and respectively `-ffp-contract=on` / `off`.
Input is 19 float values: column-major transform (9 rotation, 3 translation),
plane normal (3), local vertex (3), target plane coordinate (1).
Output is flattened translation (3), projected vertex Z, and separation.
This helper uses recorded inputs for diagnostic formula validation; it is not
the independent Mac simulation and is not a production correction.
