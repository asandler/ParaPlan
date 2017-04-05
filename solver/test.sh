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
            ../generator/task_generator "$4" "$i" "$j" > tests/cur_task
            for k in $(seq $3); do
                { time timeout $5 ./solver --spdi "$4" --task tests/cur_task --threads $k ; } > /dev/null 2> tests/tmp
                cat tests/tmp | grep "real" >> "tests/($t)_time"
            done
        done
    done
done

touch tests/overall
touch tests/overall_sorted

for t in $(seq $6); do
    paste "tests/overall" tests/\($t\)_time | sed 's/real//g' | sed 's/0m//g' | sed 's/s//g' | sed 's/\t/\ /g' | sed 's/\ \ /\ /g' | sed 's/^\ //g' > tests/tmp
    mv tests/tmp "tests/overall"

    sort tests/\($t\)_time > tests/\($t\)_time_sorted

    paste "tests/overall_sorted" tests/\($t\)_time_sorted | sed 's/real//g' | sed 's/0m//g' | sed 's/s//g' | sed 's/\t/\ /g' | sed 's/\ \ /\ /g' | sed 's/^\ //g' > tests/tmp
    mv tests/tmp "tests/overall_sorted"
done
