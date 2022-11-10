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
#include "ecdsa_pki.h"
#include "socket.h"


#define NUMBER_OF_PROCESS size_t(4)

using namespace std;

namespace accountable_confirmer{



    struct AccountableConfirmer {
        int valueSubmit;

        bool confirm;
        vector<pid_t> from;
        vector<message::MsgSubmit> lightCert;
        vector<message::MsgToSend> fullCert;

        vector<message::MsgLightCert> obtainedLightCert; // set of aggregate signature?
        vector<message::MsgToSend> obtainedFullCert;

    };

    struct Process {
        pid_t id;
        ac_bls::Key aggregateKey;   // for ShareSigned, ShareVerify
        ecdsa_pki::Key pkiKey;      // pki
        message::MsgToSend msg;

        socket_t::Socket serverSocket;      // for receiving broadcast
        socket_t::Socket broadcastSocket;   // for sending broadcast

        AccountableConfirmer ac;
    };

//    int FindPidInVector(vector<pid_t> vectorPID, pid_t elem);

    void InitProcess(struct Process* p);

    /* Create partial signature */
    void ShareSign(struct Process* p);
    /* Verify partial signature */
    int ShareVerify(struct Process* p);

    /* Create submit message and sign it with the PKI private key of the process */
    void SubmitMsgSign(struct Process* p);

    /* Accountable Confirmer main functions */
    void InitAC(struct AccountableConfirmer* ac);

    int SubmitMsgVerify(struct AccountableConfirmer* ac, struct Process* recvP);

    bool Submit(struct AccountableConfirmer* ac, int v);

    void Confirm(struct Process* p);

    /* Combine the received partial signatures into light certificate */
    void CombinedLightCert(struct Process *p);

    void DetectConflictLightCert(struct AccountableConfirmer* ac);

//    void DetectConflictFullCert(struct AccountableConfirmer* ac);

    void BroadcastSubmitProcess(struct Process* submitProcess);

    void ReceiveSubmitProcess(struct AccountableConfirmer* ac, struct Process* submitProcess);

    void BroadcastLightCert(struct message::MsgLightCert* mlc, struct Process* p);

    void ReceiveLightCert(struct Process* p);
    // void BroadcastFullCert

}
#endif //ACCOUNTABLE_CONFIRMER_AC_H
