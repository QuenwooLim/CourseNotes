#!/usr/bin/env bash
set -euo pipefail

if [ ! -d bin ]; then
  echo "bin directory not found. Run ./build.sh first."
  exit 1
fi

java -cp bin Postfix
