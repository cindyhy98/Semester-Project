#ifndef ACCOUNTABLE_CONFIRMER_MESSAGE_H
#define ACCOUNTABLE_CONFIRMER_MESSAGE_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>


/* Internal Library*/
#include "accountable_confirmer_bls.h"


using namespace std;
namespace message {

    struct SubmitAggSignMsg {
        int value;           // submit value
        accountable_confirmer_bls::Signature aggSig; // Aggregate signature for the value
//        vector<blsPublicKey> pubKeyVector; // all public keys of this signature

        bool operator==( const SubmitAggSignMsg &other){
            return value == other.value;
//            aggSig.v.x.d == other.aggSig.v.x.d &&
//            aggSig.v.y.d == other.aggSig.v.y.d &&
//            aggSig.v.z.d == other.aggSig.v.z.d;
        }

        template<class Archive>
        void serialize(Archive& ar, const unsigned version){
            ar& value;
            ar& aggSig;
        }
    };

    struct SubmitMsg {
        int submitPid;        // id if the submit process
        int value;           // submit value
        accountable_confirmer_bls::Signature sig;    // Signature for the value
        accountable_confirmer_bls::PublicKey pub;    // Public key for the signature

        template<class Archive>
        void serialize(Archive& ar, const unsigned version){
            ar & submitPid;
            ar & value;
            ar & sig;
            ar & pub;
        }
    };

    void InitSubmitMsg(struct SubmitMsg* m, struct SubmitMsg* initValue);

//    struct MsgToSend {
//        SubmitMsg msgSubmit;
//        unsigned char msgHash[SHA256_DIGEST_LENGTH];
//        secp256k1_ecdsa_signature sig;  //signature for the whole SubmitMsg
//    };


// Generate MsgHash is the input of PKI signing
//    void GenerateMsgHash(struct SubmitMsg* m, unsigned char* hash);
}



#endif //ACCOUNTABLE_CONFIRMER_MESSAGE_H
