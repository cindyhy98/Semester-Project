#ifndef ACCOUNTABLE_CONFIRMER_MESSAGE_H
#define ACCOUNTABLE_CONFIRMER_MESSAGE_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>


/* Internal Library*/
#include "ac_bls.h"
#include "ecdsa_pki.h"

using namespace std;
namespace message {

    struct MsgLightCert {
        int value;           // Submitted value
        blsSignature aggSig; // aggregate signature for the value
    };

    struct MsgSubmit {
        int value;           // Value to submit
        blsSignature sig;    // Signature for the value
    };

    struct MsgToSend {
        MsgSubmit msgSubmit;
        unsigned char msgHash[SHA256_DIGEST_LENGTH];
        secp256k1_ecdsa_signature sig;  //signature for the whole MsgSubmit
    };


// Generate MsgHash is the input of PKI signing
    void GenerateMsgHash(struct MsgSubmit* m, unsigned char* hash);
}



#endif //ACCOUNTABLE_CONFIRMER_MESSAGE_H
