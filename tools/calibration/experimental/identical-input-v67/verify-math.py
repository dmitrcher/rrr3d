"""Compile independent retained Native float32 fixtures; no game/SDK required."""
from pathlib import Path
import argparse, json, subprocess, tempfile

p = argparse.ArgumentParser()
p.add_argument('--cxx', default='clang++')
p.add_argument('--out', type=Path)
args = p.parse_args()
here = Path(__file__).resolve().parent
results = []
with tempfile.TemporaryDirectory(prefix='motorrock-native-math-') as temp:
    for source, count in [('verify-prepared.cpp', 262), ('verify-frame.cpp', 420), ('verify-stop.cpp', 240)]:
        for optimization in ['-O0', '-O2']:
            binary = Path(temp) / 'verify'
            command = [args.cxx, '-std=c++17', optimization, '-ffp-contract=off', '-I'+str(here), str(here/source), '-o', str(binary)]
            subprocess.run(command, check=True)
            run = subprocess.run([str(binary)], capture_output=True, text=True, check=True)
            if source == 'verify-stop.cpp':
                detail = json.loads(run.stdout)
                assert detail['passed'] == count and detail['failed'] == 0 and detail['generic_mismatches'] == 6
            else:
                assert run.stdout == f'ok {count} failed 0\n'
                detail = {'passed': count, 'failed': 0}
            results.append({'source': source, 'optimization': optimization, **detail})
report = {'scope': 'Identical original Native inputs, exact float32 words; preparation/frame/solver subsets, not complete physics parity.', 'results': results, 'goal_complete': False}
if args.out:
    args.out.write_text(json.dumps(report, indent=2)+'\n')
print('922 saved Native float32 words pass at both -O0 and -O2.')
