from pathlib import Path
import subprocess,json,gzip
root=Path.cwd();out=root/'outputs/cooking-order-v159/metadata';out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\cooking-order-v159';original=r'C:\MotorRock\calibration-probes\control-wake-v133';exe=folder+r'\WorldProbeMetaV159.exe'
def guest(cmd):
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=180);return r.stdout+r.stderr
files=[exe,folder+r'\PhysXCore.dll',folder+r'\PhysXCooking.dll',original+r'\WorldProbeV133.exe'];hashes={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files};captures=[]
for track,mesh in [(0,25),(64,37)]:
 ref=[r for r in map(json.loads,gzip.open(root/f'outputs/game-control-wake-v133/native/track{track}-rep0.jsonl.gz','rt')) if r.get('event')=='world.sample'];assert len(ref)==900
 for rep in [0,1]:
  prefix=folder+f'\\track{track}-mesh{mesh}-run{rep}';physical=prefix+'-physical.jsonl';meta=prefix+'-metadata.jsonl';print('Metadata helper',track,mesh,rep,'start',flush=True)
  cmd='cd /d '+folder+'&& set MOTORROCK_COOKED_MESH_V159='+str(mesh)+'&& set MOTORROCK_COOKED_CONTACT_METADATA_V159='+meta+'&& '+exe+' '+original+f'\\track{track}.words '+physical
  (out/f'track{track}-run{rep}.log').write_bytes(guest(cmd));data=guest('type '+physical);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));actual=[r for r in rows if r.get('event')=='world.sample'];assert actual==ref,('Unvalidated metadata helper changed public physics',track,rep)
  with gzip.open(out/f'track{track}-run{rep}-physical.jsonl.gz','wb') as z:z.write(data)
  data=guest('type '+meta);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));assert len(rows)==(32 if track==0 else 52)
  if track==0:assert rows==list(map(json.loads,Path('outputs/cooked-contact-v145-initialized/metadata0.jsonl').read_text(encoding='utf-8-sig').splitlines()))
  with gzip.open(out/f'track{track}-run{rep}-metadata.jsonl.gz','wb') as z:z.write(data)
  captures.append(dict(track=track,mesh=mesh,repeat=rep,original_physics_states_exact=900,cooked_triangles=len(rows),baseline32_metadata_exact=track==0));print('Metadata verified original900states',track,rep,flush=True)
for f,h in hashes.items():assert guest('certutil -hashfile '+f+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(scope='Separate instrumented helper; immutable original133EXE never modified. Two900state histories per mesh match original reference; baseline32 metadata exactly matches previous initialized reader.',hashes=hashes,captures=captures,goal_complete=False),indent=2)+'\n')
