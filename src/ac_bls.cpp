/* Standard Library */
#include <stdio.h>

/* Import BLS */
#include "ac_bls.h"

using namespace std;
namespace ac_bls {
    struct Key k;

    void Init() {
        // init library at once before calling the other APIs

        int err = blsInit(MCL_BLS12_381, MCLBN_COMPILED_TIME_VAR);
        if (err != 0) {
            printf("blsInit err %d\n", err);
            exit(1);
        }

        // use the latest eth2.0 spec
        blsSetETHmode(BLS_ETH_MODE_LATEST);

        printf("[ac_bls::Init] Successful! \n");
    }

    void KeyGen(Key* k) {
        // init SecretKey sec by random number
        blsSecretKeySetByCSPRNG(&k->sec);

        // get PublicKey pub from SecretKey sec
        blsGetPublicKey(&k->pub, &k->sec);

        printf("[ac_bls::KeyGen] Generate PublicKey from SecretKey\n");
    }

    void Sign(Key* k, blsSignature *sig, char msg[]) {
        const size_t msgSize = strlen(msg);

        blsSign(sig, &k->sec, msg, msgSize);
        printf("[ac_bls::Sign] Make a Signature\n");
    }

    /* return 1 if it is valid else 0 */
    int Verify(Key* k, blsSignature sig, char msg[]) {
        const size_t msgSize = strlen(msg);
        return blsVerify(&sig, &k->pub, msg, msgSize);
    }

    void AggSign(blsSignature *aggSig, const blsSignature *sigVec, mclSize n) {
        blsAggregateSignature(aggSig, sigVec, n);
        printf("[ac_bls::AggSign] Make a Aggregate Signature\n");
    }


    int FastAggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, mclSize n,  const void *msg, mclSize msgSize){
        return blsFastAggregateVerify(sig, pubVec, n, msg,msgSize);
    }

    /*
    pubVec is n array of PublicKey
    msgVec is (n * msgSize) -byte array, which concatenates n-byte messages of length msgSize
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