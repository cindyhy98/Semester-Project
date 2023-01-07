#ifndef ACCOUNTABLE_CONFIRMER_RELIABLE_BROADCAST_H
#define ACCOUNTABLE_CONFIRMER_RELIABLE_BROADCAST_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <thread>
#include "message.h"


using namespace std;

class ReliableBroadcast {
public:
    ReliableBroadcast() : sentecho(false), sentready(false), delivered(false) {
        echos.clear();
        readys.clear();
    }

public:
    bool sentecho;
    bool sentready;
    bool delivered;
    vector<message::RBMessage> echos;       // store all the echo message this peer receives
    vector<message::RBMessage> readys;      // store all the ready message this peer receives
};

#endif //ACCOUNTABLE_CONFIRMER_RELIABLE_BROADCAST_H
