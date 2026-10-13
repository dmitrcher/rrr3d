from pathlib import Path
import gzip,json,struct
root=Path.cwd();out=root/'outputs/tire-anchor-v91';bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];floats=lambda ws:[struct.unpack('<f',struct.pack('<I',w))[0] for w in ws];reports=[]
for rep in range(2):
 native=[json.loads(s) for s in gzip.open(out/f'native-anchor-window/force-devildriver-engine3-lateral-58-{rep}-private.jsonl.gz','rt')];gates=[r for r in native if r.get('function')=='tire-anchor-gate'];wheelids={r['ebx']:i for i,r in enumerate(gates[:4])};assert len(wheelids)==4
 mac=[json.loads(s) for s in (out/f'mac-anchor-control/control-{rep}-anchor.jsonl').open()];lookup={(r['step'],r['wheel']):r for r in mac};comparisons=[]
 for r in gates:
  tick=r['sample_tick_v71'];wheel=wheelids[r['ebx']];w=r['wheel_contact']['words'];m=lookup[tick,wheel];ready=next(z for z in native if z.get('function')=='tire-anchor-bias-ready' and z.get('sample_tick_v71')==tick and z['ebx']==r['ebx']);old=w[22:25];nativebias=ready['prepare_frame']['words'][63];currentexact=list(map(bits,m['current']))==w[:3]
  assert currentexact,(tick,wheel,'Contact point differs')
  comparisons.append({'physical_tick':tick,'wheel':wheel,'current_point_float32_exact':True,'native_previous':floats(old),'mac_previous':m['previous'],'previous_float32_exact':list(map(bits,m['previous']))==old,'native_lateral_error':floats([nativebias])[0],'mac_lateral_error':m['lateral_error'],'lateral_error_float32_exact':bits(m['lateral_error'])==nativebias})
 for wheel in range(4):
  assert not lookup[38,wheel]['has_contact'] and lookup[38,wheel]['valid_before'] and not lookup[38,wheel]['valid_after']
  assert lookup[44,wheel]['has_contact'] and not lookup[44,wheel]['valid_before']
  n=[r for r in comparisons if r['wheel']==wheel];nativeold=[bits(v) for v in n[0]['native_previous']];assert all([bits(v) for v in r['native_previous']]==nativeold for r in n)
 reports.append(comparisons)
assert reports[0]==reports[1]
summary={'scope':'Read-only same-history causal lifetime comparison. Mac callback counter directly labels physical tick before increment; solver trace counter is tick+1. All136 current worldpoints exact against Native, old static anchors differ after Mac invalidation38/recreation44.','native_same_input_formula_o0_o2_pass':True,'two_repeats_exact':True,'current_points_exact':len(reports[0]),'first_previous_anchor_difference_excluding_initialization':next(r['physical_tick'] for r in reports[0] if r['physical_tick']>19 and not r['previous_float32_exact']),'first_bias_difference':next(r['physical_tick'] for r in reports[0] if not r['lateral_error_float32_exact']),'at58':[r for r in reports[0] if r['physical_tick']==58],'comparisons':reports[0],'goal_complete':False}
(out/'anchor-lifetime-causal-proof.json').write_text(json.dumps(summary,indent=2)+'\n');print({k:v for k,v in summary.items() if k not in ['comparisons','at58']})
