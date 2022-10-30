#ifndef ACCOUNTABLE_CONFIRMER_ESDSA_PKI_H
#define ACCOUNTABLE_CONFIRMER_ESDSA_PKI_H
#include <cassert>
#include <secp256k1.h>

/* Internal */
#include "random.h"
#include "message.h"

using namespace std;
namespace ecdsa_pki {

    struct Key {
        int is_signature_valid;

        unsigned char sec[32];
        unsigned char randomize[32];
        unsigned char compressed_pub[33];
        unsigned char serialized_sig[64];

        secp256k1_pubkey pub;
        secp256k1_ecdsa_signature sig;
        secp256k1_context* ctx;
    };


    void Init(struct Key* k);

    void KeyGen(struct Key* k);

    void Sign(struct Key* k, unsigned char* msgHash);

    int Verify(struct Key* k, unsigned char* msgHash);

    void End(struct Key* k);

}

#endif //ACCOUNTABLE_CONFIRMER_ESDSA_PKI_H
