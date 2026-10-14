from pathlib import Path
import json,subprocess,re,gzip
out=Path('outputs/relative-contact-v147/native');original=json.load(open('outputs/game-control-wake-v133/native/manifest.json'))
folder=r'C:\MotorRock\calibration-probes\control-wake-v133'
fixtures={}
for track in [0,16,48]:
 file=folder+f'\\track{track}.words'
 result=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c','certutil -hashfile '+file+' SHA256'],capture_output=True,text=True,check=True,timeout=30)
 sha=re.search(r'(?m)^[0-9a-f]{64}\s*$',result.stdout).group(0).strip();expected=next(x['fixture_sha256'] for x in original['captures'] if x['track']==track);assert sha==expected
 fixtures[str(track)]=dict(sha256=sha,checked_after_capture=True,matches_original_v133_fixture=True,immediate_pre_capture_hash_recorded=track==0)
 for repeat in range(2):
  current=list(map(json.loads,gzip.open(out/f'track{track}-run{repeat}-physical.jsonl.gz','rt')))
  reference=list(map(json.loads,gzip.open(f'outputs/game-control-wake-v133/native/track{track}-rep0.jsonl.gz','rt')))
  assert current==reference
manifest=json.load(open(out/'manifest.json'));manifest['tracks']=[0,16,48];manifest['fixtures']=fixtures;manifest['note']='Original inherited manifest listed only track0. Finalized from actual six captures; tracks16/48 hashes were checked after capture against original V133 fixture hashes, rather than immediately before this capture.'
(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print('Verified six complete unchanged histories and all fixture hashes against V133')
