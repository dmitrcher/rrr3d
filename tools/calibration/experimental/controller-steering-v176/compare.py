from pathlib import Path
import json,gzip,struct
p=Path('outputs/controller-steering-v176');native=[json.loads(x) for x in gzip.open(p/'native0.jsonl.gz','rt')]
def word(f):return struct.unpack('<I',struct.pack('<f',f))[0] if f else 0 # coordinate conversion maps both signed zeroes to source zero
reports=[]
for mode in ('off0','off1'):
 commands=[json.loads(x) for x in (p/f'source-{mode}.jsonl').read_text().splitlines()];angles=[json.loads(x) for x in (p/f'source-{mode}-angles.jsonl').read_text().splitlines()];assert len(commands)==15 and len(angles)==30
 differences=[]
 for i,(n,c) in enumerate(zip(native,commands)):
  before=[word(x) for x in angles[i*2]['physical']];after=[word(x) for x in angles[i*2+1]['physical']]
  assert before==n['before'],(i,'initial physical angle mismatch',before,n['before']);assert c['visual_word']==n['visual'][0]
  if after!=n['after_step']:differences.append(dict(case=i,expected=n['after_step'],actual=after))
 reports.append(dict(run=mode,cases=15,visual_matches=15,physical_differences=differences))
assert reports[0]['physical_differences']==reports[1]['physical_differences'];(p/'comparison-before.json').write_text(json.dumps(dict(reports=reports),indent=2)+'\n');print('Physical differences per repeat:',len(reports[0]['physical_differences']))
