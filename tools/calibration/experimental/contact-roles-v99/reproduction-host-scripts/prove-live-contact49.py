from pathlib import Path
import json,gzip,struct

root=Path.cwd(); bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def load(p):
    if not p.exists():p=Path(str(p)+'.gz')
    with (gzip.open(p,'rt') if str(p).endswith('.gz') else p.open()) as f:return list(map(json.loads,f))
reports=[]
for rep in range(2):
    native=load(root/f'outputs/solver-compose-v98/native-next-border49/v98-next-border49-border-front-grounded-{rep}-private.jsonl')
    nativepub=load(root/f'outputs/creation-order-v99-verified/control-inventory-{rep}.jsonl')
    mac=load(root/f'outputs/creation-order-v99-mac-contact49/border-front-grounded-49-{rep}-physical.jsonl')
    prepared=[x for x in native if x.get('function')=='solver-initialize-exit']
    rows=[x['sample'] for x in mac if x.get('event')=='physics.native-contact-row']
    cache=next(x['sample'] for x in mac if x.get('event')=='physics.native-cache-input')
    raw=next(x for x in native if x.get('function')=='converted-create-entry')
    material=next(x['sample'] for x in mac if x.get('event')=='physics.native-material-stages')
    callback=next(x['sample'] for x in nativepub if x.get('event')=='calibration.contact-quaternion' and x['detail']=='border-front-grounded' and x['sample']['tick']==49)
    assert len(prepared)==len(rows)==20
    checks={'incoming_body_words':0,'tensor_words':0,'lever_words':0,'mass_words':0,'scaled_bias_words':0,'quaternion_words':0}; qdif=[];bdif=[]
    for i,(n,m) in enumerate(zip(prepared,rows)):
        body=n['native_body1']['words']; row=n['native_solver_row']['words']
        assert body[:3]+body[4:7]==list(map(bits,m['velocity']+m['angular'])),('incoming',rep,i)
        checks['incoming_body_words']+=6
        assert body[15:24]==list(map(bits,m['inverse_inertia'])),('tensor',rep,i);checks['tensor_words']+=9
        assert row[8:11]==list(map(bits,m['lever_arm'])),('lever',rep,i,row[8:11],list(map(bits,m['lever_arm'])));checks['lever_words']+=3
        assert row[20]==bits(m['effective_mass']),('mass',rep,i);checks['mass_words']+=1
        if row[24]!=bits(m['scaled_bias']):bdif.append({'row':i,'native':row[24],'mac':bits(m['scaled_bias'])})
        else:checks['scaled_bias_words']+=1
        for j,(a,b) in enumerate(zip(row[16:20],map(bits,material['solver_quaternion']))):
            if a!=b:qdif.append({'row':i,'component':j,'native':a,'mac':b})
            else:checks['quaternion_words']+=1
    assert cache['canonical_dynamic_slot']==1 and cache['surface_body_index']>cache['car_body_index']
    for i in range(4):
        word=raw[f'converter_raw_{i}']['words']
        assert list(map(bits,cache['after'][i][0]))==word[:3]
        assert list(map(bits,cache['after'][i][1]))==word[6:9]
    assert list(map(bits,material['callback_local0']))==list(map(bits,callback['local_quaternion0']))
    assert list(map(bits,material['callback_local1']))==list(map(bits,callback['local_quaternion1']))
    assert not bdif,bdif
    assert all({x['native'],x['mac']}=={0,2147483648} for x in qdif),qdif
    reports.append({'repeat':rep,'exact_words':checks,'raw_cache_point_words_exact':24,'actual_callback_local_quaternion_words_exact':8,'solver_quaternion_signed_zero_differences':qdif,'scaled_bias_differences':bdif,'uninterrupted_solver_checkpoints':20})
report={'scope':'Fresh V99 scene-order and canonical-role candidate; all 20 uninterrupted original Windows border49 solver body inputs, tensors, dynamic body1 levers, masses and scaled biases. No runtime oracle state injection.','repeats':reports,'native_dynamic_slot':1,'mac_cache_dynamic_slot':1,'mac_Jolt_dynamic_first':True,'solver_quaternion_signed_zero_only_difference':True,'goal_complete':False}
(root/'outputs/creation-order-v99-live-contact49-proof.json').write_text(json.dumps(report,indent=2)+'\n')
print('Actual Native49 live checkpoints bound in both repeats',json.dumps([{k:v for k,v in r.items() if k not in ['solver_quaternion_signed_zero_differences']} for r in reports]),flush=True)
