#!/bin/bash
CORRECT_VALUE=334949
WRONG_VALUE=666666

PORT_ZERO=9001
PORT_ONE=9002
PORT_TWO=9003
PORT_THREE=9004


./accountable-confirmer $PORT_ZERO  &
./accountable-confirmer $PORT_ONE  &
./accountable-confirmer $PORT_TWO  &
./accountable-confirmer $PORT_THREE  &


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
