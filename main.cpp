/* Standard Library */
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

/* Internal Library */
#include "testbls.h"
#include "generate_pki.h"
/* Define */
#define NUMBER_OF_PROCESS size_t(4)

using namespace std;


int main(int argc,char *argv[])
{
//    testbls::Init();
//
//    char msg[] = "hello";
//    const size_t msgSize = strlen(msg);
//
//    /* Test Single Signature*/
//    blsSecretKey sec;
//    blsPublicKey pub;
//    blsSignature sig;
//
//    testbls::KeyGen(&sec, &pub);
//    testbls::Sign(&sig, &sec, msg);
//
//    int verSimple = testbls::Verify(sig, pub, msg);
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
//        testbls::KeyGen(&secVec[i], &pubVec[i]);
//        testbls::Sign(&sigVec[i], &secVec[i], msg);
//    }
//
//    testbls::AggSign(&aggSig, &sigVec[0], msgSize);
//
//    int verAgg = testbls::FastAggSignVerify(&aggSig, &pubVec[0], NUMBER_OF_PROCESS, msg, msgSize);
//    if (verAgg) printf("[VerifyAgg] Correct!\n");
//    else   printf("[VerifyAgg] Wrong!\n");
//
//    return 0;


    /* Generate the key. */
    cout << "Generating RSA key..." << endl;

    EVP_PKEY * pkey = pki::generate_key();
    if(!pkey){
        cout << "Fail to Generate..." << endl;
        return 1;
    }

    FILE* pkey_file = fopen("rootca.key", "wb");
    if (pkey_file == NULL)
        std::cout << "No Such File !! " << std::endl;

    PEM_write_PrivateKey(pkey_file, pkey, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(pkey_file);

    /* Generate the certificate. */
    std::cout << "Generating x509 certificate..." << std::endl;

    X509 * x509 = pki::generate_x509(pkey);
    if(!x509)
    {
        EVP_PKEY_free(pkey);
        return 1;
    }

    FILE* x509_file = fopen("rootca.crt", "wb");
    if (x509_file == NULL)
        std::cout << "No Such File !! " << std::endl;

    PEM_write_X509(x509_file, x509);
    fclose(x509_file);

    return 0;
}