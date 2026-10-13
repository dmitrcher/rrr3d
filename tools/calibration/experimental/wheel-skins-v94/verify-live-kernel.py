from pathlib import Path
import json, gzip, struct
root=Path(__file__).resolve().parent;out=root/'captures'
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
value=lambda x:struct.unpack('<f',struct.pack('<I',x))[0]
checks=[]
for index,tick in [(0,22),(1,22),(2,23)]:
    for rep in range(2):
        native=list(map(json.loads,gzip.open(out/f'native-stop-prep/{index}-{tick}-{rep}-private.jsonl.gz','rt')))
        applied=[r for r in native if r.get('function')=='spring-solve-applied']
        objects=[r for r in native if r.get('function')=='spring-row-entry'][:4]
        prepared=[r for r in native if r.get('function')=='spring-row-ready'][:4]
        stops=list(map(json.loads,gzip.open(out/f'mac-candidate-stop/{index}-{tick}-{rep}-stop.jsonl.gz','rt')))
        for i,(o,p) in enumerate(zip(objects,prepared)):
            m=stops[3-i];row=p['constraint_row']['words']
            assert bits(m['error'])==o['spring_object']['words'][24]
            assert bits(m['delta'])==o['stack']['words'][2]
            assert bits(m['mass'])==row[11]
            assert bits(m['error'])==o['spring_object']['words'][24]
            assert m['skin_valid']
            assert bits(m['error_rate'])==row[7]
            rounded=bits(value(row[7])*value(row[15]))
            assert bits(m['position_term'])==rounded
        solver=list(map(json.loads,gzip.open(out/f'mac-candidate-stop/{index}-{tick}-{rep}-solver.jsonl.gz','rt')))
        tire=[r for r in map(json.loads,gzip.open(out/f'mac-candidate-stop/{index}-{tick}-{rep}-tire.jsonl.gz','rt')) if r['event'].endswith('result')]
        mapped=[]
        for iteration in range(1,6):
            rows=[r for r in solver if r['iteration']==iteration and r['phase'] in ('ordered-stop-after','spring-after')]
            assert len(rows)==8
            mapped.extend([(r['velocity']+r['angular'],r['lambda']) for r in rows])
            for r in tire[(iteration-1)*8:iteration*8]:
                mapped.append(([r['linear'][0],r['linear'][2],r['linear'][1],-r['angular'][0],-r['angular'][2],-r['angular'][1]],r['lambda']))
        assert len(mapped)==len(applied)==80
        for position,((body,lam),n) in enumerate(zip(mapped,applied)):
            assert list(map(bits,body))==[n['solve_body0']['words'][k]for k in (0,1,2,4,5,6)],(index,rep,position,'body')
            assert lam==value(n['solve_row']['words'][28]) or bits(lam)==n['solve_row']['words'][28],(index,rep,position,'lambda')
        checks.append({'case_index':index,'repeat':rep,'continuous_body_checkpoints_exact':80,'body_float32_words_exact':480,'stop_preparation_input_and_output_exact':4,'numeric_lambdas_exact':80})
report={'scope':'Actual uninterrupted steps22 Marauder drive/brake and23 Manticora lateral. No body reset, fixture dispatch, oracle injection or changed commands. Four stop rows, four spring rows, eight tire rows through five rounds; read-only Native debugger preserves immutable Windows Golden. Private signed-zero lambda bit parity not claimed by decimal telemetry.','checks':checks,'goal_complete':False}

print('Three actual scenarios x2: all80 continuous body checkpoints/480 body words and4 stop preparations exact per capture')
