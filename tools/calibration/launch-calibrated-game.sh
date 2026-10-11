#!/bin/zsh
set -euo pipefail
calibration_dir="${0:A:h}"
calibration_app="${RRR3D_MAC_APP:-${HOME}/Applications/MotorRock-Calibration.app}"
export RRR3D_CALIBRATION_PROFILE="${RRR3D_CALIBRATION_PROFILE:-${calibration_dir}/profiles/windows-source-v14-default.cfg}"
calibration_run="${HOME}/Library/Logs/RRR3d/calibration/game-$(date +%Y%m%d-%H%M%S)-${RANDOM}"
mkdir -p "${calibration_run}/screenshots"
export RRR3D_TRACE_FILE="${calibration_run}/trace.jsonl"
export RRR3D_CAPTURE_DIR="${calibration_run}/screenshots"
export RRR3D_SEED="${RRR3D_SEED:-42}"
if [[ "${1:-}" == "--physics-calibration" ]]; then
    exec "${calibration_app}/Contents/MacOS/RRR3d" "$@"
fi
exec "${calibration_app}/Contents/MacOS/RRR3d" --legacy-windows-debug "$@"
