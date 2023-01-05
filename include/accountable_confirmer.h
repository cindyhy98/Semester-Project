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
#include "tcp_connection.h"
#include "tcp_server.h"
#include "tcp_client.h"

#include <boost/asio.hpp>

#include <rb/core.h>
#include <rb/message.h>

#define NUMBER_OF_PROCESSES 4
#define NUMBER_OF_FAULTY_PROCESSES 1

#define MAX_PEERS_NUMBER 20
#define DEFAULT_PORT_NUMBER 8000

using namespace std;

namespace accountable_confirmer {


    struct AccountableConfirmer {
        int value;
        bool confirm;
        vector<int> from;
        vector<message::SubmitMsg> partialSignature;
        vector<message::SubmitAggSign> obtainedAggSignature;
    };

    struct Peer {
        int id;     // this can imply portNumber. i.e. id = 1, portNumber = 9001
        accountable_confirmer_bls::Key keyPair;   // for ShareSigned, ShareVerify
        message::SubmitMsg msg;
//        vector<byte> serializeMsg;
        message::SubmitAggSign aggSignMsg;
//        vector<byte> serializeAggSign;

        vector<thread> recvThread;
        bool recvMsgFlag;
        bool detectConflict;

        TCPClient clients[NUMBER_OF_PROCESSES];
        vector<thread> clientThread;


        AccountableConfirmer ac;

        /* Reliable Broadcast */
//        reliable_broadcast::Peer rb_peer;

    };

    /* Accountable Confirmer main functions */

    /* Create partial signature */
    void ShareSign(struct Peer* p);

    /* Verify partial signature
     * Return 1 if verified, Return 0 if not verified */
    int ShareVerify(struct message::SubmitMsg* recvMsg);

    /* Verify the whole submit message */
    void SubmitMsgVerify(struct Peer* p, struct message::SubmitMsg* recvMsg);

    /* Combine the received partial signatures into aggregate signature */
    void GenerateAggSignature(struct Peer* p);

    /* Verify the aggregate signature
     * Return 1 if verified, Return 0 if not verified */
//    int AggregateSignatureVerify(struct AccountableConfirmerOld* aco, struct message::SubmitAggSign * recvAggSign);

    void BroadcastSubmitMessage(struct Peer* p, int to);

    void BroadcastAggregateSignature(struct Peer* p, int to);

    void ParseAggSignature(struct Peer* p, string message);

    void ParseSubmitMessage(struct Peer* p, string message);

    void ParseMessage(struct Peer* p);

    /* Check all received messages from the queue
     * If there's enough verified messages -> go to confirm phase */
    void CheckRecvMsg(struct Peer* p);

    /* Check all received aggSignature from the queue
     * If there's enough aggSignature -> go to detect phase */
    void CheckRecvAggSignature(struct Peer* p);

    void InitClient(struct Peer* p, int index, int portNumber);

    void InitAccountableConfirmer(struct AccountableConfirmer* ac);

    void InitPeer(struct Peer* p, int id, int totalPeers);

    void Submit(struct Peer* p, int value, int to);

    void Confirm(struct Peer* p);

    /* Set detectConflict flag to true if detect conflict */
    void DetectConflictAggSignature(struct Peer* p);

    void Close(struct Peer* p);
}
#endif //ACCOUNTABLE_CONFIRMER_AC_H
