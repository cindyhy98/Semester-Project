/* Standard Library */
#include <stdio.h>


/* Internal Library */
#include "accountable_confirmer.h"

/* Define */
#define SUBMIT_VALUE 334949
#define SUBMIT_VALUE_FAKE 666666
using namespace std;

accountable_confirmer::Peer P[NUMBER_OF_PROCESSES];
int portNumber[NUMBER_OF_PROCESSES] = {9000, 9001, 9002, 9003};
bool confirm[NUMBER_OF_PROCESSES] = {false, false, false, false};
bool detectConflict = false;

//int checkAggSignature(int confirmPeer){
//    for(int k = 0; k < NUMBER_OF_PROCESSES; k++){
//        if (k != confirmPeer) {
//            accountable_confirmer::PseudoReceiveAggSignature(&P[k],&P[confirmPeer]);
//            detectConflict = accountable_confirmer::DetectConflictAggSignature(&P[k]);
//
//            if(detectConflict){
//                return 1;
//            }
//        }
//    }
//    return 0;
//}

int main(int argc,char *argv[])
{
    for (int i = 0; i < argc; i++)


        for(int i = 0; i < NUMBER_OF_PROCESSES; i++){
            printf("===================== Init ==============================\n");
            accountable_confirmer::InitPeer(&P[i], portNumber[i]);

//        socket_t::InitServerSocket(&P[i].serverSocket, portNumber[i]);
        }


    /* Choose a random process to submit */
//    srand(time(0));
//    int tmpIndex = rand() % NUMBER_OF_PROCESSES;
    for (int j = 0; j < 3; j++){
        int tmpIndex = j;
        printf("=====================SUBMIT CORRECT NUMBER==============================\n");
        accountable_confirmer::Submit(&P[tmpIndex], SUBMIT_VALUE);

        for(int i = 0; i < NUMBER_OF_PROCESSES; i++){
            if (i != tmpIndex){
                printf("=====================RECEIVE SUBMIT MESSAGE==============================\n");
                accountable_confirmer::PseudoReceiveSubmitMessage(&P[i], &P[tmpIndex]);

                if (!accountable_confirmer::Confirm(&P[i])){
                    // Confirm and broadcast AggSignature to everyone
                    confirm[i] = true;
//                    if(checkAggSignature(i)){
//                        printf("=========================================================================\n");
//                        printf("Find the proof!\n");
//                        return 0;
//                    }
                } else {
                    // Not Confirm

                }
                printf("=========================================================================\n");
            }
        }

    }

    /* Submit a wrong value */
    printf("=====================SUBMIT FAKE NUMBER==============================\n");
    accountable_confirmer::Submit(&P[3], SUBMIT_VALUE_FAKE);
    for(int i = 0; i < NUMBER_OF_PROCESSES; i++){
        if (i != 3){
            printf("=====================RECEIVE SUBMIT MESSAGE==============================\n");
            accountable_confirmer::PseudoReceiveSubmitMessage(&P[i], &P[3]);

            if (!accountable_confirmer::Confirm(&P[i])){
                // Confirm
                confirm[i] = true;
//                if(checkAggSignature(i)){
//                    printf("=========================================================================\n");
//                    printf("Find the proof!\n");
//                    return 0;
//                }

            } else {
                // Not Confirm

            }
            printf("=========================================================================\n");
        }
    }

    printf("No faulty process!\n");
    return 0;


    /* Detect conflicting aggregate signature */
//    int index = 0;
//    while(!detectConflict && index < NUMBER_OF_PROCESSES){
//        if(confirm[index]){
//            printf("=====================DETECT CONFLICT AggSignature==============================\n");
//            detectConflict = accountable_confirmer::DetectConflictAggSignature(&P[index]);
//        }
//        index++;
//    }









    return 0;
}