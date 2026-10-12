"""Re-run actual captured Windows SDK rays; hit/distance/normal and point are separate."""
from pathlib import Path
import argparse,tempfile,subprocess,json,struct,hashlib
p=argparse.ArgumentParser();p.add_argument('--out',type=Path,required=True);args=p.parse_args();here=Path(__file__).resolve().parent;results=[]
word=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
with tempfile.TemporaryDirectory(prefix='motorrock-v59-rays-') as tmp:
 for name,data,count in [('verify-box-rays.cpp','native-box-rays.jsonl',384),('verify-box-tolerance.cpp','native-box-tolerance.jsonl',408)]:
  native=[json.loads(x) for x in (here/data).read_text().splitlines()];assert len(native)==count
  for opt in ['-O0','-O2']:
   exe=Path(tmp)/'verify';subprocess.run(['clang++','-std=c++17',opt,'-ffp-contract=off','-I',str(here),str(here/name),'-o',str(exe)],check=True)
   actual=[json.loads(x) for x in subprocess.check_output([str(exe)],text=True).splitlines()];assert len(actual)==count
   mismatches={k:0 for k in ['hit','distance','normal','point']};hits=0
   for n,m in zip(native,actual):
    assert n['query']==m['query'];mismatches['hit']+=n['hit']!=m['hit']
    if n['hit'] and m['hit']:
     hits+=1;mismatches['distance']+=n['distance']!=word(m['distance'])
     for k in ['normal','point']:mismatches[k]+=sum(x!=word(y) for x,y in zip(n[k],m[k]))
   assert all(mismatches[k]==0 for k in ['hit','distance','normal']),mismatches
   # Eight output-point words in rotated/oblique queries are an open difference.
   assert mismatches['point']==(8 if count==384 else 0),mismatches
   result={'dataset':data,'optimization':opt,'queries':count,'matched_hits':hits,'float32_mismatched_words':mismatches,'reference_sha256':hashlib.sha256((here/data).read_bytes()).hexdigest()};results.append(result);print(json.dumps(result),flush=True)
args.out.parent.mkdir(parents=True,exist_ok=True);args.out.write_text(json.dumps({'results':results,'point_parity_complete':False,'goal_complete':False},indent=2)+'\n')
