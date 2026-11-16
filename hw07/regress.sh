#!/bin/bash

FAIL=0

echo "Running regression tests..."

for input_file in inputs/*.txt; do
    # Skip if no txt files exist
    [ -e "$input_file" ] || continue 

    base_name=$(basename "$input_file")
    expected_file="expected/$base_name"
    
    if [ ! -f "$expected_file" ]; then
        echo "FAIL: Missing expected output for $base_name"
        FAIL=1
        continue
    fi

    # Run balance, capture output and exit status
    ./balance < "$input_file" > output.tmp 2>&1
    status=$?
    echo "exit status $status" >> output.tmp

    # Compare
    if diff -u "$expected_file" output.tmp > /dev/null; then
        echo "PASS: $base_name"
    else
        echo "FAIL: $base_name"
        diff -u "$expected_file" output.tmp
        FAIL=1
    fi
done

rm -f output.tmp
exit $FAIL