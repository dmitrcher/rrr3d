from pathlib import Path
import json,struct,hashlib,subprocess,shutil
w=Path(__file__).resolve().parent;root=w.parent.parent.parent
vc=Path('work/crossover-reference-v188/installer-crt/vs_setup.cab').resolve()
def plan(kind):
 m=json.loads((w/f'{kind}-msi-tables.json').read_text());dirs={r[0]:r[1:] for r in m['Directory']};cs={r[0]:r[2] for r in m['Component']}
 def path(d):
  if not d:return ''
  par,n=dirs[d];return path(par)+'/'+n.split(':')[0].split('|')[-1]
 out={}
 for r in m['File']:
  d=path(cs[r[1]]);n=r[2].split('|')[-1]
  if kind=='vc':
   if '/VC/include' in d or d.endswith('/VC/lib') or '/VC/bin' in d:dest='VC/'+d.split('/VC/',1)[1]+'/'+n
   elif d.endswith('/Common7/IDE') and (n.startswith('mspdb') or n=='msobj80.dll'):dest='Common7/IDE/'+n
   else:continue
  else:
   if '/v6.0A/Include' in d:dest='WindowsSDK/'+d.split('/v6.0A/',1)[1]+'/'+n
   elif d.endswith('/v6.0A/Lib'):dest='WindowsSDK/Lib/'+n
   else:continue
  out[r[0]]={'destination':dest,'msi_directory':d,'msi_filename':n,'expected_bytes':r[3]}
 return out
plans={'vc':plan('vc'),'sdk':plan('sdk')}
# Preserve original compressed bytes; restrict only the derived CAB file table.
def subset(src,selected):
 b=bytearray(src.read_bytes());off=struct.unpack_from('<I',b,16)[0];start=off;count=struct.unpack_from('<H',b,28)[0];entries=[]
 for _ in range(count):
  end=b.index(0,off+16)+1;name=bytes(b[off+16:end-1]).decode()
  if name in selected:entries.append(bytes(b[off:end]))
  off=end
 assert len(entries)==len(selected),(len(entries),len(selected));records=b''.join(entries);b[start:start+len(records)]=records;struct.pack_into('<H',b,28,len(entries));p=w/'vc-selected.cab';p.write_bytes(b);return p
selected=subset(vc,plans['vc']);raw=w/'selected-raw';raw.mkdir(exist_ok=False)
subprocess.run(['cabextract','-q','-d',str(raw),str(selected)],check=True)
r=json.loads((w/'provenance.json').read_text());r['extraction_tool']='cabextract1.11';r['scope']='Static extraction of already-owned installer media, no installer/compiler/Windows executable executed; no prefixes changed';r['derived_vc_cab']={'operation':'File table restricted to selected original records; compressed folder unchanged','sha256':hashlib.sha256(selected.read_bytes()).hexdigest(),'selected_members':len(plans['vc'])}
r['msi_metadata']={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [w/'vs_setup.msi',w/'VistaClientHeadersLibs-x86.msi']}
for kind in ['vc','sdk']:
 if kind=='sdk':
  for cab in sorted(w.glob('VistaClientHeadersLibs-*.cab')):subprocess.run(['cabextract','-q','-d',str(raw),str(cab)],check=True)
 for member,meta in plans[kind].items():
  p=raw/member
  if not p.exists():raise ValueError('missing '+member)
  b=p.read_bytes();assert len(b)==meta['expected_bytes'],member;dest=w/meta['destination'];dest.parent.mkdir(parents=True,exist_ok=True);p.rename(dest)
  r['members'].append({'source_kind':kind,'member':member,**meta,'bytes':len(b),'sha256':hashlib.sha256(b).hexdigest()})
r['unselected_raw_files']=[p.name for p in raw.iterdir()]
if not r['unselected_raw_files']:raw.rmdir()
r['cabinet_sources']={p.name:{'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in w.glob('VistaClientHeadersLibs-*.cab')}
(w/'provenance.json').write_text(json.dumps(r,indent=2)+'\n');print({kind:len(plans[kind]) for kind in plans});print('output_bytes',sum(x['bytes'] for x in r['members']));print('unselected_raw',len(r['unselected_raw_files']))
