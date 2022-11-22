/* Standard Library */
#include <unistd.h>

/* Internal Library */
#include "accountable_confirmer.h"

using namespace std;



#define MAX_DIGIT 15 // biggest int is 2147482647
#define BUFFER_SIZE 4096

namespace accountable_confirmer {

    struct Process P[NUMBER_OF_PROCESSES];

    int FindPidInVector(vector<int> vectorPID, int elem){
        vector<int>::iterator it = find(vectorPID.begin(), vectorPID.end(), elem);
        if (it != vectorPID.end()){
//            printf("pid(%d) is ALREADY in the vector\n", elem);
//            return it - vectorPID.begin();
            return 1;
        } else {
//            printf("pid(%d) is NOT YET in the vector\n", elem);
            return 0;
        }
    }

    void InitAC(struct AccountableConfirmer* ac) {
        ac->valueSubmit = 0;
        ac->confirm = false;
        ac->from.clear();
        ac->lightCert.clear();
        ac->fullCert.clear();
        ac->obtainedLightCert.clear();
        ac->obtainedFullCert.clear();
    }

    void ShareSign(struct Process* p){
        char valueToSign[MAX_DIGIT + sizeof(char)];
        sprintf(valueToSign, "%d", p->msg.msgSubmit.value);

        // now the signature for p->value is stored in Process P
        accountable_confirmer_bls::Sign(&p->aggregateKey, &p->msg.msgSubmit.sig, valueToSign);
        printf("[ShareSign] Create a partial signature for the submit value %d\n", p->msg.msgSubmit.value);

    }

    void SubmitMsgSign(struct Process* p){
        // First, generate a SHA256 message hash for p->msg and store it in hash
        message::GenerateMsgHash(&p->msg.msgSubmit, p->msg.msgHash);

        // sign the msg with its PKI key
        ecdsa_pki::Sign(&p->pkiKey, &p->msg.sig, p->msg.msgHash);
        printf("[SubmitMsgSign] Sign the whole submit message with PKI key\n");
    }

    int ShareVerify(struct Process* p){
        char valueSigned[MAX_DIGIT + sizeof(char)];
        sprintf(valueSigned, "%d", p->msg.msgSubmit.value);

        /* return 1 if it is valid else 0 */
        if(accountable_confirmer_bls::Verify(&p->aggregateKey, p->msg.msgSubmit.sig, valueSigned)){
            printf("[ShareVerify] Successful\n");
            return 1;
        }else{
            printf("[ShareVerify] Fail\n");
            return 0;
        }

    }

    /* Return 0 if verified, Return -1 if not verified */
    int SubmitMsgVerify(struct AccountableConfirmer* ac, struct Process* recvP){
        // Authenticate the submitted msg

        int resVerify = ecdsa_pki::Verify(&recvP->pkiKey, &recvP->msg.sig, recvP->msg.msgHash);
        if (resVerify != 1){
            // Not valid -> need some error handling
            printf("[SubmitMsgVerify] This message is not authenticated!\n");
            return 0;
        }else{
            // Valid message -> ShareVerify, valueRecv == valueSubmit, this process never be submitted already
            printf("[SubmitMsgVerify] This message is authenticated!\n");

            if(ShareVerify(recvP) && (ac->valueSubmit == recvP->msg.msgSubmit.value) && !FindPidInVector(ac->from, recvP->id)){
                printf("[SubmitMsgVerify] Store the received message in the accountable confirmer\n");
                ac->from.push_back(recvP->id);
                ac->lightCert.push_back(recvP->msg.msgSubmit);
                ac->fullCert.push_back(recvP->msg);
                return 0;
            }
            return -1;
        }
    }

    /* Return 0 if verified, Return -1 if not verified */
    int LightCertVerify(struct AccountableConfirmer* ac, struct Process* recvP){
        // TODO: use FastAggSignVerify in accountable_confirmer::_blscpp
        return 0;
    }



    /* Combine the received partial signatures into light certificate (==aggregate signature)  */
    void CombinedLightCert(struct Process* p){
        int submitValue;
        vector<blsSignature> sigVec;
        vector<blsPublicKey> pubVec;

        for(auto & elem : p->ac.lightCert){
            sigVec.push_back(elem.sig);
            pubVec.push_back(elem.pub);
            submitValue = elem.value;
        }

        char valueToSign[MAX_DIGIT + sizeof(char)];
        sprintf(valueToSign, "%d", submitValue);
        const size_t msgSize = strlen(valueToSign);

        blsSignature aggSig;
        // The aggregate signature is stored in ac
        accountable_confirmer_bls::AggSign(&aggSig, &sigVec[0], msgSize);

//        struct message::MsgLightCert mlc = {.value = submitValue, .aggSig = aggSig};
//        p->mlc = {.value = submitValue, .aggSig = aggSig, .pubKeyVector = pubVec};
        p->mlc = {.value = submitValue, .aggSig = aggSig};


    }



