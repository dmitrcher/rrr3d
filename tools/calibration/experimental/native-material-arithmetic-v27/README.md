# Native material arithmetic v27

These helpers isolate numerical instructions from the game and from the private
PhysX solver. `roots.cpp` executes actual x86 SSE rsqrt in the Windows ARM VM;
`neon-roots.cpp` executes eight actual NEON estimate/refinement variants on Mac.
They do not look up fixture names or load reference answers into the calculation.

The variant with two iterations of `r * vrsqrts(x, r * r)` matched every recorded
Windows rsqrt estimate: 1213 discovery inputs and 8192 independently generated
holdout inputs. Other refinement orders did not. Correctly rounded reciprocal
sqrt is not an adequate replacement. The input files include the actual float32
values and are retained to reproduce the result. This is a finite certification
for these machines, not an exhaustive assertion about all float values, Intel,
AMD, subnormals or exceptional operands.

`seeds.cpp` constructs material frames using triangle vertices and actor basis,
then compares several plausible normalization/projection/encoding sequences.
`seed-expected.json` is the separate SDK reference. No complete general seed
algorithm was found. `candidate-driver.cpp` evaluates the current portable
candidate from `../SourceMaterialFrameCandidate.h`. Its flat first-contact seed
and modified orientations match all 12 native flat-contact cases, but tilted
cases still differ. Quaternion comparisons treat q and -q as equivalent.

Build Windows helpers with VC2008 x86 by running `build.cmd`. The helper sources
are C++98; `/fp:precise /arch:SSE2` is intentional. The duplicate
`ContactFrameScalar.h` is an unchanged copy of its parent experimental header,
so the Windows helper directory can be transferred and built independently.
These executables need no game assets or PhysX DLLs.

On Mac ARM:

```sh
clang++ -std=c++11 -O0 -ffp-contract=off neon-roots.cpp -o neon-roots
./neon-roots < roots-input.txt > discovery-mac.txt
./neon-roots < roots-input.txt > discovery-mac-repeat.txt
./neon-roots < roots-holdout-input.txt > holdout-mac.txt
./neon-roots < roots-holdout-input.txt > holdout-mac-repeat.txt
clang++ -std=c++11 -O0 -ffp-contract=on -I .. candidate-driver.cpp -o candidate-driver
./candidate-driver < candidate-input.txt > candidate-first-contact.txt
```

On Windows, run `roots.exe < roots-input.txt` and
`roots.exe < roots-holdout-input.txt`, saving two separate repeats of each.
`verify_rsqrt.py` checks matching float32 inputs, every output estimate and both
platforms' numerical repeats:

```sh
python3 verify_rsqrt.py windows.txt windows-repeat.txt mac.txt mac-repeat.txt --out report.json
```

The runtime experiment is strictly `RRR3D_EXPERIMENT_MATERIAL_FRAME_CANDIDATE=1`.
It remains disabled by default: exact instruction estimates do not prove exact
material frames, contact geometry or solver response. The audit preserves
regressions and the rejected scalar-root version rather than promoting a partial
model to the installed game.
