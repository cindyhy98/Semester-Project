#!/bin/bash
CORRECT_VALUE=334949
WRONG_VALUE=666666

PORT_ZERO=9000
PORT_ONE=9001
PORT_TWO=9002
PORT_THREE=9003

echo "starting replica 0"
./accountable-confirmer $PORT_ZERO $CORRECT_VALUE &
echo "starting replica 1"
./accountable-confirmer $PORT_ONE $CORRECT_VALUE &
echo "starting replica 2"
./accountable-confirmer $PORT_TWO $WRONG_VALUE &
echo "starting replica 3"
./accountable-confirmer $PORT_THREE $WRONG_VALUE &


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
