#ifndef ACCOUNTABLE_CONFIRMER_BLS_H
#define ACCOUNTABLE_CONFIRMER_BLS_H

/* Import BLS */
#define BLS_ETH
#include "bls/bls.h"
#include "mcl/bn_c384_256.h"

namespace bls{
    void Init();
    void KeyGen(blsSecretKey sec, blsPublicKey pub);
    blsSignature Sign(blsSecretKey sec, char msg[]);
    int Verify(blsSignature sig, blsPublicKey pub, char msg[]);

}
#endif //ACCOUNTABLE_CONFIRMER_BLS_H
