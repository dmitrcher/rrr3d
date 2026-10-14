from pathlib import Path
import json,gzip,struct
root=Path.cwd();p=root/'work/point-material-v150'
word=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
source=list(map(json.loads,open(root/'outputs/point-material-v150/source-rows358/track0-physical.jsonl')))
native=list(map(json.loads,gzip.open(root/'outputs/point-material-v150/native-step358/track0-run0-private.jsonl.gz','rt')))
cache=next(x['sample'] for x in source if x['event']=='physics.native-cache-input')
material=next(x['sample'] for x in source if x['event']=='physics.native-material-stages')
transport=next(x for x in native if x.get('function')=='point-transport-entry')
raw=next(x for x in native if x.get('function')=='converted-error-before-copy')['converter_current_output']['words']
geometry=[]
for i in range(4):
 expected=transport[f'transport_input_point_{i}']['words'];actual=list(map(word,cache['current'][i]))+[word(cache['separations'][i])];reference=expected[:3]+expected[6:7]
 geometry.append(dict(point=i,source=actual,native=reference,differing_components=[j for j,(a,b) in enumerate(zip(actual,reference)) if a!=b]))
assert [x['differing_components'] for x in geometry]==[[],[],[0],[]]
assert abs(geometry[2]['source'][0]-geometry[2]['native'][0])==1
source_local=list(map(word,material['raw_local']));native_local=raw[3:7]
assert source_local!=native_local
samples=[x['sample'] for x in source if x['event']=='solo.sample']
plain=[x['sample'] for x in map(json.loads,open(root/'outputs/point-material-v150/moving-maps/track0-on0.jsonl')) if x['event']=='solo.sample']
assert samples==plain and len(samples)==900
manifest=json.load(open(root/'outputs/point-material-v150/native-step358/manifest.json'))
assert manifest['original_public_states_preserved']==900 and manifest['repeats']==2
report=dict(scope='Next first divergence physical358, after V150; Source diagnostic trace preserves all900 states; original read-only observer preserves two original900-state histories.',geometry=geometry,source_raw_local=source_local,native_raw_local=native_local,next_cause='Converter Core743c7..744fa forms local quaternion with cross products before scalar products, and scalar dot Z/Y/X. Current framev36::multiply uses another association. Offline converter-proof.json reproduces original local quaternion from Source-derived actor and raw frame at342/358 O0/O2. Runtime fix and other material stages are not yet implemented.',remaining_geometry='Third point world X differs by one float32 ULP; local clipping/transport operation still needs isolation.',goal_complete=False)
(p/'next358-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Next358 evidence saved; two distinct causes remain open')
