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
    if(argc < 4) {
        printf("Exit without enough arguments");
    }

    int id = atoi(argv[1]);
    int portNumber = atoi(argv[2]);
    int submitValue = atoi(argv[3]);

    accountable_confirmer::Process P;
    accountable_confirmer::InitProcess(&P, portNumber);
    thread msgListener(accountable_confirmer::ReceiveSubmitMessage, &P);
    thread aggSignatureListener (accountable_confirmer::ReceiveAggregateSignature, &P);

    usleep(3);

    printf("====================Start To Submit Value (%d)====================\n", submitValue);
    accountable_confirmer::Submit(&P, submitValue);
    printf("====================Sleep For %d Seconds====================\n", id);
    usleep(5*id);
    printf("=================Start To Wait For Confirmation===================\n");
    int confirmed = accountable_confirmer::CheckRecvMsg(&P);

    int detect = 0;
    if(confirmed) {
        detect = accountable_confirmer::CheckRecvAggSignature(&P);
    } else {

    }

    if(detect) {
        printf("====================Done====================\n");
    } else {
        printf("====================No Faulty====================\n");
    }

    msgListener.join();
    aggSignatureListener.join();


    return 0;
}