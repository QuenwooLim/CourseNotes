#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT_DIR"

bash ./testcase-001.sh
bash ./testcase-002.sh
bash ./testcase-003.sh
bash ./testcase-004.sh


