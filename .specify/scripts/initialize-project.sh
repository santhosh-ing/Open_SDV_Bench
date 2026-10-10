#!/usr/bin/env bash
set -eu

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

mkdir -p "$REPO_ROOT/specs" "$REPO_ROOT/docs" "$REPO_ROOT/interfaces" "$REPO_ROOT/ecus"

printf 'Project scaffold ready in %s\n' "$REPO_ROOT"
