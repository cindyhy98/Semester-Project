#ifndef ACCOUNTABLE_CONFIRMER_BLS_H
#define ACCOUNTABLE_CONFIRMER_BLS_H

/* Import BLS */
#define BLS_ETH
#include "mcl/bn_c384_256.h"
#include "bls/bls.h"


namespace testbls{
    void Init();
    void KeyGen(blsSecretKey sec, blsPublicKey pub);
    blsSignature Sign(blsSecretKey sec, char msg[]);
    int Verify(blsSignature sig, blsPublicKey pub, char msg[]);
}
#endif //ACCOUNTABLE_CONFIRMER_BLS_H
