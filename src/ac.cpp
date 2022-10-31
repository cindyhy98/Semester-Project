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


namespace accountable_confirmer{

    struct AccountableConfirmer ac;
//    struct BestEffortBroadcast beb;
    struct Process p;

    int FindPidInVecotr(vector<pid_t> vectorPID, pid_t elem){
        if (find(vectorPID.begin(), vectorPID.end(), elem) != vectorPID.end()){
            printf("pid(%d) is ALREADY in the vector\n", elem);
            return 1;
        } else {
            printf("pid(%d) is NOT YET in the vector\n", elem);
            return 0;
        }
    }


    // [Done]
    void InitProcess(struct Process* p){
        p->id = getpid();

        // each process now has it own secret key and public key for enc/dec the message it commits
        ac_bls::Init();
        ac_bls::KeyGen(&p->aggregateKey);

        //each process also has a pair of PKI secret key and PKI public key
        ecdsa_pki::Init(&p->pkiKey);
        ecdsa_pki::KeyGen(&p->pkiKey);
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

        /* Should init here?*/
        for(int i = 0; i < NUMBER_OF_PROCESS; i++){
            InitProcess(&ac->p[i]);
        }

//        ac->sigVec.clear();
//        ac->pubVec.clear();
    }

    int SubmitMsgVerify(struct AccountableConfirmer* ac, struct Process* recvP){
        // recvP is one of the process that is stored under ac

        // TODO: Broadcast() function
        // In Broadcast(), the message will be sent from the submit process to everyone (which records in ac->p)
        // Broadcast() will call SubmitMsgVerify


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
            return 1;
        }
    }



    bool Submit(struct AccountableConfirmer* ac, int v) {
        ac->valueSubmit = v;
        return true;
    }

    int Confirm(struct AccountableConfirmer* ac) {
        if (ac->from.size() >= NUMBER_OF_THREAD - NUMBER_OF_FAULTY){
            ac->confirm = true;
        }

        // TODO: Broadcast LightCert

        return ac->valueSubmit;
    }

    void CombinedLightCert(struct AccountableConfirmer* ac){
        // Combine the received partial signatures into light certificate (==aggregate signature)
        int submitValue;
        vector<blsSignature> sigVec;

        for(auto & elem : ac->lightCert){
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
        ac->obtainedLightCert.push_back(mlc);
    }

    void DetectConflictLightCert(struct AccountableConfirmer* ac){
        message::MsgLightCert tmp = ac->obtainedLightCert.front();
        for(vector<message::MsgLightCert>::size_type i = 1; i != ac->obtainedLightCert.size(); i++) {
            // conflicting lightcert
            if(ac->obtainedLightCert[i].value != tmp.value && ac->confirm){
                // TODO: Broadcast FullCert

            }
        }

    }

    void DetectConflictFullCert(struct AccountableConfirmer* ac){
        message::MsgToSend tmp = ac->obtainedFullCert.front();
        for(vector<message::MsgToSend>::size_type i = 1; i != ac->obtainedFullCert.size(); i++) {
            // conflicting lightcert
            if(ac->obtainedFullCert[i].msgHash != tmp.msgHash){
                // TODO: call Detect
            }
        }
    }



    void Detect(struct AccountableConfirmer* ac) {

    }

    /*
     * void Broadcast(struct AccountableConfirmer* ac, struct Process* submitProcess){
     *      // a process is trying to send its value to everyone
     *      1. find a NULL Process in ac->p[i]
     *      2. store the info of submitProcess into ac->p[i] (here it stores the unverified process info)
     *      3. call SubmitMsgVerify(struct AccountableConfirmer* ac, struct Process* ac->p[i]) to check if everything is verified
     *      4. if not Verify (SubmitMsgVerify == 1) -> clear ac->p[i] (don't store it)
     * }
     */

}