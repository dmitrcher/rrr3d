import argparse,json,pathlib,subprocess,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements;here=pathlib.Path(__file__).resolve().parent;report={'scope':'Four offset-COM/shape fixtures, three original native steps, all eight solver fields','steps':{}}
for step in range(3):
 d=root/f'expanded-step{step}'
 subprocess.run([sys.executable,str(here/'compare-solver.py'),'--measurements',str(d),'--reference-dir',str(root),'--step',str(step)],check=True)
 result=json.loads((d/'solver-comparison.json').read_text());assert result['repeats']['0']==result['repeats']['1'];cases=result['repeats']['0'];assert set(cases)=={f'pose-v42-frozen-{i}' for i in [1,3,4,5]};assert all(not c['differences'] for c in cases.values());report['steps'][str(step)]=cases
report['rows']=sum(c['rows'] for cases in report['steps'].values() for c in cases.values());(root/'physics-v43-expanded-solver-audit.json').write_text(json.dumps(report,indent=2)+'\n');print('Expanded detailed solver rows exact:',report['rows'])
