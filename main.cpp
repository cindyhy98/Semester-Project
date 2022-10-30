/* Standard Library */
#include <stdio.h>


/* Internal Library */
#include "ac.h"


/* Define */
#define NUMBER_OF_PROCESS size_t(4)

using namespace std;


int main(int argc,char *argv[])
{

//    ac_bls::Init();
//
//    char msg[] = "hello";
//    const size_t msgSize = strlen(msg);
//
//    /* Test Single Signature*/
//    blsSecretKey sec;
//    blsPublicKey pub;
//    blsSignature sig;
//
//    ac_bls::KeyGen(&sec, &pub);
//    ac_bls::Sign(&sig, &sec, msg);
//
//    int verSimple = ac_bls::Verify(sig, pub, msg);
//    if (verSimple) printf("[Verify] Correct!\n");
//    else   printf("[Verify] Wrong!\n");
//
//
//    /* Test Aggregate Signature */
//    vector<blsSecretKey> secVec(NUMBER_OF_PROCESS);
//    vector<blsPublicKey> pubVec(NUMBER_OF_PROCESS);
//    vector<blsSignature> sigVec(NUMBER_OF_PROCESS);
//    blsSignature aggSig;
//
//    for (size_t i = 0; i < NUMBER_OF_PROCESS; ++i) {
//        ac_bls::KeyGen(&secVec[i], &pubVec[i]);
//        ac_bls::Sign(&sigVec[i], &secVec[i], msg);
//    }
//
//    ac_bls::AggSign(&aggSig, &sigVec[0], msgSize);
//
//    int verAgg = ac_bls::FastAggSignVerify(&aggSig, &pubVec[0], NUMBER_OF_PROCESS, msg, msgSize);
//    if (verAgg) printf("[VerifyAgg] Correct!\n");
//    else   printf("[VerifyAgg] Wrong!\n");
//
//    return 0;


    return 0;
}