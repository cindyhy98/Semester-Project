/* Standard Library */
#include <unistd.h>
#include <fstream>
#include <boost/format.hpp>
#include <queue>

/* Internal Library */
#include "accountable_confirmer.h"

#define MAX_DIGIT 15 // biggest int is 2147482647

#define BUFFER_SIZE 4096

using namespace std;

namespace accountable_confirmer {

    /* Return 1 if found, Return 0 if not found */
    int FindPidInVector(vector<pid_t> vectorPID, int elem){
        vector<int>::iterator it = find(vectorPID.begin(), vectorPID.end(), elem);
        if (it != vectorPID.end())   return 1;
        else    return 0;

    }

    void InitAC(struct AccountableConfirmer* ac) {
        ac->submitValue = 0;
        ac->confirm = false;
        ac->from.clear();
        ac->partialSignature.clear();
        ac->obtainedAggSignature.clear();
    }

    void ShareSign(struct Process* p) {
        char valueToSign[MAX_DIGIT + sizeof(char)];
        sprintf(valueToSign, "%d", p->msg.value);

        // Create a partial signature for the submit value (p->msg.value)
        accountable_confirmer_bls::Sign(&p->keyPair, &p->msg.sig, valueToSign);
    }

    /* Return 1 if it is valid, Return 0 if not valid */
    int ShareVerify(struct message::SubmitMsg* recvMsg) {
        char valueSigned[MAX_DIGIT + sizeof(char)];
        sprintf(valueSigned, "%d", recvMsg->value);

        /* Verify the partial signature of the submit value */
        if (accountable_confirmer_bls::Verify(&recvMsg->pub, &recvMsg->sig, valueSigned)) {
            printf("[ShareVerify] Successful\n");
            return 1;
        } else {
            printf("[ShareVerify] Fail\n");
            return 0;
        }

    }

    /* Return 1 if verified, Return 0 if not verified */
    int SubmitMsgVerify(struct AccountableConfirmer* ac, struct message::SubmitMsg* recvMsg) {

        /* Authenticate the whole submit message */
        if (ShareVerify(recvMsg) && (ac->submitValue == recvMsg->value) && !FindPidInVector(ac->from, recvMsg->submitPid)) {
            printf("[SubmitMsgVerify] Verify, Store the received message in the accountable confirmer\n");
            ac->from.push_back(recvMsg->submitPid);
            ac->partialSignature.push_back(*recvMsg);
            return 1;
        } else {
            if (ac->submitValue != recvMsg->value) {
                printf("[SubmitMsgVerify] Submit value (%d) != recv value (%d)\n", ac->submitValue, recvMsg->value);
            }
            printf("[SubmitMsgVerify] Not verify\n");
            return 0;
        }

    }

    /* Return 1 if verified, Return 0 if not verified */
    int AggregateSignatureVerify(struct AccountableConfirmer* ac, struct message::SubmitAggSignMsg * recvAggSign) {
        // TODO: use FastAggSignVerify in accountable_confirmer_bls
        return 0;
    }

    /* Combine the received partial signatures into a aggregate signature */
    void GenerateAggSignature(struct Process* p) {
        int submitValue;
        vector<blsSignature> sigVec;
        vector<blsPublicKey> pubVec;

        for(auto & elem : p->ac.partialSignature){
            sigVec.push_back(elem.sig);
            pubVec.push_back(elem.pub);
            submitValue = elem.value;
        }

        char valueToSign[MAX_DIGIT + sizeof(char)];
        sprintf(valueToSign, "%d", submitValue);
        const size_t msgSize = strlen(valueToSign);

        blsSignature aggSig;

        // Create an aggregate signature for the collected partial signatures
        accountable_confirmer_bls::AggSign(&aggSig, &sigVec[0], msgSize);

        p->aggSignMsg = {.value = submitValue, .aggSig = aggSig};
    }

