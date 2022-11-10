/* Standard Library */
#include <unistd.h>

/* Internal Library */
#include "ac.h"

using namespace std;

#define NUMBER_OF_THREAD 4
#define NUMBER_OF_FAULTY 1

#define PROPOSE_VALUE_1 111
#define PROPOSE_VALUE_2 222

#define MAX_DIGIT 15 //biggest int is 2147482647
#define PORT 9999

#define BUFFER_SIZE 4096

namespace accountable_confirmer{

    struct AccountableConfirmer ac;
    struct Process p;

    int FindPidInVector(vector<pid_t> vectorPID, pid_t elem){
        vector<pid_t>::iterator it = find(vectorPID.begin(), vectorPID.end(), elem);
        if (it != vectorPID.end()){
            printf("pid(%d) is ALREADY in the vector\n", elem);
            return it - vectorPID.begin();
        } else {
            printf("pid(%d) is NOT YET in the vector\n", elem);
            return -1;
        }
    }


    void InitProcess(struct Process* p){
        p->id = getpid();

        // each process now has it own secret key and public key for enc/dec the message it commits
        ac_bls::Init();
        ac_bls::KeyGen(&p->aggregateKey);

        // each process also has a pair of PKI secret key and PKI public key
        ecdsa_pki::Init(&p->pkiKey);
        ecdsa_pki::KeyGen(&p->pkiKey);

        // init socket
        // QUESTION: how to give the port number?
        socket_t::InitServerSocket(&p->serverSocket, PORT);
        socket_t::InitBroadcastSocket(&p->broadcastSocket, PORT);

        InitAC(&p->ac);
    }

    // [Done]
    void ShareSign(struct Process* p){
        char valueToSign[MAX_DIGIT + sizeof(char)];
        sprintf(valueToSign, "%d", p->msg.msgSubmit.value);

        // now the signature for p->value is stored in Process P
        ac_bls::Sign(&p->aggregateKey, &p->msg.msgSubmit.sig, valueToSign);
    }

    int ShareVerify(struct Process* p){
        char valueSigned[MAX_DIGIT + sizeof(char)];
        sprintf(valueSigned, "%d", p->msg.msgSubmit.value);

        /* return 1 if it is valid else 0 */
        if(ac_bls::Verify(&p->aggregateKey, p->msg.msgSubmit.sig, valueSigned)){
            printf("ShareVerify Successful\n");
            return 1;
        }else{
            printf("ShareVerify Fail\n");
            return 0;
        }

    }

    // [Done] Sign the whole message with PKI private key (secp256k1)
    void SubmitMsgSign(struct Process* p){
        // First, generate a SHA256 message hash for p->msg and store it in hash
        message::GenerateMsgHash(&p->msg.msgSubmit, p->msg.msgHash);

        // sign the msg with its PKI key
        ecdsa_pki::Sign(&p->pkiKey, &p->msg.sig, p->msg.msgHash);
    }


    // TODO: What's the data structure?
    void InitAC(struct AccountableConfirmer* ac) {
        ac->valueSubmit = 0;
        ac->confirm = false;
        ac->from.clear();
        ac->lightCert.clear();
        ac->fullCert.clear();
        ac->obtainedLightCert.clear();
        ac->obtainedFullCert.clear();
    }

    /* Return 0 if verified, Return -1 if not verified */
    int SubmitMsgVerify(struct AccountableConfirmer* ac, struct Process* recvP){
        // Authenticate the submitted msg

        int resVerify = ecdsa_pki::Verify(&recvP->pkiKey, &recvP->msg.sig, recvP->msg.msgHash);
        if (resVerify != 1){
            // Not valid -> need some error handling
            printf("This message is not unauthenticated!\n");
            return 0;
        }else{
            // Valid message -> ShareVerify, valueRecv == valueSubmit, this process never submit already
            if(ShareVerify(recvP) && (ac->valueSubmit == recvP->msg.msgSubmit.value) && !FindPidInVector(ac->from, recvP->id)){
                ac->from.push_back(recvP->id);
                ac->lightCert.push_back(recvP->msg.msgSubmit);
                ac->fullCert.push_back(recvP->msg);
                return 0;
            }
            return -1;
        }
    }


