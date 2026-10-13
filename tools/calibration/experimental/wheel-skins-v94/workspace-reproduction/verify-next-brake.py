from pathlib import Path
import gzip, json, struct

root = Path.cwd()
out = root / 'outputs/wheel-skins-v94'
bits = lambda x: struct.unpack('<I', struct.pack('<f', x))[0]
value = lambda x: struct.unpack('<f', struct.pack('<I', x))[0]
checks = []
for rep in range(2):
    with gzip.open(out / f'native-next-brake245/force-marauder-wheel1-brake-245-{rep}-private.jsonl.gz', 'rt') as stream:
        native = list(map(json.loads, stream))
    applied = [r for r in native if r.get('function') == 'spring-solve-applied']
    inputs = [r for r in native if r.get('function') == 'spring-solve-input']
    prepared = [r for r in native if r.get('function') == 'spring-row-ready']
    mac = out / f'mac-next-brake245/marauder-wheel1-brake-245-{rep}'
    solver = list(map(json.loads, open(str(mac) + '-solver.jsonl')))
    tire = list(map(json.loads, open(str(mac) + '-tire.jsonl')))
    results = [r for r in tire if r['event'].endswith('result')]
    tire_inputs = [r for r in tire if r['event'].endswith('input')]
    convert = lambda r: [r['linear'][0], r['linear'][2], r['linear'][1], -r['angular'][0], -r['angular'][2], -r['angular'][1]]
    mapped = []
    for iteration in range(1, 6):
        springs = [r for r in solver if r['iteration'] == iteration and r['phase'] in ('ordered-stop-after', 'spring-after')]
        assert len(springs) == 4, 'No hard-stop rows are active'
        mapped.extend([(r['velocity'] + r['angular'], r['lambda']) for r in springs])
        mapped.extend([(convert(r), r['lambda']) for r in results[(iteration - 1) * 8:iteration * 8]])
    assert len(mapped) == len(applied) == len(inputs) == 60
    first = next(i for i, ((body, lam), n) in enumerate(zip(mapped, applied)) if list(map(bits, body)) != [n['solve_body0']['words'][k] for k in (0, 1, 2, 4, 5, 6)])
    assert first == 5
    incoming = inputs[first]['solve_body0']['words']
    assert list(map(bits, convert(tire_inputs[1]))) == [incoming[k] for k in (0, 1, 2, 4, 5, 6)]
    assert len(prepared) == 12
    bounds = []
    for index, (n, m) in enumerate(zip(prepared[4:], tire_inputs[:8])):
        native_row = n['constraint_row']['words']
        assert bits(m['mass']) == native_row[11]
        assert bits(m['geometry_bias']) == native_row[7]
        bounds.append({'row': index, 'kind': m['event'], 'native_maximum': value(native_row[29]), 'mac_maximum': m['maximum'], 'maximum_float32_exact': bits(m['maximum']) == native_row[29]})
    assert [b['row'] for b in bounds if not b['maximum_float32_exact']] == [0, 1]
    checks.append({'repeat': rep, 'first_different_body_checkpoint': first, 'prior_complete_body_checkpoints_exact': 5, 'incoming_six_body_words_at_first_difference_exact': True, 'all_eight_tire_mass_and_geometry_bias_words_exact': True, 'tire_bounds': bounds, 'native_first_different_lambda': value(applied[first]['solve_row']['words'][28]), 'mac_first_different_lambda': mapped[first][1]})
assert checks[0] | {'repeat': 1} == checks[1]
report = {'scope': 'Actual physical step245 Marauder wheel1 moving-brake, after every observable step0..244 matched Windows. Two preserved Native captures and two frozen V94 Mac captures. First body divergence is first-iteration longitudinal tire row after four exact spring updates and one exact lateral update. Native tire bounds differ for only the first wheel; no active hard stops. Upstream bound inputs/branch cause still need measurement; this is a lead, not a coefficient-fit fix.', 'checks': checks, 'goal_complete': False}
(out / 'next-brake245-proof.json').write_text(json.dumps(report, indent=2) + '\n')
print('Both repeats: exact incoming body; first differing applied row5 clips at Native232.10446 vs Mac247.83633. Only first two tire bounds differ; upstream cause open.')
