/* Standard Library */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "testbls.h"
using namespace std;


int main(int argc,char *argv[])
{
    testbls::Init();

    blsSecretKey sec;
    blsPublicKey pub;
    blsSignature sig;
    char msg[] = "hello";


    testbls::KeyGen(&sec, &pub);
    testbls::Sign(&sig, &sec, msg);

    int n = testbls::Verify(sig, pub, msg);
    if (n) printf("[Verify] Correct!\n");
    else   printf("[Verify] Wrong!\n");

    return 0;
}