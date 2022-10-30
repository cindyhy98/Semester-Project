#ifndef ACCOUNTABLE_CONFIRMER_MESSAGE_H
#define ACCOUNTABLE_CONFIRMER_MESSAGE_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>


/* Internal Library*/
#include "ac_bls.h"

using namespace std;
namespace message {
    struct MsgSubmit {
        int value;              // Value to submit
        blsSignature sigVal;    // Signature for the value
    };

// Generate MsgHash is the input of PKI signing
    void GenerateMsgHash(struct MsgSubmit* m, unsigned char* hash);
}



#endif //ACCOUNTABLE_CONFIRMER_MESSAGE_H
