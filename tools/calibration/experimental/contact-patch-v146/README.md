# Contact patch inputs V146 — investigation only

Read-only process observer captures the original SDK polygon wrapper at RVA 0x225c50, edge submission, and pre-material point transport at physical sample 342. The original WorldProbeV133.exe, SDK DLL, fixture and complete 900-state public history remain unchanged in two repeats. Captures in native-step342-schema-fixed are authoritative; the first capture used incorrect pointer field sizes and is retained locally only as a diagnostic.

The wrapper uses 16 stack arguments: count0, vertices0, indices0, world0, plane0, count1, vertices1, indices1, world1, plane1, worldSepAxis, transform0to1, transform1to0, feature1, buffer, flipNormal. There are no explicit rotation-matrix arguments in this original ABI. The observer field sizes now follow this layout.

Native cooked faces 24/25 (raw Source 30/31) have different individual face normals, but the polygon wrapper receives the same first-face plane (all four words) and the same world normal for both. This proves shared-plane input before clipping; changing only the final output normal would miss separation calculations.

The offline Source reconstruction already matches all 12 output point words. Its relative box translation matches all three native words, but 4 of 9 rotation words differ. The separately provided native inverse transform differs in 7 words from a matrix transpose plus translated inverse. The exact arrays and differing indices are in proof.json. Relative orientation must be derived from the original quaternion arithmetic before implementing a runtime correction; do not fit coefficients or inject captured values into the game.

Next: reconstruct both relative transforms from identical original poses, then reproduce common-plane grouping. Compare all points, normals and separations before running moving and isolated regressions. No runtime physics change or game promotion is included here; full parity remains incomplete.
