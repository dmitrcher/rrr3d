from pathlib import Path
import subprocess,gzip,json
p=Path(__file__).resolve().parent
include=p/'include'
programs=[]
for optimization in ['-O0','-O2']:
 exe=p/('verify-relative'+optimization)
 subprocess.run(['clang++','-std=c++17',optimization,'-ffp-contract=off','-I'+str(p/'include'),'-I'+str(include),str(p/'verify-relative.cpp'),'-o',str(exe)],check=True)
 programs.append(exe)
checks=[]
for path in sorted((p/'evidence/native').glob('*-private.jsonl.gz')):
 current=None
 for row in map(json.loads,gzip.open(path,'rt')):
  if row.get('function')=='geometry-dispatch-before':current=row
  if row.get('function')!='polygon-wrapper-entry':continue
  assert current is not None
  poses=current['geometry_pose0']['words']+current['geometry_pose1']['words']
  assert row['polygon_arg3']['words'][9:]==poses[7:10]
  assert row['polygon_arg8']['words'][9:]==poses[:3]
  expected=row['polygon_arg11']['words']+row['polygon_arg12']['words']
  for exe in programs:
   result=subprocess.run([str(exe)],input=' '.join(map(str,poses)),text=True,capture_output=True,check=True)
   actual=list(map(int,result.stdout.split()))
   different=[i for i,(a,b) in enumerate(zip(expected,actual)) if a!=b]
   checks.append(dict(capture=str(path),feature=row['polygon_args']['words'][14],optimization=exe.name,different_word_indices=different,expected_words=len(expected)))
assert checks
report=dict(checks=checks,all_relative_transforms_exact=all(not c['different_word_indices'] for c in checks),goal_complete=False)
(p/'relative-proof.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
assert report['all_relative_transforms_exact']
