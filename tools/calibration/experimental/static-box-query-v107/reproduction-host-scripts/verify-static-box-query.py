from pathlib import Path
import gzip, hashlib, json, subprocess

root=Path.cwd();work=root/'work/static-box-query-v107'
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d')
fixtures=[];groups={}
for path in sorted((root/'outputs/static-box-query-v107/native-transform').glob('*-private.jsonl.gz')):
    rows=list(map(json.loads,gzip.open(path,'rt')));records=[];current=None
    for r in rows:
        event=r.get('function')
        if event=='wheel-box-ray-call':current={'call':r}
        elif event in ['box-transform-entry','local-box-ray-call','local-box-ray-result']:
            assert current is not None
            current[event]=r
        elif event=='wheel-box-ray-result':
            assert current is not None and r['eax']&255==1
            result=current['local-box-ray-result'];pose=current['box-transform-entry']
            call=current['call'];w=r['box_query_hit_after']['words']
            input_words=pose['box_world_pose']['words']+pose['box_half']['words']+call['box_query_ray']['words']+[call['stack']['words'][1]]
            expected=[1]+result['local_box_start']['words']+result['local_box_direction']['words']+result['local_box_point']['words']+result['local_box_distance']['words']+w[1:4]+w[4:7]
            assert len(input_words)==22 and len(expected)==17
            records.append((input_words,expected));current=None
    assert current is None and len(records)==2
    fixtures.extend(records)
    groups.setdefault(path.name.rsplit('-',2)[0],[]).append(records)
assert len(fixtures)==8 and len(groups)==2
assert all(len(g)==2 and g[0]==g[1]for g in groups.values())
manifest=json.loads((root/'outputs/moving-support-v80/native-transform/manifest.json').read_text())
assert len(manifest['captures'])==4 and all(c['physical_golden_exact']for c in manifest['captures'])
text=str(len(fixtures))+'\n'+'\n'.join(' '.join(map(str,i))for i,e in fixtures)+'\n'
reports=[]
for opt in ['O0','O2']:
    binary=work/('verify-box-query-'+opt)
    subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-I',str(active/'src/Rock3dEngine/header/physics'),str(work/'verify-box-query.cpp'),'-o',str(binary)],check=True)
    out=subprocess.check_output([str(binary)],input=text,text=True)
    actual=[list(map(int,line.split()))for line in out.splitlines()]
    assert len(actual)==len(fixtures)
    dif=[]
    for index,(a,(_,e))in enumerate(zip(actual,fixtures)):
        if a!=e:dif.append({'fixture':index,'fields':[(i,x,y)for i,(x,y)in enumerate(zip(a,e))if x!=y]})
    reports.append({'optimization':opt,'exact':not dif,'differences':dif})
    print(opt,'exact',not dif,'differences',dif,flush=True)
(work/'box-query-fixtures.json').write_text(json.dumps({'fixtures':[{'input_words':i,'expected_words':e}for i,e in fixtures]},indent=2)+'\n')
(root/'outputs/physics-v107-native-static-box-query-proof.json').write_text(json.dumps({'scope':'Identical original static-box inputs; curb13 and bump50, two repeats, original exe/DLL/physical histories unchanged. Prototype not installed in live physics.','fixtures':8,'raw_words_per_optimization':136,'checks':reports,'original_constants':{'candidate_initial_value':-1,'side_boundary_epsilon':1e-5},'live_physics_changed':False,'goal_complete':False},indent=2)+'\n')
assert all(r['exact']for r in reports)
