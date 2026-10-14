from pathlib import Path
import json,gzip,struct
v=lambda w:struct.unpack('<f',struct.pack('<I',w))[0];b=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
checks=[];wakecalls=[]
for rep in [0,1]:
 native=[x for x in map(json.loads,gzip.open(f'outputs/startup-import-v130/native-energy64/run{rep}-private.jsonl.gz','rt')) if 'energy_body' in x and abs(v(x['energy_body']['words'][120])-.0005)<1e-8];frames={}
 for x in native:frames.setdefault(x['sample_tick_v71'],{})[x['function']]=x
 src={x['step']:x for x in map(json.loads,open(f'outputs/startup-import-v130/source-pair-loss-fixed/track64-on{rep}.energy.jsonl'))}
 failures=[]
 for tick,d in frames.items():
  n=d['energy-sleep-entry']['energy_body']['words'];store=d['energy-wake-store'];s=src[tick]
  test=d.get('energy-threshold-test');energy=0 if not test else test['xmm_words'][0][0];threshold=0 if not test else test['xmm_words'][1][0]
  for name,a,e in [('interactions',s['interactions'],sum(n[8:12])),('wake',b(s['wake']),store['xmm_words'][0][0]),('energy',b(s['energy']),energy),('threshold',b(s['threshold']),threshold),('pending',s['pending'],bool(d['energy-sleep-exit']['energy_body']['words'][84]&0x100000))]:
   if a!=e and (name in ['interactions','pending'] or a&0x7fffffff or e&0x7fffffff):failures.append(dict(tick=tick,field=name,source=a,native=e))
 checks.append(dict(repeat=rep,frames=len(frames),failures=failures))
 r=[x for x in map(json.loads,gzip.open(f'outputs/startup-import-v130/native-wake64/run{rep}-private.jsonl.gz','rt')) if x.get('function')=='wake-request' and 'wake_body' in x and abs(v(x['wake_body']['words'][120])-.0005)<1e-8]
 energycaller=next(x['wake_stack']['words'][0] for x in r if x['sample_tick_v71']==12);core=energycaller-0x1b2cde
 for x in r:
  tick=x['sample_tick_v71'];st=x['wake_stack']['words'];wakecalls.append(dict(repeat=rep,tick=tick,caller_rva=hex(st[0]-core),requested_counter_word=st[1],previous_counter_word=x['wake_body']['words'][103]))
print('checks',[(x['frames'],len(x['failures'])) for x in checks]);print('wake calls',wakecalls)
Path('work/startup-import-v130/pair-loss-causal-proof.json').write_text(json.dumps(dict(scope='Actual original wake requests and pre/post energy helper outputs versus geometry-derived Source pair lifecycle; public900states preserved by observers.',checks=checks,wakecalls=wakecalls,model_uses_fixture_ids_or_ticks=False,whole_game_verified=False),indent=2)+'\n')
