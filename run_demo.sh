#!/bin/bash

rep=({1..4})
if [[ $# -gt 0 ]]; then
    rep=($@)
fi

for i in "${rep[@]}"; do
    echo "Running peer $i"
    ./accountable-confirmer ../config/peer${i}.conf &
done

#rep=({0..2})
#if [[ $# -gt 0 ]]; then
#    rep=($@)
#fi
#
#for i in "${rep[@]}"; do
#    echo "starting replica $i"
#    ./accountable-confirmer $i 9000+$i CORRECT_VALUE &
#done
#echo "starting replica 3"
#    ./accountable-confirmer 3 9003 WRONG_VALUE &
#wait
