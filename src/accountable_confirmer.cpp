/* Standard Library */
#include <unistd.h>

/* Internal Library */
#include "accountable_confirmer.h"

#define MAX_DIGIT 15 // biggest int is 2147482647
#define BUFFER_SIZE 4096

using namespace std;

namespace accountable_confirmer {

    struct Process P[NUMBER_OF_PROCESSES];

    /* Return 1 if found, Return 0 if not found */
    int FindPidInVector(vector<int> vectorPID, int elem){
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
        printf("[ShareSign] Create a partial signature for the submit value %d\n", p->msg.value);

    }

    /* Return 1 if it is valid, Return 0 if not valid */
    int ShareVerify(struct Process* p) {
        char valueSigned[MAX_DIGIT + sizeof(char)];
        sprintf(valueSigned, "%d", p->msg.value);

        if (accountable_confirmer_bls::Verify(&p->keyPair, p->msg.sig, valueSigned)) {
            printf("[ShareVerify] Successful\n");
            return 1;
        } else {
            printf("[ShareVerify] Fail\n");
            return 0;
        }

    }

    /* Return 1 if verified, Return 0 if not verified */
    int SubmitMsgVerify(struct AccountableConfirmer* ac, struct Process* recvP) {

        // Authenticate the whole submit msg (p->msg)
        if (ShareVerify(recvP) && (ac->submitValue == recvP->msg.value) && !FindPidInVector(ac->from, recvP->id)) {
            printf("[SubmitMsgVerify] Store the received message in the accountable confirmer\n");
            ac->from.push_back(recvP->id);
            ac->partialSignature.push_back(recvP->msg);
            return 1;
        } else {
            return 0;
        }

    }

    /* Return 1 if verified, Return 0 if not verified */
    int AggregateSignatureVerify(struct AccountableConfirmer* ac, struct Process* recvP) {
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

        p->mlc = {.value = submitValue, .aggSig = aggSig};
    }



    void BroadcastSubmitProcess(struct Process* submitProcess) {
        printf("[BroadcastSubmitProcess] [%d] broadcast value %d to everyone\n",submitProcess->id, submitProcess->msg.value);

        // Convert struct Process to char array
        // TODO: need to use serialized library
        char mChar[sizeof(submitProcess)+1];
        memcpy(mChar, &submitProcess, sizeof(submitProcess));

        int ret = socket_t::SendBroadcastMessage(&submitProcess->broadcastSocket, mChar);

        if (ret != 1) {
            perror("broadcast submit process error\n");
        }

    }

    void ReceiveSubmitProcess(struct Process* receiveProcess) {
//        char recvBuffer[BUFFER_SIZE];
        printf("[ReceiveSubmitProcess] [%d] start receiving msg...\n", receiveProcess->id);
        char *buf = NULL;
        buf = (char *)malloc (sizeof(char) * BUFFER_SIZE);
        memset(buf, 0, BUFFER_SIZE);

        /* The received message is stored in recvBuffer */
        socket_t::ReceiveMessageFromOthers(&receiveProcess->serverSocket, buf);

        struct Process* tmpProcess = (Process *) buf;
        int verify = SubmitMsgVerify(&receiveProcess->ac, tmpProcess); // Question: this value will never be used?

    }

    void BroadcastAggSignature(struct message::SubmitAggSignMsg* mlc, struct Process* submitProcess) {
        printf("[BroadcastAggSignature] [%d] broadcast AggSignature to everyone\n", submitProcess->id);
        // Convert struct Process to char array
        char mChar[sizeof(mlc)+1];
        memcpy(mChar, &mlc, sizeof(mlc));
        int ret = socket_t::SendBroadcastMessage(&submitProcess->broadcastSocket, mChar);

        if (ret != 1) {
            perror("broadcast error\n");
        }
    }

