"""One-shot compact the verified V71 broad baseline superseded by V72; never rerun."""
from pathlib import Path
import gzip,json,hashlib,os
root=Path.cwd();report=root/'outputs/storage-cleanup-v72-superseded-v71.json';assert not report.exists(),'One-shot journal exists; do not rerun'
p=root/'outputs/pair-contact-v71/broad/candidate-run0.jsonl';m=json.load(open(root/'outputs/pair-contact-v71/broad/capture-manifest.json'));expected=next(r for r in m['captures'] if r['file']==p.name)['sha256']
assert json.load(open(root/'outputs/physics-v72-final-broad-audit.json'))['two_repeats_exact']
def sha(stream):
 h=hashlib.sha256()
 for block in iter(lambda:stream.read(1024*1024),b''):h.update(block)
 return h.hexdigest()
with p.open('rb') as stream:assert sha(stream)==expected
before=p.stat().st_size;gz=Path(str(p)+'.gz');assert not gz.exists();temp=Path(str(gz)+'.partial')
with p.open('rb') as src,gzip.open(temp,'wb',compresslevel=3) as dst:
 for block in iter(lambda:src.read(1024*1024),b''):dst.write(block)
with gzip.open(temp,'rb') as stream:assert sha(stream)==expected
os.replace(temp,gz)
report.write_text(json.dumps({'scope':__doc__,'source':str(p.relative_to(root)),'gzip':str(gz.relative_to(root)),'logical_sha256':expected,'before_bytes':before,'after_bytes':gz.stat().st_size,'bytes_saved':before-gz.stat().st_size,'lossless_roundtrip_verified':True,'v72_current_raw_preserved':True,'goal_complete':False},indent=2)+'\n');p.unlink();print(report.read_text())
