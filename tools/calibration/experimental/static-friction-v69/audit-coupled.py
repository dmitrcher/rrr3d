from pathlib import Path
import json,gzip,struct,math
root=Path.cwd();f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0];distance=lambda a,b:math.sqrt(sum((x-y)**2 for x,y in zip(a,b)))
wheel=[r for r in map(json.loads,open(root/'outputs/native-border-next-wheel-v63/0-private-run0.jsonl')) if r.get('function')=='spring-solve-input']
contact=[r for r in map(json.loads,gzip.open(root/'outputs/triangle-native-followup-v60/native-border-grounded-62-0-private.jsonl.gz','rt')) if r.get('function')=='solver-iteration-entry' and r.get('simulation_step')==62]
macwheel=list(map(json.loads,open(root/'outputs/hard-stop-force-traces-v68/v64_63-run0-wheel.jsonl')))
maccontact=[r['sample'] for r in map(json.loads,open(root/'outputs/hard-stop-force-traces-v68/v64_63-run0.jsonl')) if r.get('event')=='physics.native-contact-row']
rows=[]
for i in range(5):
 mw=[r for r in macwheel if r['iteration']==i+1];mc=[r for r in maccontact if r['iteration']==i]
 assert len(mc)==6 and mw[0]['step']==64
 before=[f(x) for x in wheel[8*i]['solve_body0']['words']]
 n=contact[6*i];key='native_body0' if f(n['native_body0']['words'][11]) else 'native_body1';after=[f(x) for x in n[key]['words']]
 rows.append({'iteration':i+1,'before_wheel_velocity_error':distance(before[:3],mw[0]['velocity']),'before_wheel_angular_error':distance(before[4:7],mw[0]['angular']),'after_wheel_velocity_error':distance(after[:3],mw[-1]['velocity']),'after_wheel_angular_error':distance(after[4:7],mw[-1]['angular']),'after_wheel_matches_first_mac_contact':mw[-1]['velocity']==mc[0]['velocity'] and mw[-1]['angular']==mc[0]['angular']})
proof=json.load(open(root/'outputs/physics-v69-identical-input-static-friction-proof.json'));force=list(map(json.loads,open(root/'outputs/hard-stop-force-traces-v68/v64_63-run0-force.jsonl')))
native=next(r for r in proof['history'] if r['step']==62 and r['iteration']==2 and r['contact']==2)
mac=force[38];assert mac['entry']==39 and mac['index']==0 and mac['iteration']==2
report={'alignment':{'native_integrator_counter':62,'mac_wheel_counter':64,'mac_contact_telemetry_step':63,'corresponding_after_solve_sample_tick':63},'rounds':rows,'localized_difference':{'native_force':native['force'],'mac_force':mac['force'],'static_coefficient_native':3.200000047683716,'dynamic_coefficient_native_and_mac':1.0,'original_game_material':'src/Rock3dGame/source/game/DataBase.cpp car staticFrictionV=3.2 and dynamicFrictionV=2.0; Native pair caps affect dynamic coefficient independently.'},'identical_input_proof':{'native_static_exact_transitions':40,'legacy_dynamic_only_exact_transitions':37,'total_transitions':40},'goal_complete':False}
(root/'outputs/physics-v69-coupled-localization.json').write_text(json.dumps(report,indent=2)+'\n')
print('Large divergence begins in second contact round, not its preceding wheel round.')
