#!/bin/zsh
set -euo pipefail
calibration_dir="${0:A:h}"
calibration_binary="${RRR3D_CALIBRATION_BINARY:-${calibration_dir}/MotorRock-PhysicsCalibration-macOS}"
calibration_output="${1:-${HOME}/Library/Logs/RRR3d/calibration/$(date +%Y%m%d-%H%M%S)-${RANDOM}.jsonl}"
mkdir -p "${calibration_output:A:h}"
exec "${calibration_binary}" "${calibration_output}" "${2:-all}"
