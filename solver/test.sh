#!/bin/bash

# $4 - spdi file
# $5 - timeout (in seconds)

rm -rf tests
mkdir -p tests

for i in $(seq $1); do
    for j in $(seq $2); do
        ../generator/task_generator "$4" "$i" "$j" > "tests/test_($i)_($j)"
        for k in $(seq $3); do
            echo "$k" >> "tests/time_($i)_($j)"
            { time timeout $5 ./solver --spdi "$4" --task "tests/test_($i)_($j)" --threads $k ; } >> "tests/time_($i)_($j)" 2>> "tests/time_($i)_($j)"
        done
    done
done
