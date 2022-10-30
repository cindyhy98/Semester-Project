/* Standard Library */
#include <unistd.h>

/* Internal Library */
#include "ac.h"

using namespace std;

#define NUMBER_OF_THREAD 4
#define NUMBER_OF_FAULTY 1

#define PROPOSE_VALUE_1 520
#define PROPOSE_VALUE_2 1314

#define MAX_DIGIT 15 //biggest int is 2147482647


namespace accountable_confirmer{

    struct AccountableConfirmer ac;
//    struct BestEffortBroadcast beb;
    struct Process p;
    struct message::MsgSubmit m;


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
        sprintf(valueToSign, "%d", p->msg.value);

        // now the signature for p->value is stored in Process P
        ac_bls::Sign(&p->msg.sigVal, &p->aggregateKey, valueToSign);
    }

    // [Done] Sign the whole message with PKI private key (secp256k1)
    void SubmitMsgSign(struct Process* p){
        // First, generate a SHA256 message hash for p->msg and store it in hash
        unsigned char msgHash[SHA256_DIGEST_LENGTH];
        message::GenerateMsgHash(&p->msg, msgHash);

        // sign the msg with its PKI key
        ecdsa_pki::Sign(&p->pkiKey, msgHash);
    }


    // TODO: What's the data structure?
    void InitAC(struct AccountableConfirmer* ac) {
        ac->msg.value = 0;
        ac->confirm = false;
        ac->from.clear();
        ac->lightCert.clear();
        ac->fullCert.clear();
        ac->obtainedLightCert.clear();
        ac->obtainedFullCert.clear();
    }


    bool Submit(struct AccountableConfirmer* ac, int v) {
        ac->msg.value = v;
        return true;
    }

    int Confirm(struct AccountableConfirmer* ac) {
        if (ac->from.size() >= NUMBER_OF_THREAD - NUMBER_OF_FAULTY){
            ac->confirm = true;
        }
        return ac->msg.value;
    }

    void Detect(struct AccountableConfirmer* ac) {

    }

//    void InitBEB (struct BestEffortBroadcast* beb){
//        beb->start = 0;
//
//    }
//
//    void Broadcast(struct BestEffortBroadcast* beb, struct BroadcastMsg BMsg) {
//        //
//    }
//
//    void Deliver(struct BestEffortBroadcast* beb, struct AccountableConfirmer* ac) {
//
//    }


}