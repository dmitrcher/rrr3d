"""Verify every retained archive member and all 608 complete trajectories."""
from pathlib import Path
import argparse,hashlib,importlib.util,io,itertools,json,sys,zipfile
parser=argparse.ArgumentParser();parser.add_argument('archive',type=Path);parser.add_argument('--out',type=Path,required=True);args=parser.parse_args()
here=Path(__file__).resolve().parent;sys.path.insert(0,str(here))
spec=importlib.util.spec_from_file_location('words',here/'compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
def cases(stream):
 current=None;seen=set()
 for line in stream:
  row=json.loads(line);event=row['event']
  if event=='calibration.begin':
   assert current is None and row['detail'] not in seen;seen.add(row['detail']);current={'name':row['detail'],'meta':row['sample'],'rows':[]}
  elif event=='calibration.sample':
   assert current is not None and row['detail']==current['name'];sample=row['sample'];assert sample['tick']==len(current['rows']);current['rows'].append(sample)
  elif event=='calibration.end':
   assert current is not None and row['detail']==current['name'] and len(current['rows'])==current['meta']['steps'];yield current;current=None
 assert current is None and len(seen)==608
with zipfile.ZipFile(args.archive) as archive:
 manifest=json.loads(archive.read('manifest.json'));assert set(archive.namelist())==set(manifest['files'])|{'manifest.json'}
 for name,record in manifest['files'].items():
  h=hashlib.sha256();size=0
  with archive.open(name) as stream:
   for block in iter(lambda:stream.read(1024*1024),b''):h.update(block);size+=len(block)
  assert h.hexdigest()==record['sha256'] and size==record['bytes'],name
 assert manifest['files']['measurements/references/original-windows.jsonl']['sha256']=='4bcc66c8f4f28ccb77a07759175573cc56d72035636642617fae4e75addecc1f'
 streams=[io.TextIOWrapper(archive.open(name)) for name in ['measurements/references/original-windows.jsonl','measurements/control/v57.jsonl','measurements/broad/candidate-run0.jsonl','measurements/broad/candidate-run1.jsonl']]
 exact=[];before_exact=[];samples=0
 for a,b,c,d in itertools.zip_longest(*(cases(stream) for stream in streams)):
  assert a and b and c and d and a['name']==b['name']==c['name']==d['name'];assert c==d,('repeat',c['name']);assert len(a['rows'])==len(b['rows'])==len(c['rows'])
  native=[words.physical(row) for row in a['rows']];old=[words.physical(row) for row in b['rows']];now=[words.physical(row) for row in c['rows']]
  if native==now:exact.append(a['name'])
  if native==old:before_exact.append(a['name'])
  samples+=len(native)
 for stream in streams:stream.close()
 assert len(exact)==65 and samples==255864 and set(before_exact)<=set(exact)
 result={'all_member_hashes_verified':len(manifest['files']),'cases':608,'samples':samples,'strict_exact':len(exact),'remaining':608-len(exact),'two_actual_repeats_equal':True,'all_v57_exact_cases_preserved':True,'goal_complete':False}
 args.out.parent.mkdir(parents=True,exist_ok=True);args.out.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
