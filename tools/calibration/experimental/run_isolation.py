#!/usr/bin/env python3
"""Capture the native no-AI isolation ladder; stop Mac at its first failing stage."""
import argparse
import hashlib
import json
import os
import subprocess
from pathlib import Path
from verify_isolation import capture, verify


def stages():
    return {
        '00': [f'isolate-00-free-m{m}-v{v}' for m in range(3) for v in range(3)],
        '01': [f'isolate-01-accel-m{m}-a{a}-k{k}-v{v}' for m in range(3)
               for a in range(3) for k in range(6) for v in range(2)],
        '02': [f'isolate-02-gravity-m{m}-g{g}-v{v}-a{a}' for m in range(3)
               for g in range(2) for v in range(2) for a in range(4)],
        '03': [f'isolate-03-contact-m{m}-h{h}-v{v}-f{f}' for m in range(3)
               for h in range(2) for v in range(2) for f in range(2)],
    }


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--probe', type=Path, required=True)
    p.add_argument('--platform', choices=['windows', 'mac'], required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--reference-dir', type=Path)
    p.add_argument('--upper-face-winding', action='store_true',
                   help='Enable the v23 upper local-Z mesh-face winding experiment on Mac')
    p.add_argument('--negative-x-face-order', action='store_true',
                   help='Enable the v24 negative local-X face order experiment on Mac')
    p.add_argument('--principal-momentum', action='store_true',
                   help='Use principal-axis inertia to reconstruct Mac angular momentum')
    p.add_argument('--frozen-initial-normalize', action='store_true',
                   help='Normalize the saved frozen orientation once after its first step')
    p.add_argument('--material-frame-candidate', action='store_true',
                   help='Enable the unverified v27 material-frame arithmetic candidate')
    p.add_argument('--raw-contact-mass', action='store_true',
                   help='Include raw contact-normal length squared in translational effective mass')
    p.add_argument('--normal-row-trace', action='store_true',
                   help='Record source normal-row diagnostics with the v29 experimental SDK/adapter')
    p.add_argument('--contact-v35-cache', action='store_true',
                   help='Enable the v34 contact chain plus independent cached points/errors; requires the v35 patched probe')
    p.add_argument('--contact-v38-plane-normal', action='store_true',
                   help='Enable the v37 chain plus native triangle plane and contact normal decode; requires the v38 patched probe')
    p.add_argument('--contact-v40-cache-frame', action='store_true',
                   help='Enable the v39 chain plus a persistent raw world contact frame; requires the v40 patched probe')
    p.add_argument('--contact-v39-depth', action='store_true',
                   help='Enable the v38 chain plus separately rounded polygon depth projection; requires the v39 patched probe')
    p.add_argument('--contact-v37-inertia', action='store_true',
                   help='Enable the v36 chain plus independent native inertia rotation; requires the v37 patched probe')
    p.add_argument('--contact-v36-frame', action='store_true',
                   help='Enable the v35 chain plus reconstructed native material frame; requires the v36 patched probe')
    p.add_argument('--contact-v34', action='store_true',
                   help='Enable all independently calculated v34 contact candidates on Mac; requires the v34 patched probe')
    p.add_argument('--profile', type=Path,
                   default=Path(__file__).resolve().parents[1] / 'profiles/windows-source-v14.cfg')
    args = p.parse_args()
    if args.contact_v40_cache_frame:
        args.contact_v39_depth = True
    if args.contact_v39_depth:
        args.contact_v38_plane_normal = True
    if args.platform == 'mac' and args.reference_dir is None:
        p.error('Mac gates require two native Windows captures per stage')
    if (args.contact_v34 or args.contact_v35_cache or args.contact_v36_frame or args.contact_v37_inertia or args.contact_v38_plane_normal) and (args.platform != 'mac' or args.raw_contact_mass):
        p.error('Contact candidates are Mac-only and cannot be combined with --raw-contact-mass')
    args.out.mkdir(parents=True, exist_ok=True)
    env = {k: v for k, v in os.environ.items()
           if not k.startswith(('RRR3D_EXPERIMENT', 'RRR3D_CONTACT')) and k not in
           ('RRR3D_CALIBRATION_PROFILE', 'RRR3D_CONTACT_POINTS',
            'RRR3D_CALIBRATION_LEGACY_DIRECT_ROTATION', 'NX_SPRING_ITERATIONS')}
    env['RRR3D_SEED'] = '42'
    env['NX_SPRING_ITERATIONS'] = '4'
    if args.platform == 'mac':
        if args.upper_face_winding:
            env['RRR3D_EXPERIMENT_UPPER_FACE_WINDING'] = '1'
        if args.negative_x_face_order:
            env['RRR3D_EXPERIMENT_NEGATIVE_X_FACE_ORDER'] = '1'
        if args.principal_momentum:
            env['RRR3D_EXPERIMENT_PRINCIPAL_MOMENTUM'] = '1'
        if args.frozen_initial_normalize:
            env['RRR3D_EXPERIMENT_FROZEN_INITIAL_NORMALIZE'] = '1'
        if args.material_frame_candidate:
            env['RRR3D_EXPERIMENT_MATERIAL_FRAME_CANDIDATE'] = '1'
        if args.raw_contact_mass:
            env['RRR3D_EXPERIMENT_RAW_CONTACT_MASS'] = '1'
        env['RRR3D_CONTACT_POINTS'] = '1'
        if args.normal_row_trace:
            env['RRR3D_CONTACT_ROWS'] = '1'
        env['RRR3D_CALIBRATION_PROFILE'] = str(args.profile.resolve())
        for flag in ('SPRING_BIAS', 'SPRING_FINAL', 'SPRING_LOST', 'SPRING_COLD',
                     'FRICTION_ZERO', 'SUSPENSION_STOP', 'MESH_CONTACTS',
                     'TIRE_FORCE_HISTORY', 'SUSPENSION_RAW'):
            env['RRR3D_EXPERIMENT_' + flag] = '1'
        if args.contact_v34 or args.contact_v35_cache or args.contact_v36_frame or args.contact_v37_inertia or args.contact_v38_plane_normal:
            for flag in ('UPPER_FACE_WINDING', 'NEGATIVE_X_FACE_ORDER',
                         'PRINCIPAL_MOMENTUM', 'FROZEN_INITIAL_NORMALIZE',
                         'MATERIAL_FRAME_CANDIDATE', 'NATIVE_CONTACT_MASS',
                         'NATIVE_CONTACT_BIAS', 'NATIVE_CONTACT_POINT_VELOCITY',
                         'NATIVE_CONTACT_ORIGINAL_DELTA', 'NATIVE_CONTACT_FORCE_FIRST',
                         'NATIVE_CONTACT_ERROR_FRAME', 'NATIVE_MESH_EDGE'):
                env['RRR3D_EXPERIMENT_' + flag] = '1'
        if args.contact_v35_cache or args.contact_v36_frame or args.contact_v37_inertia or args.contact_v38_plane_normal:
            env['RRR3D_EXPERIMENT_NATIVE_CONTACT_CACHE'] = '1'
        if args.contact_v36_frame or args.contact_v37_inertia or args.contact_v38_plane_normal:
            env['RRR3D_EXPERIMENT_NATIVE_MATERIAL_FRAME'] = '1'
        if args.contact_v37_inertia or args.contact_v38_plane_normal:
            env['RRR3D_EXPERIMENT_NATIVE_CONTACT_INERTIA'] = '1'
        if args.contact_v38_plane_normal:
            env['RRR3D_EXPERIMENT_NATIVE_TRIANGLE_PLANE'] = '1'
            env['RRR3D_EXPERIMENT_NATIVE_SOLVER_NORMAL'] = '1'
        if args.contact_v39_depth:
            env['RRR3D_EXPERIMENT_NATIVE_POLYGON_DEPTH'] = '1'
        if args.contact_v40_cache_frame:
            env['RRR3D_EXPERIMENT_NATIVE_CACHE_FRAME'] = '1'
    manifest_path = args.out / 'isolation-run.json'
    if manifest_path.exists():
        p.error('Run manifest already exists; use a fresh output directory')
    manifest = {
        'platform': args.platform,
        'probe_sha256': hashlib.sha256(args.probe.read_bytes()).hexdigest(),
        'profile_sha256': (hashlib.sha256(args.profile.read_bytes()).hexdigest()
                           if args.platform == 'mac' else None),
        'contact_v34': args.contact_v34 or args.contact_v35_cache or args.contact_v36_frame or args.contact_v37_inertia or args.contact_v38_plane_normal,
        'contact_v35_cache': args.contact_v35_cache or args.contact_v36_frame or args.contact_v37_inertia or args.contact_v38_plane_normal,
        'contact_v36_frame': args.contact_v36_frame or args.contact_v37_inertia or args.contact_v38_plane_normal,
        'contact_v37_inertia': args.contact_v37_inertia or args.contact_v38_plane_normal,
        'contact_v38_plane_normal': args.contact_v38_plane_normal,
        'contact_v39_depth': args.contact_v39_depth,
        'contact_v40_cache_frame': args.contact_v40_cache_frame,
        'environment': {k: v for k, v in env.items()
                        if k.startswith(('RRR3D_', 'NX_SPRING_ITERATIONS'))},
        'stages': {}, 'status': 'capturing',
    }
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
    for stage, names in stages().items():
        files = []
        for repeat in range(2):
            dest = args.out / f'isolation-stage{stage}-run{repeat}-{args.platform}.jsonl'
            if dest.exists(): p.error(f'Capture already exists: {dest}; use a fresh output directory')
            chunks = []
            for start in range(0, len(names), 32):
                tmp = dest.with_suffix(f'.batch{start}.jsonl')
                subprocess.run([str(args.probe.resolve()), str(tmp.resolve()),
                    'list:' + ','.join(names[start:start+32])], env=env, check=True, timeout=90)
                chunks.append(tmp.read_bytes())
                tmp.unlink()
            dest.write_bytes(b''.join(chunks))
            cases, _ = capture(dest, 'windows-physx-d3d9' if args.platform == 'windows' else 'mac-jolt-metal')
            if set(cases) != set(names): p.error('Scenario set differs from isolation contract')
            if args.platform == 'mac' and args.normal_row_trace:
                traced = set()
                current = None
                with dest.open() as stream:
                    for line in stream:
                        row = json.loads(line)
                        if row['event'] == 'calibration.begin':
                            current = row['detail']
                        elif row['event'] == 'physics.source-normal-row':
                            traced.add(current)
                expected = {name for name in names if stage == '03' and name.endswith('-f1')}
                if traced != expected:
                    p.error('Normal-row trace coverage differs; use the v29 patched SDK/adapter')
            files.append(dest)
        if args.platform == 'mac':
            refs = [args.reference_dir / f'isolation-stage{stage}-run{r}-windows.jsonl' for r in range(2)]
            report = verify(*refs, *files)
            (args.out / f'isolation-stage{stage}-report.json').write_text(json.dumps(report, indent=2) + '\n')
            print(stage, report['exact_parity'], flush=True)
            manifest['stages'][stage] = {
                'cases': len(names), 'repeatable': report['repeatable'],
                'complete': report['exact_parity']['complete'],
                'remaining': len(report['exact_parity']['cases_remaining']),
                'capture_sha256': [c['sha256'] for c in report['captures']],
            }
            if not report['exact_parity']['complete']:
                manifest['status'] = 'parity_failed'
                manifest['first_failing_stage'] = stage
                manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
                return 1
            manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
    manifest['status'] = ('implemented_stages_passed' if args.platform == 'mac'
                          else 'reference_captured')
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
    return 0


if __name__ == '__main__': raise SystemExit(main())
