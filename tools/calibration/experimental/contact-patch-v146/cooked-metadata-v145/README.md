# Cooked contact metadata V145 — investigation only

After all 900 simulation steps, a separate metadata probe queries mesh 25 through the original SDK `NxTriangleMeshShape::getTriangle`. Neither the original WorldProbeV133 nor the SDK DLLs are modified. Two repeats preserve the complete original public history and all input/executable/DLL hashes.

All 32 cooked triangle vertex arrays match their Source input arrays exactly, with a cooking reorder recorded in proof.json. Source faces 30/31 are native faces 24/25. Their distinct triangle normals also match the portable triangle-plane calculation. Thus welding or vertex rounding does not explain the shared normal seen in the collision output.

Native flags are 17 (active edge 01, boundary edge 01), versus Source's coplanar-only active mask 3. Replacing mask 3 by 1 in the offline manifold preserves its four contacts and discrepancies, so this separate edge classification mismatch does not explain this first divergence.

The initial helper left unused edge-normal output slots uninitialized, causing a repeat comparison failure even though public histories, vertices and flags matched. The final helper zero-initializes every output slot; the final two full metadata records are repeat-identical. Only the initialized captures are authoritative.

No game or runtime physics correction is made in this investigation. Full parity remains incomplete.
