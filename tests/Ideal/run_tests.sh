#!/bin/bash

# a program to run tests for a Ideal Cache
# usage: run_tests.sh <path_to_ideal_binary>

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

if [ $# -ne 1 ]; then
    echo "Usage: $0 <path_to_ideal_binary>"
    exit 1
fi

BINARY="$1"

if [ ! -f "$BINARY" ]; then
    echo "ERROR: binary '$BINARY' not found!"
    exit 1
fi

echo "Testing IDEAL cache with binary: $BINARY"
echo "========================================"

passed=0
failed=0

for dat_file in *.dat; do
    test_id="${dat_file%.dat}"
    ans_file="${test_id}.ans"

    if [ ! -f "$ans_file" ]; then
        echo "ERROR: missing $ans_file"
        exit 1
    fi

    expected=$(cat "$ans_file")
    output=$("$BINARY" < "$dat_file" 2>/dev/null)

    if ! [[ "$output" =~ ^[0-9]+$ ]]; then
        echo -e "${RED}[FAIL]${NC} $test_id: invalid output '$output'"
        ((failed++))
        continue
    fi

    if [ "$output" -eq "$expected" ]; then
        echo -e "${GREEN}[PASS]${NC} $test_id"
        ((passed++))
    else
        echo -e "${RED}[FAIL]${NC} $test_id: expected $expected, got $output"
        ((failed++))
    fi
done

echo "========================================"
echo "Ideal cache: $passed passed, $failed failed"
