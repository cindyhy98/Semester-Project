/* Internal Library */
#include "esdsa_pki.h"


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

        /* Serialize the pubkey in a compressed form(33 bytes). Should always return 1. */
        size_t  len = sizeof(k->compressed_pub);
        return_val = secp256k1_ec_pubkey_serialize(k->ctx, k->compressed_pub, &len, &k->pub, SECP256K1_EC_COMPRESSED);
        assert(return_val);
        /* Should be the same size as the size of the output, because we passed a 33 byte array. */
        assert(len == sizeof(k->compressed_pub));

    }


    void Sign(struct Key* k, unsigned char* msgHash){
        /* Generate an ECDSA signature `noncefp` and `ndata` allows you to pass a
         * custom nonce function, passing `NULL` will use the RFC-6979 safe default.
         * Signing with a valid context, verified secret key
         * and the default nonce function should never fail. */

        // TODO: Check if the way of passing msgHash is right or wrong
        int return_val = secp256k1_ecdsa_sign(k->ctx, &k->sig, msgHash, k->sec, NULL, NULL);
        assert(return_val);

        /* Serialize the signature in a compact form. Should always return 1
         * according to the documentation in secp256k1.h. */
        return_val = secp256k1_ecdsa_signature_serialize_compact(k->ctx, k->serialized_sig, &k->sig);
        assert(return_val);
    }

    int Verify(struct Key* k, unsigned char* msgHash){
        /* Deserialize the signature. This will return 0 if the signature can't be parsed correctly. */
        if (!secp256k1_ecdsa_signature_parse_compact(k->ctx, &k->sig, k->serialized_sig)) {
            printf("Failed parsing the signature\n");
            return 1;
        }

        /* Deserialize the public key. This will return 0 if the public key can't be parsed correctly. */
        if (!secp256k1_ec_pubkey_parse(k->ctx, &k->pub, k->compressed_pub, sizeof(k->compressed_pub))) {
            printf("Failed parsing the public key\n");
            return 1;
        }

        /* Verify a signature. This will return 1 if it's valid and 0 if it's not. */
        k->is_signature_valid = secp256k1_ecdsa_verify(k->ctx, &k->sig, msgHash, &k->pub);

        return k->is_signature_valid;
    }

    void End(struct Key* k){
        /* This will clear everything from the context and free the memory */
        secp256k1_context_destroy(k->ctx);

        memset(k->sec, 0, sizeof(k->sec));
    }


}
