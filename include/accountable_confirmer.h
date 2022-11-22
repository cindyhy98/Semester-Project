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
#include <thread>

/* External Library */
#include <openssl/sha.h>

/* Internal Library*/
#include "accountable_confirmer_bls.h"
#include "message.h"
#include "ecdsa_pki.h"
#include "socket.h"

#define NUMBER_OF_PROCESS 4
#define NUMBER_OF_FAULTY 1

using namespace std;

namespace accountable_confirmer{

    struct AccountableConfirmer {
        int valueSubmit;

        bool confirm;
        vector<int> from;
        vector<message::MsgSubmit> lightCert;
        vector<message::MsgToSend> fullCert;

        vector<message::MsgLightCert> obtainedLightCert; // set of aggregate signature?
        vector<message::MsgToSend> obtainedFullCert;

    };

    struct Process {
        int id;
        ac_bls::Key aggregateKey;   // for ShareSigned, ShareVerify
        ecdsa_pki::Key pkiKey;      // pki
        message::MsgToSend msg;

        socket_t::Socket serverSocket;      // for receiving broadcast
        socket_t::Socket broadcastSocket;   // for sending broadcast

        AccountableConfirmer ac;

        // this is a tmp value
        // -> as long as the implementation of broadcast is done, then this value can be removed
        message::MsgLightCert mlc;
    };



//    int FindPidInVector(vector<pid_t> vectorPID, pid_t elem);

    void InitProcess(struct Process* p, int portNumber);


    /* Accountable Confirmer main functions */
    void InitAC(struct AccountableConfirmer* ac);

    /* Create partial signature */
    void ShareSign(struct Process* p);
    /* Sign the whole Submit message with PKI private key (secp256k1) */
    void SubmitMsgSign(struct Process* p);

    /* Verify partial signature */
    int ShareVerify(struct Process* p);


    /* Verify PKI key and the partial signature
     * Return 0 if verified, Return -1 if not verified */
    int SubmitMsgVerify(struct AccountableConfirmer* ac, struct Process* recvP);

    /* Verify the aggregate signature
     * Return 0 if verified, Return -1 if not verified */
    int LightCertVerify(struct AccountableConfirmer* ac, struct Process* recvP);

    bool Submit(struct Process* p, int v);

    /* Return 0 if the condition meets, otherwise return 1 */
    int Confirm(struct Process* p);

    /* Combine the received partial signatures into light certificate */
    void CombinedLightCert(struct Process *p);

    /* Return true if detect conflict; otherwise return false */
    bool DetectConflictLightCert(struct Process *p);

//    void DetectConflictFullCert(struct AccountableConfirmer* ac);

    void BroadcastSubmitProcess(struct Process* submitProcess);

    void ReceiveSubmitProcess(struct Process* receiveProcess);

    void PseudoReceiveSubmitProcess(struct Process* receiveProcess, struct Process* submitProcess);

    void BroadcastLightCert(struct message::MsgLightCert* mlc, struct Process* p);

    void ReceiveLightCert(struct Process* p);
    // void BroadcastFullCert

    void PseudoReceiveLightCert( struct Process* receiveProcess, struct Process* submitProcess);


}
#endif //ACCOUNTABLE_CONFIRMER_AC_H
