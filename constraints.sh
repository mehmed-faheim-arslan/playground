#!/bin/bash
# Usage: ./constraints.sh [command...] [memory_limit_bytes]
# Last arg = memory limit (bytes) if numeric; default 1 MB

if [ $# -lt 1 ]; then
  echo "Usage: $0 [command...] [memory_limit_bytes]"
  exit 1
fi

if [[ "${@: -1}" =~ ^[0-9]+$ ]]; then
  MAX_MEMORY="${@: -1}"
  CMD=("${@:1:$#-1}")
else
  MAX_MEMORY=1048576             # 1 MB default
  CMD=("$@")
fi

if [ ${#CMD[@]} -eq 0 ]; then
  echo "Error: no command provided"
  exit 1
fi

echo "=== Memory Constraint Test ==="
echo "Command: ${CMD[*]}"
echo "Max allowed: $((MAX_MEMORY / 1024))KB"

ulimit -v "$MAX_MEMORY"
"${CMD[@]}"
EXIT_CODE=$?

if [ $EXIT_CODE -eq 137 ]; then
  echo "❌ FAILED: Out of memory!"
else
  echo "✓ PASSED: Within memory limit"
fi
