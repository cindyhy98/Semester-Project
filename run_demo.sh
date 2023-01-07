#!/bin/bash

rep=({1..4})
if [[ $# -gt 0 ]]; then
    rep=($@)
fi

for i in "${rep[@]}"; do
    echo "Running peer $i"
    ./accountable-confirmer ../config/peer${i}.conf &
done
