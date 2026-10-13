from pathlib import Path
import json,hashlib,subprocess
root=Path.cwd();w=root/'work/creation-order-v99';s=(w/'before-car.cpp').read_text();assert s.count('\n if(c.obstacle==5)')==1;s=s.replace('\n if(c.obstacle==5)','\n if(c.obstacle==7||c.obstacle==8||c.obstacle==9){ /* Mesh already created before car; do not create fallback box. */ }\n else if(c.obstacle==5)')
trace=''' std::ostringstream inventory;inventory<<"{\\"actors\\":"<<scene->getNbActors()<<",\\"items\\":[";
 for(NxU32 i=0;i<scene->getNbActors();++i){NxActor* a=scene->getActors()[i];if(i)inventory<<',';inventory<<"{\\"dynamic\\":"<<(a->isDynamic()?"true":"false")<<",\\"shapes\\":"<<a->getNbShapes()<<",\\"id\\":"<<reinterpret_cast<size_t>(a->userData)<<'}';}inventory<<"]}";motorrock::parity::event("calibration.scene-inventory",c.name,inventory.str());
'''
anchor=' for(int tick=0;tick<c.steps;++tick)'
for name,data in [('control-inventory.cpp',(w/'control.cpp').read_text()),('before-car-only.cpp',s)]:
 assert data.count(anchor)==1;data=data.replace(anchor,trace+anchor);(w/name).write_text(data)
(w/'build-verified.cmd').write_text((w/'build.cmd').read_text().replace('(control before-car)','(control-inventory before-car-only)'))
folder=r'C:\MotorRock\calibration-probes\creation-order-v99';shared=r'\\Mac\Home\Documents\Codex\2026-10-10\motorock-mac-github-windows-mac-mac\work\creation-order-v99'
for name in ['control-inventory.cpp','before-car-only.cpp','build-verified.cmd']:
 cmd="powershell.exe -NoProfile -Command \"Copy-Item -LiteralPath '"+shared+'\\'+name+"' -Destination '"+folder+'\\'+name+"'\"";subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True)
r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',folder+r'\build-verified.cmd'],capture_output=True,timeout=180);(w/'build-verified.log').write_bytes(r.stdout+r.stderr);assert b'fatal error' not in r.stdout.lower() and b'error c' not in r.stdout.lower() and b'error lnk' not in r.stdout.lower(),r.stdout[-3000:]
hashes={}
for name in ['control-inventory.exe','before-car-only.exe']:
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c','certutil -hashfile '+folder+'\\'+name+' SHA256'],capture_output=True,check=True);assert len(r.stdout)>70 and b'ERROR' not in r.stdout;hashes[name]=r.stdout.decode(errors='replace')
(w/'build-verified-manifest.json').write_text(json.dumps({'scope':'Second creation-order experiment: same actor/shape population, separate control and only-mesh-before-car variant, inventory trace added.','executable_hashes':hashes,'source_files':[{'path':str(p.relative_to(w)),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in sorted(w.rglob('*')) if p.is_file() and p.suffix in ['.h','.cpp','.cmd']],'goal_complete':False},indent=2)+'\n')
(root/'outputs/creation-order-v99/REJECTED-EXPERIMENT.json').write_text(json.dumps({'rejected':'before-car variant and any conclusions based on it','reason':'Moved mesh creation but removed its old if guard; fallback else-if created an additional box for obstacle7/8/9. Actor population differs. Control still exactly reproduces1980Golden states per repeat.','Original_reference_unchanged':True,'Mac_V98_source_unchanged':True,'replacement':'Separate control-inventory/before-car-only with runtime inventory equality guard','goal_complete':False},indent=2)+'\n')
p=w/'run-creation-experiment.py';t=p.read_text().replace("outputs/creation-order-v99'","outputs/creation-order-v99-verified'").replace("['control','before-car']","['control-inventory','before-car-only']").replace("mode=='control'","mode=='control-inventory'").replace("folder+r'\\control.exe'","folder+r'\\control-inventory.exe'").replace("folder+r'\\before-car.exe'","folder+r'\\before-car-only.exe'")
t=t.replace("reports={};allruns={}","reports={};allruns={};inventories={}")
t=t.replace("rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));states={n:[] for n in names}","rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));states={n:[] for n in names};inv={x['detail']:x['sample'] for x in rows if x.get('event')=='calibration.scene-inventory'};assert len(inv)==4;inventories[mode]=inv")
t=t.replace("for p,h in hashes.items():", "for n in names:\n assert inventories['control-inventory'][n]['actors']==inventories['before-car-only'][n]['actors'];assert sorted((x['dynamic'],x['shapes'],x['id']) for x in inventories['control-inventory'][n]['items'])==sorted((x['dynamic'],x['shapes'],x['id']) for x in inventories['before-car-only'][n]['items'])\nfor p,h in hashes.items():")
t=t.replace("'control_all_original_Golden_states_exact':True","'scene_actor_shape_population_exact':True,'scene_inventories':inventories,'control_all_original_Golden_states_exact':True")
(w/'run-verified-creation-experiment.py').write_text(t);print('new corrected variant built and actual executable hashes verified',flush=True)
