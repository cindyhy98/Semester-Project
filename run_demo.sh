#!/bin/bash
CORRECT_VALUE=334949
WRONG_VALUE=666666

echo "starting replica 0"
./accountable-confirmer 0 9000 CORRECT_VALUE &
echo "starting replica 1"
./accountable-confirmer 1 9001 CORRECT_VALUE &
echo "starting replica 2"
./accountable-confirmer 2 9002 CORRECT_VALUE &
echo "starting replica 3"
./accountable-confirmer 3 9003 WRONG_VALUE &


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
