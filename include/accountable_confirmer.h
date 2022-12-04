#ifndef ACCOUNTABLE_CONFIRMER_AC_H
#define ACCOUNTABLE_CONFIRMER_AC_H

/* Standard Library */
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <thread>

/* External Library */
#include <openssl/sha.h>


/* Internal Library*/
#include "accountable_confirmer_bls.h"
#include "message.h"
//#include "ecdsa_pki.h"
#include "socket.h"
#include "tcp_connection.h"
#include "tcp_server.h"
#include "tcp_client.h"

#include <boost/asio.hpp>
typedef io::io_context ioContext;

#define NUMBER_OF_PROCESSES 4
#define NUMBER_OF_FAULTY_PROCESSES 2
#define MAX_MESSAGE_LENGTH 512

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
        vector<byte> serializeMsg;
        message::SubmitAggSignMsg aggSignMsg;
        vector<byte> serializeAggSign;
        queue<message::SubmitMsg> recvMsgQueue;
        queue<message::SubmitAggSignMsg> recvAggSignQueue;

//        socket_t::Socket serverSocket;      // for receiving broadcast
//        socket_t::Socket broadcastSocket;   // for sending broadcast
//        int peersPortNumber[NUMBER_OF_PROCESSES];
//        vector<TCPClient::pointer> clients;
        vector<thread> clientThread;
        AccountableConfirmer ac;
    };

    /* Accountable Confirmer main functions */

    /* Init the AccountableCOnfirmer inside a Process */
    void InitAC(struct AccountableConfirmer* ac);

    /* Create partial signature */
    void ShareSign(struct Process* p);

    /* Verify partial signature
     * Return 1 if verified, Return 0 if not verified */
    int ShareVerify(struct message::SubmitMsg* recvMsg);

    /* Verify the whole submit message
     * Return 1 if verified, Return 0 if not verified */
    int SubmitMsgVerify(struct AccountableConfirmer* ac, struct message::SubmitMsg* recvMsg);

    /* Verify the aggregate signature
     * Return 1 if verified, Return 0 if not verified
     * TODO: need to implement */
    int AggregateSignatureVerify(struct AccountableConfirmer* ac, struct message::SubmitAggSignMsg * recvAggSign);

//    void Start(struct Process* p);

    void InitProcess(struct Process* p, int portNumber, int *peersPortNumber);



    bool Submit(struct Process* p, int v);


    void Confirm(struct Process* p);

    /* Combine the received partial signatures into aggregate signature */
    void GenerateAggregateSignature(struct Process* p);

    /* Return true if detect conflict; otherwise return false */
    int DetectConflictAggSignature(struct Process* p);

    void BroadcastSubmitMessage(struct Process* p);

    /* Check all received messages from the queue
     * If there's enough messages -> go to confirm phase
     * Return 1 if it's confirmed, otherwise return 0 */
    int CheckRecvMsg(struct Process* p);

    void BroadcastAggregateSignature(struct Process* p, struct message::SubmitAggSignMsg* aggSignMsg);

    /* Check all received aggSignature from the queue
     * If there's enough aggSignature -> og to detect phase
     * Return 1 if detect some faulty guys, otherwise return 0 */
    int CheckRecvAggSignature(struct Process* p);

    void Close(struct Process* p);
}
#endif //ACCOUNTABLE_CONFIRMER_AC_H
