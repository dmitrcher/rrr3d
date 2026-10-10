#!/bin/zsh
set -euo pipefail
parity_root="${0:A:h:h:h}"
parity_app="${RRR3D_MAC_APP:-${parity_root}/build/macos-arm64-m10/Debug/RRR3d.app}"
parity_run="${RRR3D_RUN_DIR:-${HOME}/Library/Logs/RRR3d/parity/mac-$(date +%Y%m%d-%H%M%S)}"
mkdir -p "${parity_run}/screenshots"
export RRR3D_TRACE_FILE="${parity_run}/trace.jsonl"
export RRR3D_CAPTURE_DIR="${parity_run}/screenshots"
export RRR3D_SEED="${RRR3D_SEED:-42}"
export RRR3D_AI_DRIVER="${RRR3D_AI_DRIVER:-0}"
print -r -- "Trace: ${RRR3D_TRACE_FILE}"
# Choose --game-debug instead to preserve the release campaign/effects.
exec "${parity_app}/Contents/MacOS/RRR3d" --legacy-windows-debug "$@"
