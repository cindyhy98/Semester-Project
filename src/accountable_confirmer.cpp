/* Standard Library */
#include <unistd.h>
#include <fstream>
#include <boost/format.hpp>
#include <queue>
#include <charconv>

/* Internal Library */
#include "accountable_confirmer.h"

#define MAX_DIGIT 15 // biggest int is 2147482647

#define BUFFER_SIZE 4096

using namespace std;

namespace accountable_confirmer {
    int thisPid;
    /* Return 1 if found, Return 0 if not found */
    int FindPidInVector(vector<int> pidVector, int elem){
        vector<int>::iterator it = find(pidVector.begin(), pidVector.end(), elem);
        if (it != pidVector.end())   return 1;
        else    return 0;

    }

    void ShareSign(struct Peer* p) {
//        char valueToSign[MAX_DIGIT + sizeof(char)];
//        sprintf(valueToSign, "%d", p->msg.value);
//        char valueToSign[MAX_DIGIT + sizeof(char)];
//
//        std::to_chars(valueToSign, valueToSign + MAX_DIGIT, p->msg.value);

        auto msg = to_string(p->msg.value);

        /* Create a partial signature for the submit value (p->msg.value) */
        accountable_confirmer_bls::Sign(&p->keyPair, &p->msg.sig, msg.c_str());
    }

    /* Return 1 if it is valid, Return 0 if not valid */
    int ShareVerify(struct message::SubmitMsg* recvMsg) {

//        char valueSigned[MAX_DIGIT + sizeof(char)];
//        sprintf(valueSigned, "%d", recvMsg->value);

        auto msg = to_string(recvMsg->value);
        usleep(1000);
        /* Verify the partial signature of the submit value */
        if (accountable_confirmer_bls::Verify(&recvMsg->pub, &recvMsg->sig, msg.c_str())) {
            printf("[ShareVerify] [%d] Successful, for pid = %d\n", thisPid, recvMsg->pid);
            return 1;
        } else {
            printf("[ShareVerify] [%d] Fail, for pid = %d\n", thisPid, recvMsg->pid);
            return 0;
        }

    }

    /* Return 1 if verified, Return 0 if not verified */
    void SubmitMsgVerify(struct AccountableConfirmer* ac, struct message::SubmitMsg* recvMsg) {

        int id = recvMsg->pid;

        /* Authenticate the whole submit message */
        if (ShareVerify(recvMsg)) {
            if (ac->value[id] == recvMsg->value) {
                printf("[SubmitMsgVerify] Recv value(%d) of [%d] is equal to it's submit value(%d)\n",recvMsg->value, id, ac->value[id]);
                if (!FindPidInVector(ac->from[id], id)) {
                    printf("[SubmitMsgVerify] update accountable confirmer\n");

                    /* This recvMsg is verified, need to store it in the AccountableConfirmer */
                    ac->from[id].push_back(id);
                    ac->partialSignature[id].push_back(*recvMsg);
                } else {
                    printf("[SubmitMsgNotVerify] peer [%d] has already sent message\n", recvMsg->pid);
                }
            } else {
                printf("[SubmitMsgNotVerify] Recv value(%d) of [%d] is not equal to it's submit value(%d)\n",recvMsg->value, id, ac->value[id]);
            }
        }

    }

    /* Return 1 if verified, Return 0 if not verified */
//    int AggregateSignatureVerify(struct AccountableConfirmerOld* aco, struct message::SubmitAggSign * recvAggSign) {
//        // TODO: use FastAggSignVerify in accountable_confirmer_bls
//        return 0;
//    }

    /* Combine the received partial signatures into a aggregate signature */
    void GenerateAggSignature(struct Peer* p, struct AccountableConfirmer* ac) {
        int submitValue;
        vector<blsSignature> sigVec;
        vector<blsPublicKey> pubVec;

        for(auto & elem : ac->partialSignature[p->id]){
            sigVec.push_back(elem.sig);
            pubVec.push_back(elem.pub);
            submitValue = elem.value;
        }

        char valueToSign[MAX_DIGIT + sizeof(char)];
        sprintf(valueToSign, "%d", submitValue);
        const size_t msgSize = strlen(valueToSign);

        blsSignature aggSig;

        /* Create an aggregate signature for the collected partial signatures */
        accountable_confirmer_bls::AggSign(&aggSig, &sigVec[0], msgSize);

        p->aggSignMsg = {.pid = p->id, .value = submitValue, .aggSig = aggSig};
    }

