#!/usr/bin/env python3
"""Verify the read-only v46 lead; this installs no runtime correction."""
import gzip
import hashlib
import json
import math
from pathlib import Path
import struct
import sys
import tempfile

base = Path(__file__).resolve().parent
measurements = base / 'measurements'
sys.path.insert(0, str(base.parent.parent))
import compare

def f32(value):
    return struct.unpack('<f', struct.pack('<f', value))[0]

def words(values):
    return [struct.unpack('<f', struct.pack('<I', v))[0] for v in values]

def exploratory_diagonal(values):
    a = [values[3*i:3*i+3] for i in range(3)]
    axes = [[float(i == j) for j in range(3)] for i in range(3)]
    norm = sum(a[i][i]**2 for i in range(3)) + 2*sum(
        a[i][j]**2 for i in range(3) for j in range(i+1, 3))
    threshold = norm * 9.999999949504854e-13
    for sweep in range(100):
        off = sum(a[i][j]**2 for i in range(3) for j in range(i+1, 3))
        if off < threshold:
            return [f32(a[i][i]) for i in range(3)], [f32(v) for row in axes for v in row]
        for p, q in [(0, 1), (0, 2), (1, 2)]:
            if abs(a[p][q]) < 9.999999682655225e-21:
                continue
            theta = (a[q][q] - a[p][p]) / (2*a[p][q])
            root = math.sqrt(theta*theta + 1)
            t = 1/(theta + root) if theta >= 0 else 1/(theta - root)
            c = 1/math.sqrt(t*t + 1)
            s = c*t
            delta = t*a[p][q]
            a[p][p] -= delta
            a[q][q] += delta
            a[p][q] = a[q][p] = 0
            for k in range(3):
                if k not in (p, q):
                    g, h = a[k][p], a[k][q]
                    a[k][p] = a[p][k] = c*g - s*h
                    a[k][q] = a[q][k] = s*g + c*h
                g, h = axes[k][p], axes[k][q]
                axes[k][p] = c*g - s*h
                axes[k][q] = s*g + c*h
    raise AssertionError('Exploratory model did not converge')

manifest = json.loads((measurements / 'manifest.json').read_text())
for name, expected in manifest.items():
    assert hashlib.sha256((measurements / name).read_bytes()).hexdigest() == expected, name
inputs = [[f32(float(v)) for v in line.split()] for line in (base / 'input.txt').read_text().splitlines()]
assert len(inputs) == 6
reports = []
for repeat in range(2):
    events = [json.loads(line) for line in (measurements / f'mass-run{repeat}.jsonl').read_text().splitlines()]
    entered = [o for o in events if 'dense_inertia' in o]
    ended = [o for o in events if 'principal_inertia' in o]
    assert len(entered) == len(ended) == 6
    rows = []
    for i, (before, after) in enumerate(zip(entered, ended)):
        dense = words(before['dense_inertia']['words'])
        assert dense == inputs[i]
        assert before['stack_return_rva'] == 0x13a0d8
        actual = (words(after['principal_inertia']['words']), words(after['principal_axes']['words']))
        assert exploratory_diagonal(dense) == actual
        rows.append(actual)
    reports.append(rows)
assert reports[0] == reports[1]
with tempfile.TemporaryDirectory() as temporary:
    loaded = []
    for name in ['untraced-reference', 'physical-run0', 'physical-run1']:
        path = Path(temporary) / (name + '.jsonl')
        path.write_bytes(gzip.decompress((measurements / (name + '.jsonl.gz')).read_bytes()))
        loaded.append(compare.load(path, 'windows-physx-d3d9'))
    assert loaded[0] == loaded[1] == loaded[2]
print('Six original dense matrices exact; principal/axes model exact; original 720 physical states preserved twice. No runtime correction installed.')
