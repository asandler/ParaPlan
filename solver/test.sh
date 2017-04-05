#!/bin/bash

# $1 - max start edge intervals
# $2 - max finish edge intervals
# $3 - max threads number
# $4 - spdi file
# $5 - timeout (in seconds)
# $6 - tests number

rm -rf tests
mkdir -p tests

for i in $(seq $1); do
    for j in $(seq $2); do
        for t in $(seq $6); do
            ../generator/task_generator "$4" "$i" "$j" > "tests/($i)_($j)_($t)_test"
            for k in $(seq $3); do
                { time timeout $5 ./solver --spdi "$4" --task "tests/($i)_($j)_($t)_test" --threads $k ; } >> "tests/($i)_($j)_answer" 2>> "tests/($i)_($j)_($t)_time"
                cat "tests/($i)_($j)_($t)_time" | grep "real" > tmp
                mv tmp "tests/($i)_($j)_($t)_time"
            done
        done
        paste tests/\($i\)_\($j\)_*_time | sed 's/real//g' > "tests/($i)_($j)_paste"
        rm tests/\($i\)_\($j\)_answer
        rm tests/\($i\)_\($j\)_*_time
    done
done