    bool Submit(struct AccountableConfirmer* ac, int v) {
        ac->valueSubmit = v;
        return true;
    }

    void Confirm(struct Process* p) {
        if (p->ac.from.size() >= NUMBER_OF_THREAD - NUMBER_OF_FAULTY){
            p->ac.confirm = true;
        }

        // Generate aggregate signature (CombinedLightCert)
        CombinedLightCert(p);

    }

    /* Combine the received partial signatures into light certificate (==aggregate signature)  */
    void CombinedLightCert(struct Process* p){
        int submitValue;
        vector<blsSignature> sigVec;

        for(auto & elem : p->ac.lightCert){
            sigVec.push_back(elem.sig);
            submitValue = elem.value;
        }

        char valueToSign[MAX_DIGIT + sizeof(char)];
        sprintf(valueToSign, "%d", submitValue);
        const size_t msgSize = strlen(valueToSign);

        blsSignature aggSig;
        // The aggregate signature is stored in ac
        ac_bls::AggSign(&aggSig, &sigVec[0], msgSize);


        struct message::MsgLightCert mlc = {.value = submitValue, .aggSig = aggSig};

        // Broadcast this light cert for everyone
        BroadcastLightCert(&mlc, p);
    }

    void DetectConflictLightCert(struct AccountableConfirmer* ac){
        message::MsgLightCert tmp = ac->obtainedLightCert.front();
        for(vector<message::MsgLightCert>::size_type i = 1; i != ac->obtainedLightCert.size(); i++) {
            // find conflicting lightcert
            if(ac->obtainedLightCert[i].value != tmp.value && ac->confirm){
                // TODO: We reach the end of the proof!!

            }
        }

    }

    /* Question: This function is useless now since we are using aggregate signature? */
//    void DetectConflictFullCert(struct AccountableConfirmer* ac){
//        message::MsgToSend tmp = ac->obtainedFullCert.front();
//        for(vector<message::MsgToSend>::size_type i = 1; i != ac->obtainedFullCert.size(); i++) {
//            // conflicting lightcert
//            if(ac->obtainedFullCert[i].msgHash != tmp.msgHash){
//                // call Detect
//            }
//        }
//    }


    void BroadcastSubmitProcess(struct Process* submitProcess){

        // Convert struct Process to char array
        char mChar[sizeof(submitProcess)+1];
        memcpy(mChar, &submitProcess, sizeof(submitProcess));
        int ret = socket_t::SendBroadcastMessage(&submitProcess->broadcastSocket, mChar);

        if (ret != 1) {
            perror("broadcast submit process error\n");
        }

    }

    void ReceiveSubmitProcess(struct AccountableConfirmer* ac, struct Process* submitProcess){
//        char recvBuffer[BUFFER_SIZE];

        char *buf = NULL;
        buf = (char *)malloc (sizeof(char) * BUFFER_SIZE);
        memset(buf, 0, BUFFER_SIZE);

        /* The received message is stored in recvBuffer */
        socket_t::ReceiveMessageFromOthers(&submitProcess->serverSocket, buf);

        struct Process* tmpProcess = (Process *) buf;
        int verify = SubmitMsgVerify(ac, tmpProcess); // Question: this value will never be used?

    }

    void BroadcastLightCert(struct message::MsgLightCert* mlc, struct Process* p){
        // Convert struct Process to char array
        char mChar[sizeof(mlc)+1];
        memcpy(mChar, &mlc, sizeof(mlc));
        int ret = socket_t::SendBroadcastMessage(&p->broadcastSocket, mChar);

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




}