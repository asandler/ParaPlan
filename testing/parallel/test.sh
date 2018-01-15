#!/bin/bash

# $1 - max start edge intervals
# $2 - max finish edge intervals
# $3 - max threads number
# $4 - spdi file
# $5 - timeout (in seconds)
# $6 - tests number
# $7 - tests directory

rm -rf "$7"
mkdir -p "$7"

for i in $(seq $1); do
    for j in $(seq $2); do
        for t in $(seq $6); do
            task_generator "$4" "$i" "$j" > "$7"/cur_task

            { time timeout $5 solver --spdi "$4" --task "$7"/cur_task --threads $3 ; } > "$7"/answer 2> /dev/null

            ANSWER="unreach"
            if [ -s "$7"/answer ]; then
                FIRST_LINE=$(head -n1 "$7"/answer)
                if [ $FIRST_LINE == "1" ]; then
                    ANSWER="reach"
                fi

                for k in $(seq $3); do
                    { time timeout $5 solver --spdi "$4" --task "$7"/cur_task --threads $k ; } > /dev/null 2> "$7"/tmp
                    cat "$7"/tmp | grep "real" >> "$7/($k)_time_$ANSWER"
                done
            fi
        done
    done
done

rm "$7"/cur_task
touch "$7"/overall_unreach
touch "$7"/overall_reach

for k in $(seq $3); do
    paste "$7/overall_unreach" "$7"/\($k\)_time_unreach | sed 's/real//g' | sed 's/0m//g' | sed 's/s//g' | sed 's/\t/\ /g' | sed 's/\ \ /\ /g' | sed 's/^\ //g' > "$7"/tmp
    mv "$7"/tmp "$7/overall_unreach"
    paste "$7/overall_reach" "$7"/\($k\)_time_reach | sed 's/real//g' | sed 's/0m//g' | sed 's/s//g' | sed 's/\t/\ /g' | sed 's/\ \ /\ /g' | sed 's/^\ //g' > "$7"/tmp
    mv "$7"/tmp "$7/overall_reach"
done
