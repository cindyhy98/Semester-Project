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
//#include "ecdsa_pki.h"
#include "socket.h"

#define NUMBER_OF_PROCESSES 4
#define NUMBER_OF_FAULTY_PROCESSES 1

using namespace std;

namespace accountable_confirmer {

    struct AccountableConfirmer {
        int submitValue;
        bool confirm;
        vector<int> from;
        vector<message::SubmitMsg> partialSignature;
        vector<message::SubmitAggSignMsg> obtainedAggSignature; // set of aggregate signatures

    };

    struct Process {
        int id;
        accountable_confirmer_bls::Key keyPair;   // for ShareSigned, ShareVerify
        message::SubmitMsg msg;

        socket_t::Socket serverSocket;      // for receiving broadcast
        socket_t::Socket broadcastSocket;   // for sending broadcast

        AccountableConfirmer ac;

        // this is a tmp value
        // -> as long as the implementation of broadcast is done, then this value can be removed
        message::SubmitAggSignMsg mlc;
    };


    /* Accountable Confirmer main functions */
    void InitAC(struct AccountableConfirmer* ac);

    /* Create partial signature */
    void ShareSign(struct Process* p);


    /* Verify partial signature
     * Return 1 if verified, Return 0 if not verified */
    int ShareVerify(struct Process* p);

    /* Verify the partial signature
     * Return 1 if verified, Return 0 if not verified */
    int SubmitMsgVerify(struct AccountableConfirmer* ac, struct Process* recvP);

    /* Verify the aggregate signature
     * Return 1 if verified, Return 0 if not verified
     * TODO: need to implement */
    int AggregateSignatureVerify(struct AccountableConfirmer* ac, struct Process* recvP);

    void InitProcess(struct Process* p, int portNumber);

    bool Submit(struct Process* p, int v);

    /* Return 1 if the condition meets, otherwise return 0 */
    int Confirm(struct Process* p);

    /* Combine the received partial signatures into aggregate signature */
    void GenerateAggregateSignature(struct Process *p);

    /* Return true if detect conflict; otherwise return false */
    bool DetectConflictAggregateSignature(struct Process *p);

    void BroadcastSubmitProcess(struct Process* submitProcess);

    void ReceiveSubmitProcess(struct Process* receiveProcess);

    void PseudoReceiveSubmitProcess(struct Process* receiveProcess, struct Process* submitProcess);

    void BroadcastAggregateSignature(struct message::SubmitAggSignMsg* mlc, struct Process* p);

    void ReceiveAggregateSignature(struct Process* p);

    void PseudoReceiveAggregateSignature( struct Process* receiveProcess, struct Process* submitProcess);


}
#endif //ACCOUNTABLE_CONFIRMER_AC_H
