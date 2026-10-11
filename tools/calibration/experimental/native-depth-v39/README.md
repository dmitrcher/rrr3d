# v39 independently calculated polygon depth

`RRR3D_EXPERIMENT_NATIVE_POLYGON_DEPTH=1` enables local, separately rounded
matrix/projection arithmetic. It requires the full v38 experimental chain.
The flag is default-off; this is not a production promotion.
One-file patch `../native-polygon-depth-v39.patch` applies to the exact v38
experimental header recorded in its `-base.json` contract.

`capture-mac.py` captures candidate on/off in two repeats, with v38 controls.
`compare-solver.py --measurements DIR --reference-dir DIR --step N` audits
native solver fields using original Windows point traces, for N=0,1,2.
`verify.py --measurements DIR --probe PATH` verifies repeats, force isolation,
control preservation, 189 staged scenarios, 72 long contacts, and Windows
trace preservation. Native Windows references are measurement inputs to the
verifiers; the independent Mac simulator never consumes them.
`verify-model.py --measurements DIR --helper PATH` separately checks the
projection formula at recorded native inputs. The helper compiles with
C++17, -O0, -ffp-contract=on and the included tested header directory.

Results: 16/72 long contacts exact (10 newly exact), 56 remaining.
Position maximum improves in 42 cases and worsens in 18; velocity improves
in 48 and worsens in 12. All 189 previous scenarios pass. Full source build
and 53 CTest pass. New visual race verification is not included.
