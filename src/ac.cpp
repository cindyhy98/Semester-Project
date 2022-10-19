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


    // [Done]
    void InitProcess(struct Process* p){
        p->id = getpid();

        // TODO: How to choose propose value ??
        p->value = PROPOSE_VALUE_1;

        // each process now has it own secret key and public key
        testbls::KeyGen(&p->sec, &p->pub);
    }

    // [Done]
    void ShareSign(struct Process* p){
        char msg[MAX_DIGIT + sizeof(char)];
        sprintf(msg, "%d", p->value);

        // now the signature for p->value is stored in Process P
        testbls::Sign(&p->sig, &p->sec, msg);
    }

    // [Done]
    void CreateSubmitMsg(struct Process* p){
        p->msg.value = p->value;
        p->msg.sig = p->sig;

        
    }


    // TODO: What's the data structure?
    void InitAC(struct AccountableConfirmer* ac) {
        ac->value = 0;
        ac->confirm = false;
        ac->from.clear();
        ac->lightCert.clear();
        ac->fullCert.clear();
        ac->obtainedLightCert.clear();
        ac->obtainedFullCert.clear();
    }


    bool Submit(struct AccountableConfirmer* ac, int v) {
        ac->value = v;
        return true;
    }

    int Confirm(struct AccountableConfirmer* ac) {
        if (ac->from.size() >= NUMBER_OF_THREAD - NUMBER_OF_FAULTY){
            ac->confirm = true;
        }
        return ac->value;
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