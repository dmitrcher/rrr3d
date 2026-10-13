from pathlib import Path
import hashlib,json,zipfile,subprocess
root=Path.cwd();repo=root/'work/sync-rrr3d';source=repo/'tools/calibration/experimental/wheel-motor-v92';head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip();audit=json.loads((root/'outputs/physics-v92-final-broad-audit.json').read_text())
manifest=json.loads((source/'sha256-manifest.json').read_text())
for f in manifest['files']:
 p=source/f['path'];assert p.stat().st_size==f['bytes'];assert hashlib.sha256(p.read_bytes()).hexdigest()==f['sha256']
archive=root/'outputs/MotorRock-v92-Native-Wheel-Motor-Verified.zip';assert not archive.exists();files=[]
for p in sorted(source.rglob('*')):
 if p.is_file():files.append((p,'source/wheel-motor-v92/'+str(p.relative_to(source))))
for p in sorted((root/'work/frozen-v92').iterdir()):
 if p.is_file():files.append((p,'mac-probes/v92/'+p.name))
files.append((root/'outputs/physics-v92-probe-validation.json','evidence/physics-v92-probe-validation.json'));members=[{'path':name,'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p,name in files];m={'source_commit':head,'source_base':'V91 verified overlay required; see source README','research_checkpoint':True,'strict_matches':len(audit['exact']),'open':len(audit['remaining']),'members':members,'goal_complete':False}
with zipfile.ZipFile(archive,'x',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
 for p,name in files:z.write(p,name)
 z.writestr('manifest.json',json.dumps(m,indent=2)+'\n')
with zipfile.ZipFile(archive) as z:
 assert len(z.namelist())==len(members)+1
 for f in members:
  data=z.read(f['path']);assert len(data)==f['bytes'] and hashlib.sha256(data).hexdigest()==f['sha256']
sha=hashlib.sha256(archive.read_bytes()).hexdigest();archive.with_suffix('.zip.sha256').write_text(sha+'  '+archive.name+'\n');print(head,archive.name,archive.stat().st_size,sha,len(members))
