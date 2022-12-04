#ifndef ACCOUNTABLE_CONFIRMER_MESSAGE_H
#define ACCOUNTABLE_CONFIRMER_MESSAGE_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/* Internal Library*/
#include "accountable_confirmer_bls.h"


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
        int submitPid;            // id for the submit process
        int value;                // submit value
        blsSignature sig;         // Signature for the value
        blsPublicKey pub;    // Publickey for the signature
    };

}



#endif //ACCOUNTABLE_CONFIRMER_MESSAGE_H
