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
    char msg[] = "hello";

    testbls::KeyGen(sec, pub);
    blsSignature sig = testbls::Sign(sec, msg);

    int n = testbls::Verify(sig, pub, msg);
    if (n) {
        cout << "Correct Verify" << endl;
    } else {
        cout << "Wrong Verify" << endl;
    }


    return 0;
}