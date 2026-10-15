from pathlib import Path
import json,gzip,struct,subprocess
p=Path('work/wheel-bounds-v161');b=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];v=lambda w:struct.unpack('<f',struct.pack('<I',w))[0];inc='/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics';checks=[]
for opt in ['O2']:
 exe=p/('shape-bounds-prototype-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+inc,str(p/'shape-bounds-prototype.cpp'),'-o',str(exe)],check=True)
 for track,last in [(16,420),(64,720)]:
  base=Path(f'outputs/wheel-bounds-v161/source-track16-step420' if track==16 else 'outputs/sleep-inputs-v160/source-track64-step720');poses=[x for x in map(json.loads,open(base/f'track{track}-pose.jsonl')) if x['phase']=='before-update'];before={x['step']:x for x in poses};fixture=Path(f'outputs/real-map-v124/repeats/track{track}-run0.words');ws=list(map(int,fixture.read_text().split()[1:]));assert ws[47]==4;input=[];labels=[]
  for t in range(last+1):
   for wheel in range(4):
    z=48+23*wheel;input.append(list(map(b,before[t]['rotation']+before[t]['center']))+ws[14:17]+ws[z:z+4]+[ws[z+5]]);labels.append((t,wheel))
  actual=[list(map(int,l.split())) for l in subprocess.check_output([str(exe),'3'],input='\n'.join(' '.join(map(str,w)) for w in input)+'\n',text=True).splitlines()]
  for rep in [0,1]:
   native={(x['tick'],x['shape']-1):x['words'] for x in map(json.loads,gzip.open(f'outputs/wheel-bounds-v161/native/track{track}-run{rep}-bounds.jsonl.gz','rt')) if x['shape']};diff=[]
   for label,a in zip(labels,actual):
    e=native[label];fields=[i for i,(x,y) in enumerate(zip(a,e)) if x!=y and (x&0x7fffffff or y&0x7fffffff)]
    if fields:diff.append(dict(tick=label[0],wheel=label[1],fields=fields,source_words=a,native_words=e,source=list(map(v,a)),native=list(map(v,e))))
   r=dict(track=track,optimization=opt,repeat=rep,frames=len(actual),different_bounds=len(diff),first_difference=diff[0] if diff else None,last_difference=diff[-1] if diff else None);checks.append(r);print(track,opt,rep,'different',len(diff),'first',None if not diff else (diff[0]['tick'],diff[0]['wheel'],diff[0]['fields']))
(p/'shape-bounds-prototype-proof.json').write_text(json.dumps(dict(scope='Actual Source prepareNativeContactOrderV79 ray-line bounds formula driven by Source private before-step poses and same Source fixture; compared to original public SDK bounds. Source16same physical through419,64through719; these before-step bounds include420/720.',checks=checks,goal_complete=False),indent=2)+'\n')
