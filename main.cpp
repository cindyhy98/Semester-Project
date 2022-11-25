/* Standard Library */
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <thread>

/* Internal Library */
#include "accountable_confirmer.h"

///* Define */
//#define SUBMIT_VALUE 334949
//#define SUBMIT_VALUE_FAKE 666666

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)                     /* Test for correct number of parameters */
    {
        exit(1);
    }


    int portNumber = atoi(argv[1]);
    int submitValue = atoi(argv[2]);

    accountable_confirmer::Process P;
    accountable_confirmer::InitProcess(&P, portNumber);
    thread msgListener(accountable_confirmer::ReceiveSubmitMessage, &P);
    thread aggSignatureListener (accountable_confirmer::ReceiveAggregateSignature, &P);

    usleep(3);


    accountable_confirmer::Submit(&P, submitValue);

    usleep(10);

    int detect = 0;
    if(accountable_confirmer::CheckRecvMsg(&P)) {
        detect = accountable_confirmer::CheckRecvAggSignature(&P);
    }
//    while(!accountable_confirmer::CheckRecvMsg(&P)) {
//        printf("=================Keep Checking the RecvMsg===================\n");
//    }


//    if(detect) {
//        printf("====================(%d) Detect Faulty====================\n");
//    } else {
//        printf("====================(%d) No Faulty====================\n");
//    }

    msgListener.join();
    aggSignatureListener.join();


    return 0;
}