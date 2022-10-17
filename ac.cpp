#include <stdio.h>
#include <stdlib.h>

#include <cassert>
#include <set>
#include <string>
#include <iostream>

#include "ac.h"


using namespace std;

#define NUMBER_OF_THREAD 4
#define NUMBER_OF_FAULTY 1


namespace accountable_confirmer{
    
    struct AccountableConfirmer ac;
    struct BestEffortBroadcast beb;

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

    void InitBEB (struct BestEffortBroadcast* beb){
        beb->start = 0;
    
    }

    void Broadcast(struct BestEffortBroadcast* beb, struct BroadcastMsg BMsg) {
        // 
    }
    
    void Deliver(struct BestEffortBroadcast* beb, struct AccountableConfirmer* ac) {
        if 
    }


}