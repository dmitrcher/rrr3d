# V162 read-only actor interaction and cold-wake audit

Six original debugger hooks record actual wheel query, creation/enqueue and manager lifecycle. Both maps and both repeats preserve every900 immutable WorldProbeV133 physical states and original executable/Core/Cooking hashes. No physics code changed in V162.

Native creation/enqueue agrees with Source every active tick before cold wake. At track16/420 Windows creates [1,3,2,0], Source [0,1,2,3]; track64/720 Windows [1,0,2,3], Source [1,0,3,2]. These arrays are observations only and never become runtime inputs.

Observer-partition records manager+0xc owner and its dense interaction vector. Historical processor/partition labels in raw trace names are misleading: this is the ACTOR-OWNED list, not a shared wheel processor. Canonical manager base is vectorPointer-8. Word7 is interaction TYPE, not active count:0 wheel manager,1 body manager,5 suppressed marker. Word5 is separate type-processor slot; word9/10 are collidable interfaces. Filtering actor list to wheel managers yields actual cold-wake creation order. Actor list stays unchanged through sleep, while type-processor slots are removed and rebuilt.

Initial sort observer captures full actual inputs/outputs before physics. Current Source endpoint-sort helper reproduces all12 Native sorting operations,5040 raw words, at both O0/O2. Original inputs include a death plane omitted by Source. Four initial observer runs also preserve all900 original states. Source initial-sort diagnostics preserve all900 V160 states.

Original raw captures, hash manifests, observer source/build scripts and derived history proofs are retained. All proprietary original DLLs remain external. V163..V166 implement the Source lifecycle reconstructed from these observations. Goal remains incomplete.
