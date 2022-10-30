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

/* External Library */
#include <openssl/sha.h>

/* Internal Library*/
#include "ac_bls.h"
#include "message.h"
#include "esdsa_pki.h"


#define NUMBER_OF_PROCESS size_t(4)

using namespace std;

namespace accountable_confirmer{

//    struct Key {
//        vector<blsSecretKey> secVec;
//        vector<blsPublicKey> pubVec;
//    };


    struct Process {
        pid_t id;
        ac_bls::Key aggregateKey;   // for ShareSigned, ShareVerify
        ecdsa_pki::Key pkiKey;      //pki
        message::MsgSubmit msg;
    };

    struct AccountableConfirmer {
        message::MsgSubmit msg;
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

    /* Create submit message and sign it with the PKI private key of the process */
    void SubmitMsgSign(struct Process* p);


    /* Accountable Confirmer main functions */
    void InitAC(struct AccountableConfirmer* ac);
    bool Submit(struct AccountableConfirmer* ac, int v);
    int Confirm(struct AccountableConfirmer* ac);
    void Detect(struct AccountableConfirmer* ac);

//    void InitBEB(struct BestEffortBroadcast* beb);
//    void Broadcast(struct BestEffortBroadcast* beb, struct BroadcastMsg BMsg);
//    void Deliver(struct BestEffortBroadcast* beb, struct AccountableConfirmer* ac);

}
#endif //ACCOUNTABLE_CONFIRMER_AC_H
