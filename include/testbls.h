#ifndef ACCOUNTABLE_CONFIRMER_BLS_H
#define ACCOUNTABLE_CONFIRMER_BLS_H

/* Import BLS */
#define BLS_ETH
#include <mcl/bn_c384_256.h>
#include <bls/bls.h>


namespace testbls{
    void Init();
    void KeyGen(blsSecretKey *sec, blsPublicKey *pub);
    void Sign(blsSignature *sig, blsSecretKey *sec, char msg[]);
    int Verify(blsSignature sig, blsPublicKey pub, char msg[]);
    void AggSign(blsSignature *aggSig, const blsSignature *sigVec, mclSize n);
    int FastAggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, mclSize n,  const void *msg, mclSize msgSize);
    int AggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, const void *msgVec, mclSize msgSize, mclSize n);
}
#endif //ACCOUNTABLE_CONFIRMER_BLS_H
