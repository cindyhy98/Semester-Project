#ifndef ACCOUNTABLE_CONFIRMER_MESSAGE_H
#define ACCOUNTABLE_CONFIRMER_MESSAGE_H

#include <string>
#include <iostream>


/* Internal Library*/
#include "accountable_confirmer_bls.h"


using namespace std;
namespace message {
    /* Use Enum for the message type */
    enum messageType {none, send, echo, ready, submit, aggSign};

    /* For Reliable Broadcast */
    struct RBMessage {
        int type;
        int pid;
        string content;
    };

    struct SubmitAggSign {
        int type;
        int pid;             // id for the submit process
        int value;           // submit value
        blsSignature aggSig; // Aggregate signature for the value
//        vector<blsPublicKey> pubKeyVector; // all public keys of this signature

        bool operator==( const SubmitAggSign &other){
            return value == other.value;
        }

    };

    struct SubmitMsg {
        int type;
        int pid;            // id for the submit process
        int value;          // submit value
        blsSignature sig;   // Signature for the value
        blsPublicKey pub;    // Publickey for the signature
    };

}



#endif //ACCOUNTABLE_CONFIRMER_MESSAGE_H
