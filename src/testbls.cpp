/* Import BLS */
#include "testbls.h"
#include <stdio.h>

using namespace std;
namespace testbls {

    void Init() {
        // init library at once before calling the other APIs

        int err = blsInit(MCL_BLS12_381, MCLBN_COMPILED_TIME_VAR);
        if (err != 0) {
            printf("blsInit err %d\n", err);
            exit(1);
        }

        // use the latest eth2.0 spec
        blsSetETHmode(BLS_ETH_MODE_LATEST);

        printf("[Init] Successful! \n");
    }

    void KeyGen(blsSecretKey *sec, blsPublicKey *pub) {
        // init SecretKey sec by random number
        blsSecretKeySetByCSPRNG(sec);

        // get PublicKey pub from SecretKey sec
        blsGetPublicKey(pub, sec);

        printf("[KeyGen] PublicKey from SecretKey\n");
    }

    void Sign(blsSignature *sig, blsSecretKey *sec, char msg[]) {
        const size_t msgSize = strlen(msg);

        blsSign(sig, sec, msg, msgSize);
        printf("[Sign] Make a Signature\n");
    }

    /* return 1 if it is valid else 0 */
    int Verify(blsSignature sig, blsPublicKey pub, char msg[]) {
        const size_t msgSize = strlen(msg);
        return blsVerify(&sig, &pub, msg, msgSize);
    }

}