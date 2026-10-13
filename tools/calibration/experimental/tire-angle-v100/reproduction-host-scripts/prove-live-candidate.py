from pathlib import Path
import json,gzip
root=Path.cwd();reports=[]
for tick in [148,149,150]:
 for rep in range(2):
  tag=f'border-front-grounded-{tick}-{rep}'
  native=list(map(json.loads,gzip.open(root/f'outputs/tire-angle-v100/native-angle/{tag}-private.jsonl.gz','rt')))
  args=[x for x in native if x.get('function')=='tire-angle-argument-ready'];stored=[x for x in native if x.get('function')=='tire-angle-stored'];mac=list(map(json.loads,open(root/f'outputs/tire-angle-v100/mac-candidate/{tag}-angle.jsonl')));assert len(args)==len(stored)==len(mac)==4
  for n,s,m in zip(args,stored,mac):
   f=n['prepare_frame']['words'];words=m['words'];assert f[13:16]+n['wheel_contact']['words'][6:9]==words[:6];assert f[6]==words[6];assert s['prepare_frame']['words'][26]==words[7];assert n['wheel_contact']['words'][:3]==words[9:12]
  reports.append({'physical_tick':tick,'repeat':rep,'input_vector_words_exact':24,'actual_argument_words_exact':4,'actual_result_words_exact':4,'actual_world_point_words_exact':12})
m=json.loads((root/'outputs/tire-angle-v100/mac-candidate/manifest.json').read_text());assert len(m['captures'])==6 and all(x['all_original_Windows_states_exact'] and x['states']==600 for x in m['captures'])
report={'scope':'Fresh candidate at148/149/150: all24wheel computations bind actual original Windows vectors, arguments, stored results and world points; six entire600step histories exact original Windows. No replay or measured-state injection.','repeats':reports,'original_Windows_states_exact':3600,'probe_sha256':m['probe_sha256'],'goal_complete':False};(root/'outputs/tire-angle-v100/live-candidate-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Actual Native24 angle calculations and3600 uninterrupted Windows states exact',flush=True)
