/* Standard Library */
#include <stdio.h>

/* Internal Library*/
#include "accountable_confirmer_bls.h"

using namespace std;
namespace accountable_confirmer_bls {

    void Init() {
        // init library at once before calling the other APIs

        int err = blsInit(MCL_BLS12_381, MCLBN_COMPILED_TIME_VAR);
        if (err != 0) {
            printf("blsInit err %d\n", err);
            exit(1);
        }

        // use the latest eth2.0 spec
        blsSetETHmode(BLS_ETH_MODE_LATEST);
    }

    void KeyGen(Key* k) {
        // init SecretKey sec by random number
        blsSecretKeySetByCSPRNG(&k->sec);

        // get PublicKey pub from SecretKey sec
        blsGetPublicKey(&k->pub, &k->sec);
    }


    void Sign(Key* k, blsSignature* sig, const char* msg) {
        blsSign(sig, &k->sec, msg, strlen(msg));
    }

    /* Return 1 if it is valid else 0 */
    int Verify(blsPublicKey* pub, blsSignature* sig, const char* msg){
        return blsVerify(sig, pub, msg, strlen(msg));
    }

    void AggSign(blsSignature *aggSig, const blsSignature *sigVec, mclSize n) {
        blsAggregateSignature(aggSig, sigVec, n);
    }

    /* Return 1 if it is valid else 0 */
    int FastAggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, mclSize n,  const void *msg, mclSize msgSize){
        return blsFastAggregateVerify(sig, pubVec, n, msg,msgSize);
    }

    /*
     * pubVec is n array of PublicKey
     * msgVec is (n * msgSize) -byte array, which concatenates n-byte messages of length msgSize
     * return 1 if it is valid else 0
     */
    int AggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, const void *msgVec, mclSize msgSize, mclSize n){
        /*
         * blsAggregateVerifyNoCheck does not check
            - sig has the correct order
            - every n-byte messages of length msgSize are different from each other
         * Check them at the caller if necessary.
         */
        return blsAggregateVerifyNoCheck(sig, pubVec, msgVec, msgSize,n);
    }

}