    void BroadcastSubmitProcess(struct Process* submitProcess){
        printf("[BroadcastSubmitProcess] [%d] broadcast value %d to everyone\n",submitProcess->id, submitProcess->msg.msgSubmit.value);
        // Convert struct Process to char array
        char mChar[sizeof(submitProcess)+1];
        memcpy(mChar, &submitProcess, sizeof(submitProcess));
        int ret = socket_t::SendBroadcastMessage(&submitProcess->broadcastSocket, mChar);

        if (ret != 1) {
            perror("broadcast submit process error\n");
        }

    }

    // TODO: How to make it non-blocking?
    void ReceiveSubmitProcess(struct Process* receiveProcess){
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

    void BroadcastLightCert(struct message::MsgLightCert* mlc, struct Process* submitProcess){
        printf("[BroadcastLightCert] [%d] broadcast LightCert to everyone\n", submitProcess->id);
        // Convert struct Process to char array
        char mChar[sizeof(mlc)+1];
        memcpy(mChar, &mlc, sizeof(mlc));
        int ret = socket_t::SendBroadcastMessage(&submitProcess->broadcastSocket, mChar);

        if (ret != 1) {
            perror("broadcast error\n");
        }
    }

    void ReceiveLightCert(struct Process* p){
        char *buf = NULL;
        buf = (char *)malloc (sizeof(char) * BUFFER_SIZE);
        memset(buf, 0, BUFFER_SIZE);

        /* The received message is stored in recvBuffer */
        socket_t::ReceiveMessageFromOthers(&p->serverSocket, buf);

        struct message::MsgLightCert* tmpLightCert = (message::MsgLightCert *) buf;
        p->ac.obtainedLightCert.push_back(*tmpLightCert);
    }

    /* Main functionality */
    void InitProcess(struct Process* p, int portNumber){
        p->id = portNumber;

        // each process now has it own secret key and public key for enc/dec the message it commits
        accountable_confirmer_bls::Init();
        accountable_confirmer_bls::KeyGen(&p->aggregateKey);

        // each process also has a pair of PKI secret key and PKI public key
        ecdsa_pki::Init(&p->pkiKey);
        ecdsa_pki::KeyGen(&p->pkiKey);

        // init socket
        // The portNumber is allocated by the caller
        socket_t::InitServerSocket(&p->serverSocket, portNumber);
        socket_t::InitBroadcastSocket(&p->broadcastSocket, portNumber);

        InitAC(&p->ac);

        printf("[InitProcess] PID = %d\n", p->id);
    }

    bool Submit(struct Process* p, int v) {
        p->msg.msgSubmit.value = v;
        p->msg.msgSubmit.pub = p->aggregateKey.pub;
        p->ac.valueSubmit = v;

        // SharedSign on the submit value
        ShareSign(p);

        // After SharedSign, sign the whole submitted message
        SubmitMsgSign(p);

        // Broadcast the signed submit message to everyone
        BroadcastSubmitProcess(p);

        return true;
    }

    void PseudoReceiveSubmitProcess(struct Process* receiveProcess, struct Process* submitProcess){
        printf("[PseudoReceiveSubmitProcess] [%d] receives message from [%d]\n", receiveProcess->id, submitProcess->id);
        receiveProcess->ac.valueSubmit = submitProcess->msg.msgSubmit.value;
        int verify = SubmitMsgVerify(&receiveProcess->ac, submitProcess); // Question: this value will never be used?
    }

    int Confirm(struct Process* p) {
        if (p->ac.from.size() >= NUMBER_OF_PROCESSES - NUMBER_OF_FAULTY_PROCESSES){
            printf("[Confirm] [%d] Confirm\n", p->id);
            p->ac.confirm = true;

            // Generate aggregate signature and store it in p
            CombinedLightCert(p);

            // Broadcast the light cert to everyone
            BroadcastLightCert(&p->mlc, p);
            return 0;
        } else {
            printf("[Confirm] [%d] Not Yet confirm\n", p->id);
            return 1;
        }

    }

    void PseudoReceiveLightCert( struct Process* receiveProcess, struct Process* submitProcess){
        printf("[PseudoReceiveLightCert] [%d] receives LightCert from [%d]\n", receiveProcess->id, submitProcess->id);
        // Verify the LightCert
        // int verify = LightCertVerify(&receiveProcess->ac, submitProcess);
        receiveProcess->ac.obtainedLightCert.push_back(submitProcess->mlc);
        printf("[PseudoReceiveLightCert] [%d] number of obtainedLightCert = %lu\n", receiveProcess->id, receiveProcess->ac.obtainedLightCert.size());
    }

    bool DetectConflictLightCert(struct Process* p){
        bool detect = false;
        if(p->ac.obtainedLightCert.size() <= 1){
            // No need to compare
            return detect;
        }

        message::MsgLightCert tmp = p->ac.obtainedLightCert.front();


        for(vector<message::MsgLightCert>::size_type i = 1; i != p->ac.obtainedLightCert.size(); i++) {
            printf("[DetectConflictLightCert] Detecting....\n");

            if(!(p->ac.obtainedLightCert[i] == tmp) && p->ac.confirm){
                // TODO: How to represent the proof?
                printf("[DetectConflictLightCert] first value = %d, second value = %d\n",tmp.value ,p->ac.obtainedLightCert[i].value);
                printf("[DetectConflictLightCert] Reach the proof\n");
                detect = true;
                break;
            }
        }

        return detect;


    }

}