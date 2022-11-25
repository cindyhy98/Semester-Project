/* Standard Library */
#include <unistd.h>
#include <fstream>

/* Internal Library */
#include "accountable_confirmer.h"

#define MAX_DIGIT 15 // biggest int is 2147482647
#define BUFFER_SIZE 4096

using namespace std;

namespace accountable_confirmer {

    struct Process P[NUMBER_OF_PROCESSES];

    /* Return 1 if found, Return 0 if not found */
    int FindPidInVector(vector<pid_t> vectorPID, pid_t elem){
        vector<pid_t>::iterator it = find(vectorPID.begin(), vectorPID.end(), elem);
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
        printf("[ShareSign] [%d] Create a partial signature for the submit value %d\n",p->id, p->msg.value);

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
            printf("[SubmitMsgVerify] Verify\n");
            printf("[SubmitMsgVerify] Store the received message in the accountable confirmer\n");
            ac->from.push_back(recvMsg->submitPid);
            ac->partialSignature.push_back(*recvMsg);
            return 1;
        } else {
            printf("[SubmitMsgVerify] Not verify\n");
            return 0;
        }

    }

    /* Return 1 if verified, Return 0 if not verified */
    int AggregateSignatureVerify(struct AccountableConfirmer* ac, struct message::SubmitAggSignMsg * recvAggSign) {
        // TODO: use FastAggSignVerify in accountable_confirmer::_blscpp
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

    void BroadcastSubmitMessage(struct Process* submitProcess) {
        printf("[BroadcastSubmitMessage] [%d] broadcast value %d to everyone\n",submitProcess->id, submitProcess->msg.value);

        // Old way of serialize the message
         char buf[sizeof(submitProcess->msg)+1];
         memcpy(buf, &submitProcess->msg, sizeof(submitProcess));

//        // Serialized the input
//        char* buf;
//        ofstream ofs(buf);
//        boost::archive::text_oarchive oarchive(ofs);
//        oarchive << submitProcess->msg;

        int ret = socket_t::BroadcastMessage(&submitProcess->broadcastSocket, buf);

        if (ret != 1) {
            perror("broadcast submit process error\n");
        }

    }



    void ReceiveSubmitMessage(struct Process* receiveProcess) {
//        char recvBuffer[BUFFER_SIZE];
        printf("[ReceiveSubmitMessage] [%d] Start receiving msg...\n", receiveProcess->id);
        char *buf = NULL;
        buf = (char *)malloc (sizeof(char) * BUFFER_SIZE);
        memset(buf, 0, BUFFER_SIZE);

        /* The received message is stored in recvBuffer */
        socket_t::ReceiveMessage(&receiveProcess->serverSocket, buf);

//        struct message::SubmitMsg* recvMsg;
//        // Deserialize buf
//        ifstream ifs(buf);
//        boost::archive::text_iarchive iarchive(ifs);
//        iarchive >> recvMsg;

        struct message::SubmitMsg* recvMsg = (struct message::SubmitMsg*) buf;

        printf("[ReceiveSubmitMessage] [%d] put recvMsg into its queue\n", receiveProcess->id);
        receiveProcess->recvMsgQueue.push(*recvMsg);


//        struct message::SubmitMsg* recvMsg = (struct message::SubmitMsg *) buf;
//        int verify = SubmitMsgVerify(&receiveProcess->ac, recvMsg); // Question: this value will never be used?

    }

    int CheckRecvMsg(struct Process* p){
        int size = p->recvMsgQueue.size();
        printf("[CheckRecvMsg] [%d] Received queue size = %d\n",p->id, size);
        for (int i = 0; i < size; i++) {
            SubmitMsgVerify(&p->ac, &p->recvMsgQueue.front());
            p->recvMsgQueue.pop();
        }

        if (p->ac.from.size() >= NUMBER_OF_PROCESSES - NUMBER_OF_FAULTY_PROCESSES) {
            printf("[CheckRecvMsg] [%d] Receiving enough messages -> progress to Confirm phase\n", p->id);
            Confirm(p);
            return 1;
        } else {
            printf("[CheckRecvMsg] [%d] Not receiving enough messages\n", p->id);
            return 0;
        }

    }

    void BroadcastAggregateSignature(struct Process* submitProcess) {
        printf("[BroadcastAggSignature] [%d] broadcast AggSignature to everyone\n", submitProcess->id);

        char buf[sizeof(submitProcess->aggSignMsg)+1];
        memcpy(buf, &submitProcess->aggSignMsg, sizeof(submitProcess));

//        // Serialized the input
//        char* buf;
//        fstream ofs(buf);
//        boost::archive::text_oarchive oa(ofs);
//        oa << submitProcess->aggSignMsg;

        int ret = socket_t::BroadcastMessage(&submitProcess->broadcastSocket, buf);

        if (ret != 1) {
            perror("broadcast error\n");
        }
    }

    void ReceiveAggregateSignature(struct Process* receiveProcess) {
        printf("[ReceiveAggSignature] [%d] Start receiving aggSignature...\n", receiveProcess->id);

        char *buf = NULL;
        buf = (char *)malloc (sizeof(char) * BUFFER_SIZE);
        memset(buf, 0, BUFFER_SIZE);

        /* The received message is stored in recvBuffer */
        socket_t::ReceiveMessage(&receiveProcess->serverSocket, buf);

//        struct message::SubmitAggSignMsg* recvAggSignature;
//        // Deserialize buf
//        ifstream ifs(buf);
//        boost::archive::text_iarchive iarchive(ifs);
//        iarchive >> recvAggSignature;

        struct message::SubmitAggSignMsg* recvAggSignature = (struct message::SubmitAggSignMsg*) buf;

        printf("[ReceiveAggSignature] [%d] put recvAggSignature into its queue\n", receiveProcess->id);
        receiveProcess->recvAggSignQueue.push(*recvAggSignature);

//        struct message::SubmitAggSignMsg* tmpAggSignature = (message::SubmitAggSignMsg *) buf;
//        receiveProcess->ac.obtainedAggSignature.push_back(*recvAggSignature);
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
    void InitProcess(struct Process* p, int portNumber) {

        p->id = getpid();

        // Each process now has it own secret key and public key for enc/dec the message it commits
        accountable_confirmer_bls::Init();
        accountable_confirmer_bls::KeyGen(&p->keyPair);

        // Clear the receiving queue
        while (!p->recvMsgQueue.empty()) p->recvMsgQueue.pop();
        while (!p->recvAggSignQueue.empty()) p->recvAggSignQueue.pop();

        // Init socket
        // The portNumber is allocated by the caller
        socket_t::InitServerSocket(&p->serverSocket, portNumber);
        socket_t::InitBroadcastSocket(&p->broadcastSocket, portNumber);

        // Init the accountable confirmer
        InitAC(&p->ac);

        printf("[InitProcess] [%d] Init\n", p->id);
    }



    bool Submit(struct Process* p, int v) {
        printf("[Submit] [%d] submit value %d \n", p->id, v);
        struct message::SubmitMsg initMsg {
            .submitPid = p->id,
            .value = v,
            .pub = p->keyPair.pub,
        };

        message::InitSubmitMsg(&p->msg, &initMsg);

        p->ac.submitValue = v;

        // SharedSign on the submit value
        ShareSign(p);

        // Broadcast the signed submit message to everyone
        BroadcastSubmitMessage(p);

        return true;
    }

    void PseudoReceiveSubmitMessage(struct Process* receiveProcess, struct Process* submitProcess) {
        printf("[PseudoReceiveSubmitMessage] [%d] receives message from [%d]\n", receiveProcess->id, submitProcess->id);
        receiveProcess->ac.submitValue = submitProcess->msg.value;
        int verify = SubmitMsgVerify(&receiveProcess->ac, &submitProcess->msg); // Question: this value will never be used?
    }

    void Confirm(struct Process* p) {
        printf("[Confirm] [%d] Confirmed\n", p->id);
        p->ac.confirm = true;
        // Generate aggregate signature and store it in p
        GenerateAggSignature(p);

        // Broadcast the aggregate signature to everyone
        BroadcastAggregateSignature(p);
    }

    void PseudoReceiveAggSignature(struct Process* receiveProcess, struct Process* submitProcess) {
        printf("[PseudoReceiveAggSignature] [%d] receives AggSignature from [%d]\n", receiveProcess->id, submitProcess->id);
        // Verify the AggSignature
        // int verify = AggSignatureVerify(&receiveProcess->ac, submitProcess);
        receiveProcess->ac.obtainedAggSignature.push_back(submitProcess->aggSignMsg);
        printf("[PseudoReceiveAggSignature] [%d] number of obtainedAggSignature = %lu\n", receiveProcess->id, receiveProcess->ac.obtainedAggSignature.size());
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



}