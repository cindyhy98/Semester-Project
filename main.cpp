/* Standard Library */
#include <stdio.h>


/* Internal Library */
#include "ac.h"

/* Define */
#define SUBMIT_VALUE 334949
#define SUBMIT_VALUE_FAKE 666666
using namespace std;


int main(int argc,char *argv[])
{
    accountable_confirmer::Process P[NUMBER_OF_PROCESS];
    int portNumber[NUMBER_OF_PROCESS] = {9000, 9001, 9002, 9003};
    bool confirm[NUMBER_OF_PROCESS] = {false, false, false, false};
    bool detectConflict = false;

    for(int i = 0; i < NUMBER_OF_PROCESS; i++){
        accountable_confirmer::InitProcess(&P[i], portNumber[i]);
        printf("==================================================================\n");
//        socket_t::InitServerSocket(&P[i].serverSocket, portNumber[i]);
    }


    /* Choose a random process to submit */
//    srand(time(0));
//    int tmpIndex = rand() % NUMBER_OF_PROCESS;
    for (int j = 0; j < 3; j++){
        int tmpIndex = j;
        accountable_confirmer::Submit(&P[tmpIndex], SUBMIT_VALUE);
        printf("=====================CORRECT NUMBER==============================\n");
        for(int i = 0; i < NUMBER_OF_PROCESS; i++){
            if (i != tmpIndex){
                accountable_confirmer::PseudoReceiveSubmitProcess(&P[i], &P[tmpIndex]);

                if (!accountable_confirmer::Confirm(&P[i])){
                    // Confirm
                    confirm[i] = true;
                    accountable_confirmer::PseudoReceiveLightCert(&P[i],&P[tmpIndex]);
                } else {
                    // Not Confirm

                }
                printf("==================================================================\n");
            }
        }

    }

    /* Submit a wrong value */
    accountable_confirmer::Submit(&P[3], SUBMIT_VALUE_FAKE);
    printf("=====================WRONG NUMBER==============================\n");
    for(int i = 0; i < NUMBER_OF_PROCESS; i++){
        if (i != 3){
            accountable_confirmer::PseudoReceiveSubmitProcess(&P[i], &P[3]);

            if (!accountable_confirmer::Confirm(&P[i])){
                // Confirm
                confirm[i] = true;
                accountable_confirmer::PseudoReceiveLightCert(&P[i],&P[3]);

            } else {
                // Not Confirm

            }
            printf("==================================================================\n");
        }
    }


    /* Detect conflicting aggregate signature */
    for(int i = 0; i < NUMBER_OF_PROCESS; i++){
       if(confirm[i]){
           printf("TEST\n");
           accountable_confirmer::DetectConflictLightCert(&P[i]);
           printf("==================================================================\n");
           break;
       }
    }










    return 0;
}