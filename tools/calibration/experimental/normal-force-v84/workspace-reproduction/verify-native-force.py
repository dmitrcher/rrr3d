"""Portable verification of immutable Windows observations and the actual C++ helper."""
from pathlib import Path
import argparse, gzip, hashlib, json, struct, subprocess, tempfile

root = Path(__file__).resolve().parent
p = argparse.ArgumentParser()
p.add_argument('--check-cpp', action='store_true')
args = p.parse_args()
bits = lambda x: struct.unpack('<I', struct.pack('<f', x))[0]
value = lambda x: struct.unpack('<f', struct.pack('<I', x))[0]
F = lambda x: value(bits(x))
reference = {}
with gzip.open(root / 'captures/windows-reference.jsonl.gz', 'rt') as stream:
    for r in map(json.loads, stream):
        if r['event'] == 'calibration.sample':
            reference.setdefault(r['detail'], []).append(r['sample'])
samples = captures = callbacks = 0
for folder in ['native-force', 'native-callback', 'native-callback-branches', 'native-kernel']:
    directory = root / 'captures' / folder
    manifest = json.loads((directory / 'manifest.json').read_text())
    assert any('3935cb983910290e2ff844bb437286d140d0fafa6cfbcbfbcbfd0f07b6208262' in h for h in manifest['hashes'].values())
    assert any('c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb' in h for h in manifest['hashes'].values())
    files = sorted(directory.glob('*-private.jsonl.gz'))
    assert len(files) == len(manifest['captures'])
    for path in files:
        raw = gzip.open(path, 'rb').read()
        assert hashlib.sha256(raw).hexdigest() in {c['private_logical_sha256'] for c in manifest['captures']}
        rows = [r for r in map(json.loads, raw.decode().splitlines()) if 'function' in r]
        tick = rows[0]['sample_tick_v71']
        assert rows and all(r['sample_tick_v71'] == tick for r in rows)
        physical = list(map(json.loads, gzip.open(str(path).replace('-private', '-physical'), 'rt')))
        meta = next(r for r in physical if r['event'] == 'calibration.begin')
        actual = [r['sample'] for r in physical if r['event'] == 'calibration.sample']
        assert actual == reference[meta['detail']]
        samples += len(actual)
        captures += 1
        selected = [r for r in rows if r['function'].startswith('callback')]
        for i in range(0, len(selected), 3):
            entry, loaded, exit = selected[i:i + 3]
            cfg = entry['callback_config']['words']
            mass = meta['sample']['mass']
            normal = value(entry['callback_normal']['words'][2])
            gravity, spring = map(value, cfg[1:3])
            incoming = value(entry['callback_input_force']['words'][0])
            load = abs(F(mass * normal * gravity / 4))
            assert loaded['x87_control'] == 895
            assert bits(load) == loaded['callback_load']['words'][0]
            force = 0 if not (cfg[3] & 255) or spring > 0 and incoming > F(load * spring) else min(incoming, F(load * 1.5))
            assert bits(force) == exit['callback_output_force']['words'][0]
            callbacks += 1
assert (captures, callbacks) == (20, 48)
if args.check_cpp:
    with tempfile.TemporaryDirectory(prefix='motorrock-v84-proof-') as tmp:
        for mode in ['-O0', '-O2']:
            exe = Path(tmp) / mode[1:]
            subprocess.run(['clang++', '-std=c++17', mode, '-I', str(root / 'proofs'), str(root / 'proofs/verify-force.cpp'), '-o', str(exe)], check=True)
            subprocess.run([str(exe)], check=True)
print(f'{captures} immutable Windows captures, {samples} Golden-exact states, {callbacks} exact callback loads/forces; goal incomplete.')
