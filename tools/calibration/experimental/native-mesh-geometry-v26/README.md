# Standalone contact geometry benchmark

This is the active v24 face-contact algorithm adapted to C++98 so the same
algorithm can be built on Mac and with VC2008 x86 in Windows. It does not run
PhysX or its solver. The real SDK's callback points are the separate reference.
The C++98 Mac build matched all numeric outputs of the active Debug geometry
helper in all 64 supplied initial states (306 contacts).

`native-input.txt` contains half-extents, actual position/matrix and native
triangle vertices/IDs. `expected-sdk-points.json` records the corresponding
native SDK points. Reconstructed separation is diagnostic, not an exact
measurement of SDK internal separation. The scope is these initial face
contacts, not arbitrary meshes, moving actors or the complete game.

Build on Mac:

```sh
clang++ -std=c++98 -O0 -ffp-contract=on mesh-geometry.cpp -o mesh-geometry
RRR3D_EXPERIMENT_UPPER_FACE_WINDING=1 \
RRR3D_EXPERIMENT_NEGATIVE_X_FACE_ORDER=1 \
./mesh-geometry < native-input.txt > mac.txt
python3 compare_geometry.py mac.txt --out mac-report.json
```

On Windows, `build.cmd` explicitly compiles and links four VC2008 variants:
default, strict, fast and SSE2. Use unchanged inputs for every run:

```bat
set RRR3D_EXPERIMENT_UPPER_FACE_WINDING=1
set RRR3D_EXPERIMENT_NEGATIVE_X_FACE_ORDER=1
set RRR3D_GEOMETRY_X87_BITS=64
geometry-default.exe < native-input.txt > windows-default-64.txt
```

`RRR3D_GEOMETRY_X87_BITS` accepts `24`, `53`, `64`; otherwise it leaves the
startup precision alone. The observed control word goes to stderr. Precision
changes here apply only to this helper process. They must not be installed as
a blanket game setting. VC2008 rejects combining `/fp:strict` with `/arch:SSE2`;
the supplied build uses these as separate variants.

All 16 precision/compiler combinations were repeated with identical numeric
outputs. Every combination keeps contact count and triangle/vertex order,
but none reproduces all native SDK coordinates. Some improve aggregate error
and regress individual points. No compiler mode is accepted as a complete
SDK geometry model. SSE normalization trials are retained only in the release's
research folder and are not part of the active game/probe implementation.
