#ifndef ACCOUNTABLE_CONFIRMER_AC_H
#define ACCOUNTABLE_CONFIRMER_AC_H
#include <stdio.h>
#include <stdlib.h>

#include <cassert>
#include <set>
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

namespace accountable_confirmer{

    struct Process {
        pid_t P;
        // Private key
    };

    struct AccountableConfirmer {
        int value;
        bool confirm;
        set<int> from;
        set<int> lightCert;
        set<int> fullCert;
        set<int> obtainedLightCert;
        set<int> obtainedFullCert;

    };

    struct BroadcastMsg {
        // need to contain all kind of message to be broadcasted
    };

    struct DeliverMsg {
        // need to contain all kind of message to be delivered
    };


    struct MsgSubmit {
        int value;
        int shareSig;
        // ShareSignature(v)

    };


    struct BestEffortBroadcast {
        int start;
        struct MsgSubmit submit;
        struct BroadcastMsg BMsg;
        struct DeliverMsg DMsg;

    };

    void InitAC(struct AccountableConfirmer* ac);
    bool Submit(struct AccountableConfirmer* ac, int v);
    int Confirm(struct AccountableConfirmer* ac);
    void Detect(struct AccountableConfirmer* ac);

    void InitBEB(struct BestEffortBroadcast* beb);
    void Broadcast(struct BestEffortBroadcast* beb, struct BroadcastMsg BMsg);
    void Deliver(struct BestEffortBroadcast* beb, struct AccountableConfirmer* ac);

}
#endif //ACCOUNTABLE_CONFIRMER_AC_H
