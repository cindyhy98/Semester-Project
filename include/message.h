#ifndef ACCOUNTABLE_CONFIRMER_MESSAGE_H
#define ACCOUNTABLE_CONFIRMER_MESSAGE_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>


/* Internal Library*/
#include "accountable_confirmer_bls.h"
#include "ecdsa_pki.h"

using namespace std;
namespace message {

    struct SubmitAggSignMsg {
        int value;           // submit value
        blsSignature aggSig; // Aggregate signature for the value
//        vector<blsPublicKey> pubKeyVector; // all public keys of this signature

        bool operator==( const SubmitAggSignMsg &other){
            return value == other.value;
//            aggSig.v.x.d == other.aggSig.v.x.d &&
//            aggSig.v.y.d == other.aggSig.v.y.d &&
//            aggSig.v.z.d == other.aggSig.v.z.d;
        }
    };

    struct SubmitMsg {
        int value;           // submit value
        blsSignature sig;    // Signature for the value
        blsPublicKey pub;    // Public key for the signature
    };

    struct MsgToSend {
        SubmitMsg msgSubmit;
        unsigned char msgHash[SHA256_DIGEST_LENGTH];
        secp256k1_ecdsa_signature sig;  //signature for the whole SubmitMsg
    };


// Generate MsgHash is the input of PKI signing
    void GenerateMsgHash(struct SubmitMsg* m, unsigned char* hash);
}



#endif //ACCOUNTABLE_CONFIRMER_MESSAGE_H
