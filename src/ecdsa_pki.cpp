/* Internal Library */
#include "ecdsa_pki.h"


using namespace std;
namespace ecdsa_pki {
    struct Key k;

    void Init(struct Key* k){
        k->ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
        if (!fill_random(k->randomize, sizeof(k->randomize))) {
            printf("Failed to generate randomness\n");
            return;
        }

        /* Randomizing the context is recommended to protect against side-channel
        * leakage See `secp256k1_context_randomize` in secp256k1.h for more
        * information about it. This should never fail. */
        int return_val = secp256k1_context_randomize(k->ctx, k->randomize);
        assert(return_val);
    }

    void KeyGen(struct Key* k){
        /* If the secret key is zero or out of range (bigger than secp256k1's
        * order), we try to sample a new key. Note that the probability of this
        * happening is negligible. */
        while (1) {
            if (!fill_random(k->sec, sizeof(k->sec))) {
                printf("Failed to generate randomness\n");
                return;
            }
            if (secp256k1_ec_seckey_verify(k->ctx, k->sec)) {
                break;
            }
        }

        /* Public key creation using a valid context with a verified secret key should never fail */
        int return_val = secp256k1_ec_pubkey_create(k->ctx, &k->pub, k->sec);
        assert(return_val);

    }


    void Sign(struct Key* k,secp256k1_ecdsa_signature* sig, unsigned char* msgHash){
        /* Generate an ECDSA signature `noncefp` and `ndata` allows you to pass a
         * custom nonce function, passing `NULL` will use the RFC-6979 safe default.
         * Signing with a valid context, verified secret key
         * and the default nonce function should never fail. */

        // TODO: Check if the way of passing msgHash is right or wrong
        int return_val = secp256k1_ecdsa_sign(k->ctx, sig, msgHash, k->sec, NULL, NULL);
        assert(return_val);
    }

    int Verify(struct Key* k, secp256k1_ecdsa_signature* sig, unsigned char* msgHash){

        /* Verify a signature. This will return 1 if it's valid and 0 if it's not. */
        int is_signature_valid = secp256k1_ecdsa_verify(k->ctx, sig, msgHash, &k->pub);

        return is_signature_valid;
    }

    void End(struct Key* k){
        /* This will clear everything from the context and free the memory */
        secp256k1_context_destroy(k->ctx);

        memset(k->sec, 0, sizeof(k->sec));
    }


}
