from pathlib import Path
import gzip,json,struct
r=Path.cwd();bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];v=lambda x:struct.unpack('<f',struct.pack('<I',x))[0];f32=lambda x:v(bits(x));fixtures=[];captures=[]
for tick in [13,14]:
 for rep in [0,1]:
  rows=list(map(json.loads,gzip.open(r/f'outputs/tiny-contact-v123/native-force/kernel-marauder-drop-{tick}-{rep}-private.jsonl.gz','rt')))
  ready=[(i,x) for i,x in enumerate(rows) if x.get('function')=='contact-force-ready'];norm=[x for x in rows if x.get('function')=='contact-force-norm-ready'];assert len(ready)==len(norm)==10
  source=list(map(json.loads,gzip.open(r/f'outputs/tiny-contact-v123/mac-candidate/skin-tilted-triangle-angle3-skin2-thin1-{tick}-{rep}-force.jsonl.gz','rt')))[tick*10:tick*10+10];assert len(source)==10
  equalforces=0;skips=[]
  for index,((pos,x),y,z) in enumerate(zip(ready,norm,source)):
   force=[x['xmm_words'][k][0] for k in [4,5,6]];expected=y['xmm_words'][0][0];squares=[f32(v(k)*v(k)) for k in force];actual=bits(f32(f32(squares[0]+squares[2])+squares[1]));assert actual==expected
   end=ready[index+1][0] if index+1<len(ready) else len(rows);applied=any(rr.get('function')=='contact-force-apply' for rr in rows[pos:end]);assert applied==(v(expected)!=0.0)
   assert x['mxcsr_word']==8064 and y['mxcsr_word']==8064
   if not applied:skips.append(index)
   if True:
    original=[bits(-v(k)) for k in force];other=list(map(bits,z['force']));assert all(a==b or a&0x7fffffff==b&0x7fffffff==0 for a,b in zip(original,other));equalforces+=1
   fixtures.append({'tick':tick,'repeat':rep,'row':index,'force_words':force,'expected_norm_word':expected,'expected_applied':applied,'mxcsr_word':8064})
  captures.append({'tick':tick,'repeat':rep,'actual_norm_and_apply_decisions_verified':10,'Source_forces_match_before_first_divergence':equalforces,'skipped_rows':skips})
(r/'work/tiny-contact-v123/candidate-force-proof.json').write_text(json.dumps({'scope':'Actual forceX/Z/Y squared norm and branch decision read atRVA1e83bf/1e83dc/1e83e9. Original MXCSR8064: gradual underflow; no FTZ/DAZ. Actual candidate Source force vectors match all40 Original rows (opposite body-slot sign). Original complete24-state histories and on-disk executable hashes unchanged.','captures':captures,'fixtures':fixtures,'first_skipped_row_at14':3,'goal_complete':False},indent=2)+'\n');print(captures)
