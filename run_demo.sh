#!/bin/bash
CORRECT_VALUE=334949
WRONG_VALUE=666666


rep=({0..2})
if [[ $# -gt 0 ]]; then
    rep=($@)
fi

for i in "${rep[@]}"; do
    echo "starting replica $i"
    ./accountable-confirmer $i 9000+$i CORRECT_VALUE &
done
echo "starting replica 3"
    ./accountable-confirmer 3 9003 WRONG_VALUE &
wait
