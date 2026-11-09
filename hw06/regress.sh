#!/bin/bash

# Ensure temporary file deletion on exit
trap 'rm -f trace.tmp' EXIT
FAIL=0

# Loop through all text files in scenarios/
for scen in scenarios/*.txt; do
    filename=$(basename "$scen")
    expected="expected/$filename"
    
    if [ ! -f "$expected" ]; then
        echo "WARNING: Missing expected output file $expected"
        continue
    fi

    ./replay "$scen" > trace.tmp
    
    # Run regression diff[cite: 2]
    if diff -u "$expected" trace.tmp > /dev/null; then
        echo "REGRESS PASS: $filename"
    else
        echo "REGRESS FAIL: $filename (Differences found)"
        FAIL=1
    fi
done

exit $FAIL