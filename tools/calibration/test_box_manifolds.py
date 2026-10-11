#!/usr/bin/env python3
"""Check production contact physics against independent Nx SDK samples."""
import json
import math
import os
from pathlib import Path
import subprocess
import sys
import tempfile


def capture(executable, folder, enabled, names):
    profile = folder / ("profile-%d.cfg" % enabled)
    profile.write_text("physx_box_manifolds %d\nphysx_frozen_pose 1\n" % enabled)
    destination = folder / ("capture-%d.jsonl" % enabled)
    env = dict(os.environ)
    for key in list(env):
        if key.startswith("RRR3D_EXPERIMENT_"):
            del env[key]
    env["RRR3D_CALIBRATION_PROFILE"] = str(profile)
    subprocess.run([executable, str(destination), "list:" + ",".join(names)],
                   env=env, check=True, timeout=60, capture_output=True)
    samples = {}
    for line in destination.read_text().splitlines():
        event = json.loads(line)
        if event["event"] == "calibration.sample":
            samples[(event["detail"], event["sample"]["tick"])] = event["sample"]
    return samples


def main():
    executable, oracle_path = sys.argv[1:]
    oracle = json.loads(Path(oracle_path).read_text())
    names = sorted(oracle["cases"])
    limits = {"position": 5e-5, "velocity": 5e-5,
              "angular_momentum": .002, "rotation": 2e-6}
    with tempfile.TemporaryDirectory(prefix="motorrock-box-reference-") as temp:
        folder = Path(temp)
        actual = capture(executable, folder, 1, names)
        count = 0
        for name, case in oracle["cases"].items():
            for expected in case["samples"]:
                row = actual[(name, expected["tick"])]
                row_limits = dict(limits)
                if expected["tick"] >= 500:
                    # Bound accumulated ten-second angular drift separately.
                    # Exact residuals remain visible in the full parity report.
                    row_limits.update(angular_momentum=.01, rotation=5e-5)
                for field, limit in row_limits.items():
                    error = max(abs(a-b) for a,b in zip(row[field], expected[field]))
                    if not math.isfinite(error) or error > limit:
                        raise AssertionError((name, expected["tick"], field, error, limit))
                count += 1
        legacy = capture(executable, folder, 0, names)
        negative_error = max(
            abs(a-b)
            for name, case in oracle["cases"].items()
            for expected in case["samples"]
            for field in ("position", "velocity")
            for a,b in zip(legacy[(name, expected["tick"])][field], expected[field]))
        if negative_error < .001:
            raise AssertionError("The independent oracle did not reject the legacy solver")
        print("%d independent Nx samples passed; legacy error %.6g" % (count, negative_error))


if __name__ == "__main__":
    main()
