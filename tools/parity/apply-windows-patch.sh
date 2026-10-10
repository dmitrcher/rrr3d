#!/bin/zsh
set -euo pipefail
if (( $# != 1 )); then
 print -u2 'Usage: apply-windows-patch.sh PATH_TO_ORIGINAL_WINDOWS_SDK_CHECKOUT'
 exit 2
fi
parity_source="${1:A}"
parity_patch="${0:A:h}/windows-reference.patch"
git -C "${parity_source}" apply --check --directory=prog "${parity_patch}"
git -C "${parity_source}" apply --directory=prog "${parity_patch}"
