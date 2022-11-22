#ifndef ACCOUNTABLE_CONFIRMER_ECDSA_PKI_H
#define ACCOUNTABLE_CONFIRMER_ECDSA_PKI_H
#include <cassert>
#include <cstring>
#include <secp256k1.h>

/* Internal */
#include "random.h"


using namespace std;
namespace ecdsa_pki {

    struct Key {
        secp256k1_context* ctx;
        unsigned char randomize[32];
        unsigned char sec[32];
        secp256k1_pubkey pub;

    };


    void Init(struct Key* k);

    void KeyGen(struct Key* k);

    void Sign(struct Key* k,secp256k1_ecdsa_signature* sig, unsigned char* msgHash);

    int Verify(struct Key* k,secp256k1_ecdsa_signature* sig, unsigned char* msgHash);

    void End(struct Key* k);

}

#endif //ACCOUNTABLE_CONFIRMER_ECDSA_PKI_H
