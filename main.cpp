/* Standard Library */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "bls.h"
using namespace std;

int main(int argc,char *argv[])
{
    bls::Init();

    blsSecretKey sec;
    blsPublicKey pub;
    char msg[] = "hello";

    bls::KeyGen(sec, pub);
    blsSignature sig = bls::Sign(sec, msg);

    int n = bls::Verify(sig, pub, msg);
    if (n) {
        cout << "Correct Verify" << endl;
    } else {
        cout << "Wrong Verify" << endl;
    }


    return 0;
}