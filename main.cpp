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
    int portNumber[5] = {9000, 9001, 9002, 9003};
    bool confirm = false;
    bool detectConflict = false;

    for(int i = 0; i < NUMBER_OF_PROCESS; i++){
        accountable_confirmer::InitProcess(&P[i], portNumber[i]);
        printf("==================================================================\n");
    }


    /* Choose a random process to submit */
//    srand(time(0));
//    int tmpIndex = rand() % NUMBER_OF_PROCESS;
    for (int j = 0; j < 3; j++){
        int tmpIndex = j;
        accountable_confirmer::Submit(&P[tmpIndex], SUBMIT_VALUE);
        printf("==================================================================\n");
        for(int i = 0; i < NUMBER_OF_PROCESS; i++){
            if (i != tmpIndex){
                accountable_confirmer::PseudoReceiveSubmitProcess(&P[i], &P[tmpIndex]);

                if (!accountable_confirmer::Confirm(&P[i])){
                    // Confirm
                    confirm = true;

                } else {
                    // Not Confirm

                }
                printf("==================================================================\n");
            }
        }

    }







    return 0;
}