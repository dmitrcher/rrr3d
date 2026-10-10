#!/usr/bin/env python3
"""Compare single-race MotorRock JSONL captures; never asserts visual/physics parity."""
import argparse
import collections
import csv
import json
import math
from pathlib import Path


def read(path):
    rows = []
    for n, line in enumerate(Path(path).read_text(encoding='utf-8').splitlines(), 1):
        if line.strip():
            try:
                row = json.loads(line)
                if not isinstance(row, dict) or 'event' not in row:
                    raise ValueError('missing event')
                rows.append(row)
            except (ValueError, TypeError) as e:
                raise ValueError(f'{path}:{n}: {e}') from e
    if not rows:
        raise ValueError(f'{path}: empty trace')
    return rows


def normalize(path):
    return path.replace('\\', '/').lower()


def metadata(rows):
    return {
        'platforms': sorted({r.get('platform', '?') for r in rows}),
        'seeds': [r['detail'] for r in rows if r['event'] == 'rng.seed'],
        'races': [normalize(r['detail']) for r in rows if r['event'] == 'race.begin'],
        'events': dict(collections.Counter(r['event'] for r in rows)),
    }


def samples(rows):
    result = {}
    for r in rows:
        if r['event'] == 'physics.sample':
            s = r['sample']
            key = (s['tick'], s['racer'])
            if key in result:
                raise ValueError('Multiple sessions/races in one trace. Capture each run into a new file.')
            result[key] = s
    return result


def norm(a, b):
    return math.sqrt(sum((x-y)**2 for x, y in zip(a, b)))


def compare(left, right, destination, allow_different_seed=False):
    a, b = read(left), read(right)
    ma, mb = metadata(a), metadata(b)
    reasons = []
    if len(ma['races']) != 1 or len(mb['races']) != 1:
        reasons.append('Exactly one race per capture is required.')
    elif ma['races'] != mb['races']:
        reasons.append('Different maps.')
    if len(ma['seeds']) != 1 or len(mb['seeds']) != 1:
        reasons.append('Exactly one RNG seed per capture is required.')
    elif ma['seeds'] != mb['seeds'] and not allow_different_seed:
        reasons.append('Different seeds (use --allow-different-seed for randomness experiments).')
    sa, sb = samples(a), samples(b)
    shared = sorted(sa.keys() & sb.keys())
    if not shared:
        reasons.append('No matching physics samples.')
    elif any(abs(sa[k]['dt']-sb[k]['dt']) > 1e-7 for k in shared):
        reasons.append('Different simulation step sizes.')
    out = Path(destination)
    out.mkdir(parents=True, exist_ok=True)
    differences = []
    for tick, racer in shared:
        x, y = sa[tick, racer], sb[tick, racer]
        differences.append({
            'tick': tick, 'racer': racer,
            'position_error': norm(x['position'], y['position']),
            'velocity_error': norm(x['velocity'], y['velocity']),
            'speed_error': abs(x['speed']-y['speed']),
            'lap_error': abs(x['lap']-y['lap']),
            # q and -q describe the same rotation.
            'rotation_error_radians': 0.0 if x['rotation'] == y['rotation'] else 2*math.acos(min(1.0, abs(sum(u*v for u,v in zip(x['rotation'],y['rotation'])))/max(1e-30, norm(x['rotation'],[0]*4)*norm(y['rotation'],[0]*4)))),
        })
    with (out/'physics.csv').open('w', newline='') as f:
        fields = ['tick','racer','position_error','velocity_error','speed_error','lap_error','rotation_error_radians']
        writer = csv.DictWriter(f, fieldnames=fields)
        writer.writeheader(); writer.writerows(differences)
    for label, rows in [('left', a), ('right', b)]:
        with (out/f'{label}-timeline.csv').open('w', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(['elapsed_ms','utc','event','detail'])
            first = rows[0]['monotonic_ms']
            for row in rows:
                if row['event'] != 'physics.sample':
                    writer.writerow([round(row['monotonic_ms']-first,3), row['utc'], row['event'], row.get('detail','')])
    report = {
        'left': ma, 'right': mb, 'comparable_scenario': not reasons,
        'limitations': reasons + ['This checks captured trajectories only. Match car roster, difficulty, controls, camera, resolution and race start before interpreting cross-platform errors. Fixed seed does not equalize CRT RNG or physics engines.'],
        'matching_samples': len(shared), 'left_only_samples': len(sa.keys()-sb.keys()), 'right_only_samples': len(sb.keys()-sa.keys()),
        'identical_sample_values': bool(shared) and sa == sb,
        'maximum_errors': {k: max((r[k] for r in differences), default=None) for k in ['position_error','velocity_error','speed_error','lap_error','rotation_error_radians']},
    }
    (out/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    return report


def image_difference(left, right, out):
    try:
        from PIL import Image, ImageChops, ImageStat
    except ImportError as e:
        raise ValueError('Screenshot comparison requires Pillow: python3 -m pip install Pillow') from e
    a, b = Image.open(left).convert('RGB'), Image.open(right).convert('RGB')
    if a.size != b.size:
        raise ValueError(f'Screenshot resolutions differ: {a.size} vs {b.size}; capture at the same resolution.')
    diff = ImageChops.difference(a,b)
    diff.save(Path(out)/'image-difference.png')
    stats = ImageStat.Stat(diff)
    result = {'size': list(a.size), 'mean_absolute_error_rgb': stats.mean, 'root_mean_square_error_rgb': stats.rms}
    (Path(out)/'image-report.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('left'); p.add_argument('right'); p.add_argument('--out', required=True)
    p.add_argument('--allow-different-seed', action='store_true')
    p.add_argument('--images', nargs=2, metavar=('LEFT','RIGHT'))
    args = p.parse_args()
    try:
        result = compare(args.left, args.right, args.out, args.allow_different_seed)
        if args.images:
            result['images'] = image_difference(*args.images,args.out)
        print(json.dumps(result,indent=2))
        return 0 if result['comparable_scenario'] else 2
    except (OSError, ValueError, KeyError, TypeError) as e:
        p.exit(2, str(e)+'\n')

if __name__ == '__main__':
    raise SystemExit(main())
