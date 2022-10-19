/* Standard Library */
#include <stdio.h>
#include <stdlib.h>
#include <vector>

/* Internal Library */
#include "testbls.h"

/* Define */
#define NUMBER_OF_PROCESS size_t(3)

using namespace std;


int main(int argc,char *argv[])
{
    testbls::Init();

    blsSecretKey sec;
    blsPublicKey pub;
    blsSignature sig;
    char msg[] = "hello";
    const size_t msgSize = strlen(msg);


    testbls::KeyGen(&sec, &pub);
    testbls::Sign(&sig, &sec, msg);

    int verSimple = testbls::Verify(sig, pub, msg);
    if (verSimple) printf("[Verify] Correct!\n");
    else   printf("[Verify] Wrong!\n");


    /* Test Aggregate Signature */
    vector<blsSecretKey> secVec(NUMBER_OF_PROCESS);
    vector<blsPublicKey> pubVec(NUMBER_OF_PROCESS);
    vector<blsSignature> sigVec(NUMBER_OF_PROCESS);
    blsSignature aggSig;


    for (size_t i = 0; i < NUMBER_OF_PROCESS; ++i)
    {
        testbls::KeyGen(&secVec[i], &pubVec[i]);
        testbls::Sign(&sigVec[i], &secVec[i], msg);
    }

    testbls::AggSign(&aggSig, &sigVec[0], msgSize);


    int verAgg = testbls::FastAggSignVerify(&aggSig, &pubVec[0], NUMBER_OF_PROCESS, msg, msgSize);
    if (verAgg) printf("[VerifyAgg] Correct!\n");
    else   printf("[VerifyAgg] Wrong!\n");

    return 0;
}