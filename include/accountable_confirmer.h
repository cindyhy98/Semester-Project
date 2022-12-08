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
#define NUMBER_OF_FAULTY_PROCESSES 1
#define MAX_MESSAGE_LENGTH 512

using namespace std;

namespace accountable_confirmer {

    /* This is a global variable that record every peer's value */
    struct AccountableConfirmer {
        int value[NUMBER_OF_PROCESSES];
        bool confirm[NUMBER_OF_PROCESSES];
//        vector<int> from[NUMBER_OF_PROCESSES];
//        vector<message::SubmitMsg> partialSignature[NUMBER_OF_PROCESSES];
//        vector<message::SubmitAggSign> obtainedAggSignature[NUMBER_OF_PROCESSES];
        vector<int> from;
        vector<message::SubmitMsg> partialSignature;
        vector<message::SubmitAggSign> obtainedAggSignature;
    };

    struct Peer {
        int id;
        accountable_confirmer_bls::Key keyPair;   // for ShareSigned, ShareVerify
        message::SubmitMsg msg;
        vector<byte> serializeMsg;
        message::SubmitAggSign aggSignMsg;
        vector<byte> serializeAggSign;
        queue<string> rawMessage;
        queue<message::SubmitMsg> recvMsgQueue;
        queue<message::SubmitAggSign> recvAggSignQueue;

        vector<thread> clientThread;
        vector<thread> recvThread;
        bool recvMsgFlag;
        bool recvAggSignFlag;

    };

    /* Accountable Confirmer main functions */

    /* Create partial signature */
    void ShareSign(struct Peer* p);

    /* Verify partial signature
    * Return 1 if verified, Return 0 if not verified */
    int ShareVerify(struct message::SubmitMsg* recvMsg);

    /* Verify the whole submit message */
    void SubmitMsgVerify(struct Peer* p, struct AccountableConfirmer* ac, struct message::SubmitMsg* recvMsg);

    /* Combine the received partial signatures into aggregate signature */
    void GenerateAggSignature(struct Peer* p, struct AccountableConfirmer* ac);

    /* Verify the aggregate signature
     * Return 1 if verified, Return 0 if not verified */
//    int AggregateSignatureVerify(struct AccountableConfirmerOld* aco, struct message::SubmitAggSign * recvAggSign);

    void BroadcastSubmitMessage(struct Peer* p);

    void BroadcastAggregateSignature(struct Peer* p);

    void ParseMessage(struct Peer* p, struct AccountableConfirmer* ac);

    void ParseSubmitMessage(struct Peer* p, struct AccountableConfirmer* ac, string message);

    void ParseAggSignature(struct Peer* p, struct AccountableConfirmer* ac, string message);

    /* Init the AccountableConfirmer */
    void InitAccountableConfirmer(struct AccountableConfirmer* ac);

    void InitPeer(struct Peer* p, struct AccountableConfirmer* ac, int id, int portNumber);

    void Submit(struct Peer* p, struct AccountableConfirmer* ac, int v);

    void Confirm(struct Peer* p, struct AccountableConfirmer* ac);

    /* Check all received messages from the queue
     * If there's enough messages -> go to confirm phase
     * Return 1 if it's confirmed, otherwise return 0 */
    void CheckRecvMsg(struct Peer* p, struct AccountableConfirmer* ac);

    /* Check all received aggSignature from the queue
     * If there's enough aggSignature -> og to detect phase
     * Return 1 if detect some faulty guys, otherwise return 0 */
    int CheckRecvAggSignature(struct Peer* p, struct AccountableConfirmer* ac);

    /* Return true if detect conflict; otherwise return false */
    int DetectConflictAggSignature(struct Peer* p, struct AccountableConfirmer* ac);

    void Close(struct Peer* p);
}
#endif //ACCOUNTABLE_CONFIRMER_AC_H
