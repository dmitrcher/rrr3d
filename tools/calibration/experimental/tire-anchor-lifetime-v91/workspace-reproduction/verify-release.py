from pathlib import Path
import subprocess,json,hashlib,tempfile,zipfile
root=Path.cwd();tag='physics-tire-anchor-lifetime-v91-investigation-2026-10-13';local=root/'outputs/MotorRock-v91-Tire-Anchor-Lifetime-Verified.zip';expected=hashlib.sha256(local.read_bytes()).hexdigest();release=json.loads(subprocess.check_output(['gh','api','repos/dmitrcher/rrr3d/releases/tags/'+tag],text=True));asset=next(a for a in release['assets'] if a['name']==local.name);assert asset['size']==local.stat().st_size;assert asset.get('digest') in (None,'sha256:'+expected)
with tempfile.TemporaryDirectory(prefix='motorrock-v91-release-') as t:
 tmp=Path(t);subprocess.run(['gh','release','download',tag,'--repo','dmitrcher/rrr3d','--pattern',local.name,'--dir',str(tmp)],check=True);download=tmp/local.name;assert hashlib.sha256(download.read_bytes()).hexdigest()==expected
 with zipfile.ZipFile(download) as z:
  m=json.loads(z.read('manifest.json'));assert len(z.namelist())==len(m['members'])+1
  for item in m['members']:
   data=z.read(item['path']);assert len(data)==item['bytes'] and hashlib.sha256(data).hexdigest()==item['sha256']
  z.extractall(tmp/'unpacked')
 base=tmp/'unpacked';probe=base/'mac-probes/v91/rrr3d_physics_calibration';probe.chmod(0o755)
 for args in [ ['python3',str(base/'source/tire-anchor-lifetime-v91/verify-native-anchor.py')], ['python3',str(base/'source/tire-anchor-lifetime-v91/verify-checkpoint.py'),'--probe',str(probe)] ]:
  subprocess.run(args,cwd=base,check=True)
report={'url':release['html_url'],'tag':tag,'asset':asset['name'],'asset_id':asset['id'],'bytes':asset['size'],'sha256':expected,'actual_download_sha256_exact':True,'all_members_sha256_exact':True,'native_helper_o0_o2_from_download_pass':True,'fresh1800_states_from_downloaded_probe_pass':True,'goal_complete':False};(root/'outputs/physics-v91-github-release-verification.json').write_text(json.dumps(report,indent=2)+'\n');print('V91 release actual download/memberSHA/NativeO0O2/fresh1800states all verified')
