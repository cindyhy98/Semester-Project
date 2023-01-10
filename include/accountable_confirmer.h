#ifndef ACCOUNTABLE_CONFIRMER_AC_H
#define ACCOUNTABLE_CONFIRMER_AC_H

/* Standard Library */
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <thread>

/* External Library */
#include <openssl/sha.h>


/* Internal Library*/
#include "accountable_confirmer_bls.h"
#include "message.h"
#include "tcp_connection.h"
#include "tcp_server.h"
#include "tcp_client.h"

using namespace std;

class AccountableConfirmer {
public:
    AccountableConfirmer() : value(0), confirm(false) {
        from.clear();
        partialSignature.clear();
        obtainedAggSignature.clear();
    }

public:
    int value;
    bool confirm;
    vector<int> from;
    vector<message::SubmitMsg> partialSignature;
    vector<message::SubmitAggSign> obtainedAggSignature;
};

#endif //ACCOUNTABLE_CONFIRMER_AC_H
