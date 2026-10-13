"""One-shot lossless compaction of verified, closed V72 captures. Never rerun."""
from pathlib import Path
import gzip,json,hashlib,os
root=Path.cwd();journal=root/'outputs/storage-cleanup-v72-journal.json';assert not journal.exists(),'One-shot journal exists; do not rerun'
base=root/'outputs/pair-contact-v72'
assert len(json.load(open(base/'broad/capture-manifest.json'))['captures'])==2
assert json.load(open(root/'outputs/physics-v72-final-broad-audit.json'))['two_repeats_exact']
def sha_stream(stream):
 h=hashlib.sha256()
 for b in iter(lambda:stream.read(1024*1024),b''):h.update(b)
 return h.hexdigest()
paths=[p for p in base.rglob('*.jsonl') if p.stat().st_size>=1024*1024 and p!=base/'broad/candidate-run0.jsonl'];entries=[]
for p in paths:
 before=p.stat().st_size
 with p.open('rb') as s:original=sha_stream(s)
 gz=Path(str(p)+'.gz');assert not gz.exists();tmp=Path(str(gz)+'.partial')
 with p.open('rb') as src,gzip.open(tmp,'wb',compresslevel=3) as dst:
  for b in iter(lambda:src.read(1024*1024),b''):dst.write(b)
 with gzip.open(tmp,'rb') as s:assert sha_stream(s)==original
 os.replace(tmp,gz)
 entries.append({'source':str(p.relative_to(root)),'gzip':str(gz.relative_to(root)),'logical_sha256':original,'before_bytes':before,'after_bytes':gz.stat().st_size,'roundtrip_verified':True})
 journal.write_text(json.dumps({'scope':__doc__,'entries':entries,'goal_complete':False},indent=2)+'\n')
 p.unlink()
summary={'files':len(entries),'bytes_saved':sum(x['before_bytes']-x['after_bytes'] for x in entries),'all_lossless_roundtrips_verified':True,'latest_broad_run0_kept_raw':True,'goal_complete':False}
(root/'outputs/storage-cleanup-v72-result.json').write_text(json.dumps(summary,indent=2)+'\n');print(summary)
