#!/bin/bash

# a program to run tests for a comparing ARC and Ideal Cache

ARC_BIN="../../build/arc"
IDEAL_BIN="../../build/ideal"
OUTPUT_CSV="comparison_results.csv"

for bin in "$ARC_BIN" "$IDEAL_BIN"; do
    if [ ! -f "$bin" ]; then
        echo "ERROR: $bin not found!"
        exit 1
    fi
done

echo "test_id,arc_hits,ideal_hits" > "$OUTPUT_CSV"

echo "Running comparison on stress tests..."
echo "========================================"

for dat_file in *.dat; do
    test_id="${dat_file%.dat}"

    arc_out=$("$ARC_BIN" < "$dat_file" 2>/dev/null)             # Ideal
    if ! [[ "$arc_out" =~ ^[0-9]+$ ]]; then
        echo "Invalid ARC output for $test_id: '$arc_out'"
        arc_out="0"
    fi

    ideal_out=$("$IDEAL_BIN" < "$dat_file" 2>/dev/null)         # ARC
    if ! [[ "$ideal_out" =~ ^[0-9]+$ ]]; then
        echo "Invalid Ideal output for $test_id: '$ideal_out'"
        ideal_out="0"
    fi

    echo "$test_id,$arc_out,$ideal_out" >> "$OUTPUT_CSV"
    echo "Test $test_id: ARC=$arc_out, Ideal=$ideal_out"
done

echo "========================================"
echo "Results saved to $OUTPUT_CSV"