    void BroadcastSubmitMessage(struct Peer* p) {
        printf("[BroadcastSubmitMessage] [%d] broadcast value %d to everyone\n",p->id, p->msg.value);

        /* Serialize blsSignature */
        byte bufSig[128];
        auto sig_sz = blsSignatureSerialize(bufSig, 128, &p->msg.sig);
        vector<byte> vSig; vSig.reserve(sig_sz);
        for (int i = 0; i < sig_sz; i++) vSig.push_back(bufSig[i]);

        /* Serialize blsPublicKey */
        byte bufPub[128];
        auto pub_sz = blsPublicKeySerialize(bufPub, 128, &p->msg.pub);
        vector<byte> vPub; vPub.reserve(pub_sz);
        for (int i = 0; i < pub_sz; i++)  vPub.push_back(bufPub[i]);


        /* Serialize Submit message */
        string message_type = "submit";
        auto pre = (boost::format("%1%::%2%::%3%::%4%::") % message_type % p->msg.pid % p->msg.value % sig_sz ).str();
        for (char i : pre) p->serializeMsg.push_back(byte(i));
        p->serializeMsg.insert(p->serializeMsg.end(), vSig.begin(), vSig.end());
        auto mid = (boost::format("::%1%::") % pub_sz ).str();
        for (char i : mid) p->serializeMsg.push_back(byte(i));
        p->serializeMsg.insert(p->serializeMsg.end(), vPub.begin(), vPub.end());
        auto end = (boost::format("::") ).str();
        for (char i : end) p->serializeMsg.push_back(byte(i));
        // printf("[BroadcastSubmitMessage] Sent byte = %lu\n", p->serializeMsg.size());

        // p->clients[broadcastIndex]->Post(message);
    }

    void BroadcastAggregateSignature(struct Peer* p) {
        printf("[BroadcastAggSignature] [%d] broadcast AggSignature to everyone\n", p->id);

        /* Serialize blsSignature (Aggregate Signature) */
        byte bufSig[128];
        auto sig_sz = blsSignatureSerialize(bufSig, 128, &p->aggSignMsg.aggSig);
        vector<byte> vSig; vSig.reserve(sig_sz);
        for (int i = 0; i < sig_sz; i++) vSig.push_back(bufSig[i]);

        /* Serialize SubmitAggSign */
        string message_type = "aggSign";
        auto pre = (boost::format("%1%::%2%::%3%::%4%::") % message_type % p->aggSignMsg.pid % p->aggSignMsg.value % sig_sz ).str();
        for (char i : pre) p->serializeAggSign.push_back(byte(i));
        p->serializeAggSign.insert(p->serializeAggSign.end(), vSig.begin(), vSig.end());
        auto end = (boost::format("::") ).str();
        for (char i : end) p->serializeAggSign.push_back(byte(i));
        // printf("[BroadcastAggSignature] Sent byte = %lu\n", p->serializeAggSign.size());

        // Send the message

    }

    void CheckRecvMsg(struct Peer* p, struct AccountableConfirmer* ac){
        printf("[CheckRecvMsg] Start recvMsgFlag = %d\n", p->recvMsgFlag);
        while (!p->recvMsgFlag) {
            int size = p->recvMsgQueue.size();
            if (size != 0) {
                printf("[CheckRecvMsg] [%d] Received queue size = %d\n",p->id, size);
                for (int i = 0; i < size; i++) {
                    SubmitMsgVerify(ac, &p->recvMsgQueue.front());
                    p->recvMsgQueue.pop();
                }
            }

            if (ac->from[p->id].size() > 0) {
                printf("[CheckRecvMsg] AC->from[%d] size = %lu\n", p->id, ac->from[p->id].size());
            }


            if (ac->from[p->id].size() >= NUMBER_OF_PROCESSES - NUMBER_OF_FAULTY_PROCESSES) {
                p->recvMsgFlag = true;
                printf("[CheckRecvMsg] [%d] Receiving enough messages -> progress to Confirm phase\n", p->id);
                Confirm(p, ac);
            }
        }

    }


