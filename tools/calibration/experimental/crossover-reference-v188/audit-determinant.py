"""Bounded arithmetic replay from immutable Windows and CrossOver checkpoints."""
from pathlib import Path
import collections,gzip,hashlib,json,struct
ROOT=Path.cwd();W=ROOT/'work/crossover-reference-v188';O=ROOT/'outputs/crossover-reference-v188';P=W/'prefix/drive_c/MotorRock/game-debug/native-d3dx-instrumented0.jsonl';REF=ROOT/'outputs/controller-stabilize-v185/accepted-native0.jsonl.gz'
def load(p):
 with gzip.open(p,'rt') if p.suffix=='.gz' else p.open() as f:return [json.loads(s) for s in f if 'stabilize.pose' in s]
def f(word):return struct.unpack('<f',struct.pack('<I',word))[0]
def u(value):return struct.unpack('<I',struct.pack('<f',value))[0]
def rounded(value):return f(u(value))
a,b=load(P),load(REF);assert len(a)==len(b)==1440;counts=collections.Counter();examples=[];initial_differences=[]
for x,y in zip(a,b):
 assert x['case']==y['case']
 initial=[k for k in ['rotation_before','omega_before','momentum_before','inertia','inverse_inertia','mass_word','raw_fp_control','damping','roll_word','pitch_word','clutch_time_word','clutch_strength_word'] if x[k]!=y[k]]
 if initial:initial_differences.append(dict(case=x['case'],fields=initial))
 if not x['checkpoint3_hit']:continue
 assert y['checkpoint3_hit'] and x['checkpoint0'][:9]==y['checkpoint0'][:9]
 m=list(map(f,x['checkpoint0'][:9]));co=list(map(f,x['checkpoint3'][:9]));products=[rounded(co[i]*m[3*i]) for i in range(3)]
 # Original .text RVA4aa7f4..4aa812: three FMUL, two FADDP, FSTP32.
 p0,p1,p2=products;every_sum_rounded=u(rounded(rounded(p0+p1)+p2));register_sums_retained=u(p0+p1+p2)
 counts['cases']+=1;counts['cross_rounded_exact']+=every_sum_rounded==x['checkpoint3'][9];counts['cross_retained_exact']+=register_sums_retained==x['checkpoint3'][9];counts['windows_retained_exact']+=register_sums_retained==y['checkpoint3'][9];counts['windows_rounded_exact']+=every_sum_rounded==y['checkpoint3'][9];counts['cofactors_identical']+=x['checkpoint3'][:9]==y['checkpoint3'][:9]
 if every_sum_rounded!=register_sums_retained and len(examples)<4:examples.append(dict(case=x['case'],products=[u(z) for z in products],each_sum_rounded=every_sum_rounded,register_sums_retained=register_sums_retained,crossover=x['checkpoint3'][9],windows=y['checkpoint3'][9]))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report=dict(scope='Replay of bounded original inverse determinant instruction sequence using saved input words. Arithmetic-model evidence, not a new native Windows run or proof of a CPU defect. No change to Source physics.',counts=dict(counts),initial_differences=initial_differences,first_distinguishing_cases=examples,reference=str(REF.relative_to(ROOT)),reference_sha256=sha(REF),cross_capture_sha256=sha(P),disassembly_sha256=sha(ROOT/'work/controller-stabilize-v185/original-inverse.disasm.txt'),full_goal_complete=False)
(O/'determinant-arithmetic.json').write_text(json.dumps(report,indent=2)+'\n')
assert not initial_differences
assert counts['cases']==counts['cross_rounded_exact']==counts['windows_retained_exact']==counts['cofactors_identical']==1380
assert counts['cross_retained_exact']==counts['windows_rounded_exact']==690
print(json.dumps(dict(counts),indent=2))
