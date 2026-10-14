from pathlib import Path
import gzip, json, hashlib
p = Path(__file__).resolve().parent

def rows(path):
    opener = gzip.open if path.suffix == '.gz' else open
    with opener(path, 'rt', encoding='utf-8-sig') as f:
        return [json.loads(line) for line in f if line.strip()]

raw = list(map(int, (p/'manifold-inputs.txt').read_text().split()))
triangles = [raw[22+i*9:22+(i+1)*9] for i in range(32)]
metadata = [rows(p/f'evidence/cooked-metadata-v145/metadata{i}.jsonl.gz') for i in range(2)]
assert metadata[0] == metadata[1] and len(metadata[0]) == 32
assert all(x['vertices'] in triangles for x in metadata[0])
assert [triangles.index(x['vertices']) for x in metadata[0]] == json.loads((p/'cooked-metadata-v145/proof.json').read_text())['native_face_remap']
for folder, source in [('observer', 'trace-contact.cpp'), ('cooked-metadata-v145/native', 'world-probe.cpp')]:
    info = json.loads((p/folder/'observer-build.json').read_text())
    assert hashlib.sha256((p/folder/source).read_bytes()).hexdigest() == info['source_sha256']
proof = json.loads((p/'proof.json').read_text())
snapshots = []
for repeat in range(2):
    trace = rows(p/f'evidence/native-step342/track0-run{repeat}-private.jsonl.gz')
    polygons = [x for x in trace if x.get('function') == 'polygon-wrapper-entry']
    assert len(polygons) == 2
    for polygon in polygons:
        assert polygon['sample_tick_v71'] == 342
        assert polygon['polygon_arg4']['words'] == proof['shared_plane0']
        assert polygon['polygon_arg10']['words'] == [1059212190, 3209017028, 1025787013]
        assert polygon['polygon_arg11']['words'] == proof['native_mesh_in_box']
        assert polygon['polygon_arg12']['words'] == proof['native_box_in_mesh']
    snapshots.append([{k: x[k]['words'] for k in ['polygon_arg1', 'polygon_arg4', 'polygon_arg6', 'polygon_arg8', 'polygon_arg9', 'polygon_arg10', 'polygon_arg11', 'polygon_arg12']} for x in polygons])
assert snapshots[0] == snapshots[1]
print('PASS: 32 exact cooked triangles, repeat-identical metadata and polygon inputs, shared plane, native transforms, and observer source hashes.')