    void ReceiveAggSignature(struct Process* p) {
        char *buf = NULL;
        buf = (char *)malloc (sizeof(char) * BUFFER_SIZE);
        memset(buf, 0, BUFFER_SIZE);

        /* The received message is stored in recvBuffer */
        socket_t::ReceiveMessageFromOthers(&p->serverSocket, buf);

        struct message::SubmitAggSignMsg* tmpAggSignature = (message::SubmitAggSignMsg *) buf;
        p->ac.obtainedAggSignature.push_back(*tmpAggSignature);
    }

    /* Main functionality */
    void InitProcess(struct Process* p, int portNumber) {
        p->id = portNumber;

        // Each process now has it own secret key and public key for enc/dec the message it commits
        accountable_confirmer_bls::Init();
        accountable_confirmer_bls::KeyGen(&p->keyPair);

        // Init socket
        // The portNumber is allocated by the caller
        socket_t::InitServerSocket(&p->serverSocket, portNumber);
        socket_t::InitBroadcastSocket(&p->broadcastSocket, portNumber);

        // Init the accountable confirmer
        InitAC(&p->ac);

        printf("[InitProcess] PID = %d\n", p->id);
    }

    bool Submit(struct Process* p, int v) {
        p->msg.value = v;
        p->msg.pub = p->keyPair.pub;
        p->ac.submitValue = v;

        // SharedSign on the submit value
        ShareSign(p);

        // Broadcast the signed submit message to everyone
        BroadcastSubmitProcess(p);

        return true;
    }

    void PseudoReceiveSubmitProcess(struct Process* receiveProcess, struct Process* submitProcess) {
        printf("[PseudoReceiveSubmitProcess] [%d] receives message from [%d]\n", receiveProcess->id, submitProcess->id);
        receiveProcess->ac.submitValue = submitProcess->msg.value;
        int verify = SubmitMsgVerify(&receiveProcess->ac, submitProcess); // Question: this value will never be used?
    }

    int Confirm(struct Process* p) {
        if (p->ac.from.size() >= NUMBER_OF_PROCESSES - NUMBER_OF_FAULTY_PROCESSES){
            printf("[Confirm] [%d] Confirm\n", p->id);
            p->ac.confirm = true;

            // Generate aggregate signature and store it in p
            GenerateAggSignature(p);

            // Broadcast the aggregate signature to everyone
            BroadcastAggSignature(&p->mlc, p);
            return 1;
        } else {
            printf("[Confirm] [%d] Not Yet confirm\n", p->id);
            return 0;
        }

    }

    void PseudoReceiveAggSignature(struct Process* receiveProcess, struct Process* submitProcess) {
        printf("[PseudoReceiveAggSignature] [%d] receives AggSignature from [%d]\n", receiveProcess->id, submitProcess->id);
        // Verify the AggSignature
        // int verify = AggSignatureVerify(&receiveProcess->ac, submitProcess);
        receiveProcess->ac.obtainedAggSignature.push_back(submitProcess->mlc);
        printf("[PseudoReceiveAggSignature] [%d] number of obtainedAggSignature = %lu\n", receiveProcess->id, receiveProcess->ac.obtainedAggSignature.size());
    }

    bool DetectConflictAggSignature(struct Process* p) {
        bool detect = false;
        if(p->ac.obtainedAggSignature.size() <= 1){
            // No need to compare
            return detect;
        }

        message::SubmitAggSignMsg tmp = p->ac.obtainedAggSignature.front();


        for(vector<message::SubmitAggSignMsg>::size_type i = 1; i != p->ac.obtainedAggSignature.size(); i++) {
            printf("[DetectConflictAggSignature] Detecting....\n");

            if(!(p->ac.obtainedAggSignature[i] == tmp) && p->ac.confirm){
                // TODO: How to represent the proof?
                printf("[DetectConflictAggSignature] first value = %d, second value = %d\n",tmp.value ,p->ac.obtainedAggSignature[i].value);
                printf("[DetectConflictAggSignature] Reach the proof\n");
                detect = true;
                break;
            }
        }

        return detect;


    }

}