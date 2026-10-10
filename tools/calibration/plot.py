#!/usr/bin/env python3
"""Plot source-unit telemetry; requires matplotlib."""
import argparse
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from compare import load
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('windows');p.add_argument('baseline');p.add_argument('candidate');p.add_argument('--out',required=True);a=p.parse_args()
series=[('Windows reference',load(a.windows,'windows-physx-d3d9'),'#242938'),('Mac original',load(a.baseline,'mac-jolt-metal'),'#bd633b'),('Mac candidate',load(a.candidate,'mac-jolt-metal'),'#008b8b')]
fig,axes=plt.subplots(2,3,figsize=(13,7),constrained_layout=True)
for ax,(name,field,axis,label) in zip(axes.flat,[('force','position',0,'X distance'),('gravity','position',2,'Z position'),('wheel-drive','position',0,'X distance'),('brake','velocity',0,'X velocity'),('lateral','velocity',1,'Y velocity'),('suspension-drop','position',2,'Z position')]):
 for title,cases,color in series:
  rows=cases[name]['rows'];ax.plot([s['time'] for s in rows],[s[field][axis] for s in rows],label=title,color=color,lw=1.8)
 ax.set(title=name,xlabel='simulation time (s)',ylabel=label+' (source world units'+('/s)' if field=='velocity' else ')'))
 ax.grid(alpha=.2)
axes.flat[0].legend(fontsize=8)
fig.suptitle('MotorRock: controlled PhysX → Jolt calibration\nSynthetic four-wheel fixture; whole-game parity remains unverified',fontsize=13)
out=Path(a.out);out.parent.mkdir(parents=True,exist_ok=True)
fig.savefig(out,dpi=160);fig.savefig(out.with_suffix('.pdf'));plt.close(fig)
