#!/bin/bash

TASK="./tmp_task"
TMP="./tmp"
REGIONS=60
GENERATIONS=50
GEN_ITERATIONS=1000

mkdir -p "./runs"

for spdi in ./spdi/*.spdi; do
    spdi_name="${spdi:7:1000}"
    echo "Start $spdi_name"

    for i in $(seq $GEN_ITERATIONS); do
        task_generator "$spdi" 1 1 > "$TASK"
        solver --spdi "$spdi" --task "$TASK" | sort | uniq | wc -l >> "./runs/$spdi_name.1"
        solver2 --spdi "$spdi" --task "$TASK" | sort | uniq | wc -l >> "./runs/$spdi_name.2"
        echo $i
    done

    echo "$spdi_name done"
done

#mkdir -p "./spdi/random"
#
#for i in $(seq $GENERATIONS); do
#    echo "Random SPDI #$i"
#
#    > "$TMP"
#    while [ ! -s "$TMP" ]; do
#        generator "$REGIONS" > "$TMP"
#    done
#
#    NUM=$(cat "$TMP" | grep ":" | grep "v_" | wc -l)
#    SPDI="./spdi/random/$NUM-$i.spdi"
#    mv "$TMP" "$SPDI"
#
#    task_generator "$SPDI" 1 1 > "$TASK"
#    solver --spdi "$SPDI" --task "$TASK" > /dev/null 2> /dev/null
#
#    if [ "$?" == "0" ]; then
#        for j in $(seq $GEN_ITERATIONS); do
#            task_generator "$SPDI" 1 1 > "$TASK"
#            solver --spdi "$SPDI" --task "$TASK" | sort | uniq | wc -l >> "./runs/random-$NUM-$i.1"
#            solver2 --spdi "$SPDI" --task "$TASK" | sort | uniq | wc -l >> "./runs/random-$NUM-$i.2"
#
#            echo "$j"
#        done
#    else
#        rm "$SPDI"
#    fi
#
#    echo "$SPDI completed"
#done

rm $TASK
