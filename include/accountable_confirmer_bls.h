#ifndef ACCOUNTABLE_CONFIRMER_BLS_H
#define ACCOUNTABLE_CONFIRMER_BLS_H

/* Import BLS */
#define BLS_ETH
#include <mcl/bn_c384_256.h>
#include <bls/bls.h>
#include <vector>
#include <iostream>

using namespace std;

namespace accountable_confirmer_bls {

    struct Key {
        blsSecretKey sec;   // Private key
        blsPublicKey pub;   // Public Key
    };


    void Init();

    void KeyGen(Key* k);

    void Sign(Key* k, blsSignature* sig, const char* msg);

    /* return 1 if it is valid else 0 */
    int Verify(blsPublicKey* pub, blsSignature* sig, const char* msg);

    void AggSign(blsSignature *aggSig, const blsSignature *sigVec, mclSize n);

    /* return 1 if it is valid else 0 */
    int FastAggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, mclSize n,  const void *msg, mclSize msgSize);

    /* return 1 if it is valid else 0 */
    int AggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, const void *msgVec, mclSize msgSize, mclSize n);
}
#endif //ACCOUNTABLE_CONFIRMER_BLS_H
