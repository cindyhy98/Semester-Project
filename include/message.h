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

        template<class Archive>
        void serialize(Archive& ar, const unsigned version){
            ar & value;

            for (int i = 0; i < 2; i++){
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & aggSig.v.x.d[i].d[j];
                }

            }
            for (int i = 0; i < 2; i++){
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & aggSig.v.y.d[i].d[j];
                }

            }
            for (int i = 0; i < 2; i++){
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & aggSig.v.z.d[i].d[j];
                }
            }
//            ar& aggSig;
        }
    };

    struct SubmitMsg {
        int submitPid;                               // id for the submit process
        int value;                                   // submit value
        blsSignature sig;    // Signature for the value
        blsPublicKey pub;    // Publickey for the signature

        template<class Archive>
        void serialize(Archive& ar, const unsigned version){
            ar & submitPid;
            ar & value;

            // Serialize blsSignature
            for (int i = 0; i < 2; i++){
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & sig.v.x.d[i].d[j];
                }

            }
            for (int i = 0; i < 2; i++){
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & sig.v.y.d[i].d[j];
                }

            }
            for (int i = 0; i < 2; i++){
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & sig.v.z.d[i].d[j];
                }

            }

            // Serialize blsPublicKey
            for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                ar & pub.v.x.d[j];
            }
            for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                ar & pub.v.y.d[j];
            }
            for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                ar & pub.v.z.d[j];
            }
//            ar & sig;
//            ar & pub;
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
