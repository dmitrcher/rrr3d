from pathlib import Path
import argparse,json,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=Path,required=True);a=p.parse_args();out=a.measurements
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
report={}
for stage,count in [('00',9),('01',108),('02',48),('03',24)]:
 paths=[out/f'calibration-isolation-stage{stage}-run{r}-v33-mac.jsonl' for r in range(2)]
 captures=[compare.load(p,'mac-jolt-metal') for p in paths];assert captures[0]==captures[1]
 assert len(captures[0])==count
 result=compare.compare(out/f'reference-isolation-stage{stage}-run0-v22-windows.jsonl',paths[1])
 remaining=len(result['exact_parity']['cases_remaining']);assert remaining==(12 if stage=='03' else 0)
 (out/f'physics-v33-isolation-stage{stage}-report.json').write_text(json.dumps(result,indent=2)+'\n')
 report[stage]={'cases':count,'remaining':remaining,'semantic_repeats_physical_states_exact':True}
control=compare.load(out/'calibration-derived-error-control-run0-v33-mac.jsonl','mac-jolt-metal')
old=compare.load(out/'calibration-native-arithmetic-ordinary-mask0-run0-v32-mac.jsonl','mac-jolt-metal')
assert all(control[n]==old[n] for n in control)
(out/'physics-v33-isolation-summary.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report));print('ordinary mask0 unchanged from v32')
