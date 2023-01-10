#!/bin/bash

NUMBER_OF_PEERS=4

# shellcheck disable=SC2004
for ((i = 1; i <= $NUMBER_OF_PEERS; i++));
do
    echo "Running peer $i"
    ./accountable-confirmer ../config/${NUMBER_OF_PEERS}_peers/peer${i}.conf &
done