    void BroadcastSubmitMessage(struct Process* p) {
        printf("[BroadcastSubmitMessage] [%d] broadcast value %d to everyone\n",p->id, p->msg.value);

        // Serialize blsSignature
        byte bufSig[128];
        auto sig_sz = blsSignatureSerialize(bufSig, 128, &p->msg.sig);
        vector<byte> vSig; vSig.reserve(sig_sz);
        for (int i = 0; i < sig_sz; i++) vSig.push_back(bufSig[i]);

        // Serialize blsPublicKey
        byte bufPub[128];
        auto pub_sz = blsPublicKeySerialize(bufPub, 128, &p->msg.pub);
        vector<byte> vPub; vPub.reserve(pub_sz);
        for (int i = 0; i < pub_sz; i++)  vPub.push_back(bufPub[i]);


        // Serialize Submit message
        string message_type = "submit";
        auto pre = (boost::format("%1%::%2%::%3%::%4%::") % message_type % p->msg.submitPid % p->msg.value % sig_sz ).str();
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




    int CheckRecvMsg(struct Process* p){
//        int size = p->recvMsgQueue.size();
//        printf("[CheckRecvMsg] [%d] Received queue size = %d\n",p->id, size);
//        for (int i = 0; i < size; i++) {
//            SubmitMsgVerify(&p->ac, &p->recvMsgQueue.front());
//            p->recvMsgQueue.pop();
//        }


        if (p->ac.from.size() >= NUMBER_OF_PROCESSES - NUMBER_OF_FAULTY_PROCESSES) {
            printf("[CheckRecvMsg] [%d] Receiving enough messages -> progress to Confirm phase\n", p->id);
            Confirm(p);
            return 1;
        } else {
            printf("[CheckRecvMsg] [%d] Not receiving enough messages\n", p->id);
            return 0;
        }

    }

    void BroadcastAggregateSignature(struct Process* p) {
        printf("[BroadcastAggSignature] [%d] broadcast AggSignature to everyone\n", p->id);

        // Serialize blsSignature (Aggregate Signature)
        byte bufSig[128];
        auto sig_sz = blsSignatureSerialize(bufSig, 128, &p->aggSignMsg.aggSig);
        vector<byte> vSig; vSig.reserve(sig_sz);
        for (int i = 0; i < sig_sz; i++) vSig.push_back(bufSig[i]);

        // Serialize Submit message
        string message_type = "aggSign";
        auto pre = (boost::format("%1%::%2%::%3%::") % message_type % p->msg.value % sig_sz ).str();
        for (char i : pre) p->serializeAggSign.push_back(byte(i));
        p->serializeAggSign.insert(p->serializeAggSign.end(), vSig.begin(), vSig.end());
        auto end = (boost::format("::") ).str();
        for (char i : end) p->serializeAggSign.push_back(byte(i));
        // printf("[BroadcastAggSignature] Sent byte = %lu\n", p->serializeAggSign.size());

        // Send the message

    }


    int CheckRecvAggSignature(struct Process* p){
        int size = p->recvAggSignQueue.size();
        printf("[CheckRecvAggSignature] [%d] Received queue size = %d\n",p->id, size);
        for (int i = 0; i < size; i++) {
            p->ac.obtainedAggSignature.push_back(p->recvAggSignQueue.front());
            p->recvAggSignQueue.pop();
        }

        int detect = 0;
        if (size >= 2) {
            printf("[CheckRecvAggSignature] receive enough aggSignatures -> progress to detect phase\n");
            detect = DetectConflictAggSignature(p);
        }

        return detect;
    }

    /* Main functionality */
    void InitProcess(struct Process* p, int portNumber, int *peersPortNumber) {

        p->id = portNumber;

        // Each process now has it own secret key and public key for enc/dec the message it commits
        accountable_confirmer_bls::Init();
        accountable_confirmer_bls::KeyGen(&p->keyPair);

        // Clear the receiving queue
        while (!p->recvMsgQueue.empty()) p->recvMsgQueue.pop();
        while (!p->recvAggSignQueue.empty()) p->recvAggSignQueue.pop();

        // Init the accountable confirmer
        InitAC(&p->ac);

        printf("[InitProcess] [%d] Init, portNumber = %d\n", p->id, portNumber);
    }

    bool Submit(struct Process* p, int v) {

        p->msg.submitPid = p->id;
        p->msg.value = v;
        p->msg.pub = p->keyPair.pub;


        p->ac.submitValue = v;

        // SharedSign on the submit value
        ShareSign(p);

        // Broadcast the signed submit message to everyone
        BroadcastSubmitMessage(p);

        printf("[Submit] [%d] submit value %d \n", p->id, v);

        return true;
    }


    void Confirm(struct Process* p) {
        printf("[Confirm] [%d] Confirmed\n", p->id);
        p->ac.confirm = true;
        // Generate aggregate signature and store it in p
        GenerateAggSignature(p);

        // Broadcast the aggregate signature to everyone
        BroadcastAggregateSignature(p);
    }

    int DetectConflictAggSignature(struct Process* p) {
        int detect = 0;
//        if(p->ac.obtainedAggSignature.size() <= 1){
//            // No need to compare
//            return detect;
//        }

        message::SubmitAggSignMsg tmp = p->ac.obtainedAggSignature.front();


        for(vector<message::SubmitAggSignMsg>::size_type i = 1; i != p->ac.obtainedAggSignature.size(); i++) {
            printf("[DetectConflictAggSignature] Detecting....\n");

            if(!(p->ac.obtainedAggSignature[i] == tmp) && p->ac.confirm){
                printf("[DetectConflictAggSignature] first value = %d, second value = %d\n",tmp.value ,p->ac.obtainedAggSignature[i].value);
                printf("[DetectConflictAggSignature] Reach the proof\n");
                detect = 1;
                break;
            }
        }

        return detect;


    }

    void Close(struct Process* p) {
        for (int i = 0; i < NUMBER_OF_PROCESSES; i++) {
//            p->clients[i]->Stop();

            if (p->clientThread[i].joinable()) {
                printf("[%d] join thread %d\n",p->id, i);
                p->clientThread[i].join();
            }

        }
        printf("Close Successfully\n");
    }



}