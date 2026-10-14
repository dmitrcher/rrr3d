# Relative contact arithmetic V147 — investigation

This supplies a portable implementation of the original Core relative-pose arithmetic, plus an isolated reconstruction of map0 body/border contact generation at physical sample 342. No active game physics file or coefficient was modified.

`include/NativeRelativeContactV147.h` derives each relative quaternion, constructs its scalar float32 rotation matrix, and independently rotates the difference in origins for translation. The original Core uses this path instead of multiplying/transposing precomputed matrices. The product scalar sums Y, Z, X. Inverse translation uses a distinct X, Z, Y dot product and explicit operation association. The accompanying original DLL disassembly records the source of the operation order.

`prove-relative.py` compiles with O0 and O2, then compares all 24 transform words against the actual original polygon wrapper inputs. Maps0/16 supply two independent relative poses; each is captured twice (map0 supplies two polygons with the same transform). All 12 per-polygon/optimization comparisons are exact. Map48 sample358 has a geometry dispatch but no polygon-wrapper entry, so it contributes no relative-transform check. Six native runs retain all 900 original public states. The finalized capture manifest correctly lists all three maps; immediate pre/post fixture hashes were recorded for map0, while maps16/48 were audited after capture against the original V133 hashes. This distinction is retained in the manifest.

`prove-manifold.py` tests all eight combinations of relative quaternion transforms, shared contact plane and vertex-based plane distance at O0/O2. The starting mesh-local reconstruction has 21/28 exact words; transforms plus a shared plane give 27/28; recomputing the plane distance at the current polygon's first vertex gives 28/28. All four points, normals and separations match the original pre-material buffer exactly. Source body position/quaternion at the input phase are independently verified equal to Native, and the mesh pose is derived through the existing Source placed-mesh conversion.

**Scope limit:** The offline fixture explicitly supplies the observed contact group containing raw faces30/31 and its first face plane. This isolates and proves the required arithmetic; it does not implement a general triangle grouping or representative-plane selection algorithm. Do not install that fixture-specific grouping into runtime code. The next step is to derive grouping and global SAT/representative-plane selection from the original path, then integrate the general calculation behind an experiment flag and rerun moving and isolated regressions. The current game remains V143 experimental / V73 installed, with the known moving discrepancies still open.

Run from any directory with Python3 and clang++:

```sh
python3 prove-relative.py
python3 prove-manifold.py
```

The included dependency closure is sufficient for these portable programs. Windows observer compilation uses the existing original VS2008/SDK environment; no proprietary SDK headers or DLLs are included here.
