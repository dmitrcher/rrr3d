from pathlib import Path
import subprocess,json,gzip,hashlib,base64,zipfile
p=Path('work/startup-import-v130');out=Path('outputs/startup-import-v130/native-final');out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\startup-import-v130'
def guest(cmd,timeout=120):
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=timeout);return r.stdout+r.stderr
# Fixtures already uploaded by first capture; each SHA is verified before use.
files=[r'C:\MotorRock\game-debug\Rock3dGame.dll',r'C:\MotorRock\game-debug\Rock3dGame.pdb',r'C:\MotorRock\calibration-probes\contact-pose-v45\SpringProbe.exe',r'C:\MotorRock\calibration-probes\contact-pose-v45\PhysXCore.dll']+[folder+'\\'+n for n in ['WorldProbeV130Cleanup.exe','PhysXCore.dll','PhysXCooking.dll','PhysXLoader.dll','cudart32_30_9.dll']]
hashes={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files};captures=[]
for track in [0,16,48,64]:
 runs=[];fixture=folder+f'\\track{track}.words';sha=hashlib.sha256(Path(f'outputs/real-map-v124/repeats/track{track}-run0.words').read_bytes()).hexdigest();assert sha in guest('certutil -hashfile '+fixture+' SHA256').decode().lower().replace(' ','')
 for rep in [0,1]:
  remote=folder+f'\\track{track}-rep{rep}.jsonl';print('original startup Native',track,rep,'start',flush=True)
  log=guest('cd /d '+folder+'&& WorldProbeV130Cleanup.exe '+fixture+' '+remote);(out/f'track{track}-rep{rep}.log').write_bytes(log);data=guest('type '+remote);rows=[json.loads(l) for l in data.decode('utf-8-sig').splitlines()];assert len([x for x in rows if x['event']=='world.sample'])==900;assert len([x for x in rows if x['event']=='world.original-startup'])==1;runs.append(rows)
  with gzip.open(out/f'track{track}-rep{rep}.jsonl.gz','wb') as z:z.write(data)
  captures.append(dict(track=track,repeat=rep,states=900,fixture_sha256=sha));print('completed',track,rep,flush=True)
 assert runs[0]==runs[1]
for f,h in hashes.items():assert guest('certutil -hashfile '+f+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(scope='Original BaseSceneNode SetDir/SetUp/GetRot plus original px::Actor SetPos/SetRot on real SDK actor. Parent-free, source fixture geometry/spawn; rendered original Race spawn placement not certified. Raw SDK no GameCar controls or AI.',captures=captures,hashes=hashes,repeatable=True,goal_complete=False),indent=2)+'\n')