    int CheckRecvAggSignature(struct Peer* p, struct AccountableConfirmer* ac){
        int size = p->recvAggSignQueue.size();
        printf("[CheckRecvAggSignature] [%d] Received queue size = %d\n",p->id, size);
        for (int i = 0; i < size; i++) {
            ac->obtainedAggSignature[p->id].push_back(p->recvAggSignQueue.front());
            p->recvAggSignQueue.pop();
        }

        int detect = 0;
        if (size >= 2) {
            printf("[CheckRecvAggSignature] receive enough aggSignatures -> progress to detect phase\n");
            detect = DetectConflictAggSignature(p, ac);
        }

        return detect;
    }

    /* Main functionality */
    void InitAccountableConfirmer(struct AccountableConfirmer* ac) {
        ac->value[0] = 334949;
        ac->value[1] = 334949;
        ac->value[2] = 334949;
        ac->value[3] = 666666;

        for (int i = 0; i < NUMBER_OF_PROCESSES; i++) {
//            ac->value[i] = 0;
            ac->confirm[i] = false;
            ac->from[i].clear();
            ac->partialSignature[i].clear();
            ac->obtainedAggSignature[i].clear();
        }

    }

    void InitPeer(struct Peer* p, struct AccountableConfirmer* ac, int id, int portNumber) {

        p->id = id;
        thisPid = id;

        /* Generate blsPublicKey and blsSecretKey for SubmitMsg Enc/Dec */
        accountable_confirmer_bls::Init();
        accountable_confirmer_bls::KeyGen(&p->keyPair);

        /* Clear the receiving queue */
        while (!p->recvMsgQueue.empty()) p->recvMsgQueue.pop();
        while (!p->recvAggSignQueue.empty()) p->recvAggSignQueue.pop();

        p->recvMsgFlag = false;
        p->recvAggSignFlag = false;

        thread runRecv(CheckRecvMsg, p, ac);
        p->recvThread.emplace_back(move(runRecv));

        printf("[InitPeer] [%d] Init, portNumber = %d\n", p->id, portNumber);
    }

    void Submit(struct Peer* p, struct AccountableConfirmer* ac, int v) {

        /* Init the submitMsg */
        p->msg.pid = p->id;
        p->msg.value = v;
        p->msg.pub = p->keyPair.pub;

        /* Init the partial signature for the submit value */
        ShareSign(p);

        /* Store this peer's value to the AccountableConfirmer */
        ac->value[p->id] = v;

        /* Broadcast the submitMsg to everyone */
        BroadcastSubmitMessage(p);

        printf("[Submit] [%d] submit value %d \n", p->id, v);
    }

    void Confirm(struct Peer* p, struct AccountableConfirmer* ac) {

        /* Confirm in the AccountableConfirmer */
        ac->confirm[p->id] = true;

        /* Generate the aggregate signature and store it in p */
        GenerateAggSignature(p, ac);

        /* Broadcast the aggregate signature to everyone */
        BroadcastAggregateSignature(p);

        printf("[Confirm] [%d] Confirmed\n", p->id);
    }

    int DetectConflictAggSignature(struct Peer* p, struct AccountableConfirmer* ac) {
        int detect = 0;
//        if(p->aco.obtainedAggSignature.size() <= 1){
//            // No need to compare
//            return detect;
//        }

        message::SubmitAggSign tmp = ac->obtainedAggSignature[p->id].front();


        for(vector<message::SubmitAggSign>::size_type i = 1; i != ac->obtainedAggSignature[p->id].size(); i++) {
            printf("[DetectConflictAggSignature] Detecting....\n");

            if(!(ac->obtainedAggSignature[p->id][i] == tmp) && ac->confirm[p->id]){
                printf("[DetectConflictAggSignature] first value = %d, second value = %d\n",tmp.value ,ac->obtainedAggSignature[p->id][i].value);
                printf("[DetectConflictAggSignature] Reach the proof\n");
                detect = 1;
                break;
            }
        }

        return detect;

    }

    void Close(struct Peer* p) {
        for (int i = 0; i < p->recvThread.size(); i++) {
            if (p->recvThread[i].joinable()) {
                p->recvThread[i].join();
            }

        }
        for (int i = 0; i < NUMBER_OF_PROCESSES; i++) {
            if (p->clientThread[i].joinable()) {
                printf("[%d] join thread %d\n",p->id, i);
                p->clientThread[i].join();
            } else {
                printf("[%d] thread %d is not joinable\n",p->id, i);
            }

        }
        printf("[Close] Successfully\n");
    }



}