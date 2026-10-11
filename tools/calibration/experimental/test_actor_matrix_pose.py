#!/usr/bin/env python3
"""Verify actor matrix input and stored quaternion against native PhysX fixtures."""
import argparse
import hashlib
import json
import struct
import subprocess
from pathlib import Path


def floats(values):
    return [struct.unpack('f', struct.pack('f', value))[0] for value in values]


def verify(helper, reference):
    fixture = json.loads(reference.read_text())
    cases = fixture['cases']
    request = ''.join(' '.join(map(str, floats(case['requested_quaternion'])))+'\n' for case in cases)
    result = subprocess.run([str(helper.resolve())], input=request, capture_output=True,
                            text=True, check=True, timeout=15)
    actual = [floats(list(map(float, line.split()))) for line in result.stdout.splitlines()]
    assert len(actual) == len(cases), 'Missing helper output'
    after_matrix_differences = []
    for case, row in zip(cases, actual):
        assert len(row) == 22, 'Wrong output shape'
        expected_matrix = floats([v for axis in range(3) for v in case[f'input_axis{axis}']])
        assert row[:9] == expected_matrix, (case['case'], 'Input matrix differs')
        assert row[9:13] == floats(case['quaternion']), (case['case'], 'Stored quaternion differs')
        # A different SDK arithmetic path generates the actual actor matrix.
        # Keep this diagnostic visible; decode(stored_q) is not its exact model.
        expected_after = floats([v for axis in range(3) for v in case[f'actual_axis{axis}']])
        if row[13:] != expected_after:
            after_matrix_differences.append({'case': case['case'],
                'maximum_error': max(abs(a-b) for a, b in zip(row[13:], expected_after))})
    return {'scope': fixture['source'], 'cases': len(cases),
            'input_matrix_and_stored_quaternion_exact_float32': True,
            'decode_stored_quaternion_matrix_differences': after_matrix_differences,
            'reference_sha256': hashlib.sha256(reference.read_bytes()).hexdigest(),
            'helper_sha256': hashlib.sha256(helper.read_bytes()).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--helper', type=Path, required=True)
    parser.add_argument('--reference', type=Path, default=Path(__file__).with_name('reference-initial-pose-v25.json'))
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    report = verify(args.helper, args.reference)
    args.out.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
