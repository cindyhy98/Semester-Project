#ifndef ACCOUNTABLE_CONFIRMER_BLS_H
#define ACCOUNTABLE_CONFIRMER_BLS_H

/* Import BLS */
#define BLS_ETH
#include <mcl/bn_c384_256.h>
#include <bls/bls.h>


namespace ac_bls{

    struct Key {
        blsSecretKey sec;   // Private key
        blsPublicKey pub;   // Public Key
    };

    void Init();
    void KeyGen(Key* k);
    void Sign(blsSignature *sig, Key* k, char msg[]);
    int Verify(blsSignature sig, Key* k, char msg[]);
    void AggSign(blsSignature *aggSig, const blsSignature *sigVec, mclSize n);
    int FastAggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, mclSize n,  const void *msg, mclSize msgSize);
    int AggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, const void *msgVec, mclSize msgSize, mclSize n);
}
#endif //ACCOUNTABLE_CONFIRMER_BLS_H
