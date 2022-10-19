#ifndef ACCOUNTABLE_CONFIRMER_AC_H
#define ACCOUNTABLE_CONFIRMER_AC_H

/* Standard Library */
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <set>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

/* Internal Library*/
#include "testbls.h"

#define NUMBER_OF_PROCESS size_t(4)

using namespace std;

namespace accountable_confirmer{

//    struct Key {
//        vector<blsSecretKey> secVec;
//        vector<blsPublicKey> pubVec;
//    };
    struct MsgSubmit {
        int value;
        blsSignature sig;   // Signature for the value
    };

    struct Process {
        pid_t id;
        int value;          // The value this process submits
        blsSecretKey sec;   // Private key
        blsPublicKey pub;   // Public Key
        blsSignature sig;   // Signature
        MsgSubmit msg;
    };

    struct AccountableConfirmer {
        int value;
        bool confirm;

        set<pid_t> from;
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




//    struct BestEffortBroadcast {
//        int start;
//        struct MsgSubmit submit;
//        struct BroadcastMsg BMsg;
//        struct DeliverMsg DMsg;
//
//    };

    void InitProcess(struct Process* p);

    /* Create partial signature */
    void ShareSign(struct Process* p);

    /* Create submit message and signed it with the private key of the process */
    void CreateSubmitMsg(struct Process* p);



    void InitAC(struct AccountableConfirmer* ac);
    bool Submit(struct AccountableConfirmer* ac, int v);
    int Confirm(struct AccountableConfirmer* ac);
    void Detect(struct AccountableConfirmer* ac);

//    void InitBEB(struct BestEffortBroadcast* beb);
//    void Broadcast(struct BestEffortBroadcast* beb, struct BroadcastMsg BMsg);
//    void Deliver(struct BestEffortBroadcast* beb, struct AccountableConfirmer* ac);

}
#endif //ACCOUNTABLE_CONFIRMER_AC_H
