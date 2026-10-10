#!/usr/bin/env bash
# Generates C pack/unpack code for the body ECU from the CAN contract.
# The output is generated code: never edit it by hand, regenerate instead.
#
# Usage: tools/gen_can.sh [output-dir]
#   default output-dir: ecus/body-zone-stm32/app/com/generated
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DBC="$REPO_ROOT/interfaces/can/body.dbc"
OUT="${1:-$REPO_ROOT/ecus/body-zone-stm32/app/com/generated}"

if ! python3 -c "import cantools" 2>/dev/null; then
  echo "cantools not found. Install with: pip install cantools" >&2
  exit 1
fi

mkdir -p "$OUT"
python3 -m cantools generate_c_source "$DBC" --output-directory "$OUT"
echo "Generated CAN code from $(basename "$DBC") into ${OUT#"$REPO_ROOT"/}"
