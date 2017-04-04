mkdir -p tests

for i in $(seq $1); do
    for j in $(seq $2); do
        ../generator/task_generator ../examples/random_70.spdi "$i" "$j" > "tests/test_($i)_($j)"
        for k in $(seq $3); do
            echo "$k" >> "tests/time_($i)_($j)"
            { time ./solver --spdi ../examples/random_70.spdi --task "tests/test_($i)_($j)" --threads $k ; } >> "tests/time_($i)_($j)" 2>> "tests/time_($i)_($j)"
        done
    done
one
