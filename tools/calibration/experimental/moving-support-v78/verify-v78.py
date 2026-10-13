from pathlib import Path
import argparse, gzip, json, runpy, subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--captures', type=Path, default=Path(__file__).resolve().parent/'captures')
parser.add_argument('--build', type=Path, required=True)
args = parser.parse_args()
here = Path(__file__).resolve().parent
native_sort = runpy.run_path(str(here/'native-endpoint-sort.py'))['native_sort']
read = lambda p: list(map(json.loads, gzip.open(p, 'rt')))
pairs = lambda r: list(zip(r['sort_endpoints']['words'][::2], r['sort_endpoints']['words'][1::2]))
fixtures, repeat_groups = [], {}
for path in sorted((args.captures/'sort-matrix').glob('*-private.jsonl.gz')):
    rows = [r for r in read(path) if 'sort_endpoints' in r]
    assert len(rows) == 6, (path, len(rows))
    operations = []
    for before, after in zip(rows[::2], rows[1::2]):
        assert before['function'] == 'endpoint-sort-before'
        assert after['function'] in ('endpoint-sort-after-z', 'endpoint-sort-after-xy')
        assert before['sample_tick_v71'] == after['sample_tick_v71'] == 0
        original, expected = pairs(before), pairs(after)
        assert native_sort(original) == expected, (path, after['function'])
        fixtures.append((original, expected))
        operations.append((original, expected))
    repeat_groups.setdefault(path.name.rsplit('-', 2)[0], []).append(operations)
assert len(repeat_groups) == 6 and len(fixtures) == 36
assert all(len(r) == 2 and r[0] == r[1] for r in repeat_groups.values())
manifest = json.loads((args.captures/'sort-matrix'/'manifest.json').read_text())
assert len(manifest['captures']) == 12
assert all(c['physical_golden_exact'] for c in manifest['captures'])

# Predict onset order from the original general sort, before any contacts exist.
# The later capture is used only as the independent oracle for that prediction.
initial = read(next((args.captures/'sort-matrix').glob('*pair-stock-pole-1-0-0-private.jsonl.gz')))
z_after_index = next(i for i, r in enumerate(initial) if r.get('function') == 'endpoint-sort-after-z')
initial_z = pairs(initial[z_after_index - 1])
sorted_z = list(reversed(native_sort(initial_z)))
pipeline = []
for path in sorted((args.captures/'endpoints-onset').glob('*-private.jsonl.gz')):
    rows = read(path)
    crossing = next(r for r in rows if r.get('function') == 'z-max-forward-start')
    inserts = [r['stack']['words'][1:3] for r in rows if r.get('function') == 'adjacency-insert']
    assert len(inserts) == 4 and all(i[0] == inserts[0][0] for i in inserts)
    owners = {i[1] for i in inserts}
    predicted = [v[1] & 0x7fffffff for v in sorted_z
                 if v[1] & 0x80000000 and (v[1] & 0x7fffffff) in owners]
    assert predicted == [i[1] for i in inserts]
    endpoints = list(zip(crossing['endpoints_z']['words'][::2], crossing['endpoints_z']['words'][1::2]))
    start = crossing['eax']
    assert [v[1] & 0x7fffffff for v in endpoints[start + 1:start + 5]] == predicted
    getter = next(r for r in rows if r.get('function') == 'broadphase-created-api')
    count = getter['broadphase_api']['words'][0x64 // 4]
    bitmap = getter['created_bitmap']['words'][:count]
    nodes = getter['adjacency_nodes']['words']
    shorts = [v for word in nodes for v in (word & 0xffff, word >> 16)]
    exported = []
    for word_index, word in enumerate(bitmap):
        for bit in range(32):
            if word & (1 << bit):
                node, slot = divmod(word_index * 32 + bit, 4)
                exported.append([shorts[node * 6 + 5], shorts[node * 6 + slot]])
    assert exported == inserts
    pipeline.append({'capture': path.name, 'predicted_bounds_ids': predicted,
                     'crossing_order_exact': True, 'storage_export_order_exact': True})
assert len(pipeline) == 2

payload = [str(len(fixtures))]
for original, expected in fixtures:
    payload.append(str(len(original)))
    payload.extend(f'{a} {b}' for a, b in original + expected)
input_text = '\n'.join(payload) + '\n'
args.build.mkdir(parents=True, exist_ok=True)
checks = []
for optimization in ('O0', 'O2'):
    binary = args.build/f'verify-sort-{optimization}'
    subprocess.run(['xcrun', 'clang++', '-std=c++17', '-'+optimization,
                    str(here/'verify-sort.cpp'), '-o', str(binary)], check=True)
    result = subprocess.run([str(binary)], input=input_text, text=True,
                            check=True, capture_output=True)
    checks.append({'optimization': optimization, 'output': result.stdout.strip()})
report = {'scope': 'Original initialization sort and dynamic-support first-contact ordering only',
          'sort_operations': 36, 'raw_output_words_per_implementation': 1440,
          'python_exact': True, 'cpp_checks': checks, 'six_scenarios_two_repeats_exact': True,
          'original_physical_golden_samples_unchanged': 3600, 'pipeline': pipeline,
          'hardcoded_wheel_permutation': False, 'live_physics_modified': False,
          'full_suite_strict_cases_unchanged': 116, 'full_suite_total_cases': 608,
          'goal_complete': False,
          'remaining': ['Reconstruct endpoint merging/update and pair storage as generic persistent state.',
                        'Integrate original contact creation order in default-off Mac research profile.',
                        'Verify first divergence, trajectories and full regressions before promotion.']}
(args.build/'result.json').write_text(json.dumps(report, indent=2)+'\n')
print(json.dumps(report, indent=2))
