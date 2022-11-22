/* Standard Library */
#include <stdio.h>


/* Internal Library */
#include "accountable_confirmer.h"

/* Define */
#define SUBMIT_VALUE 334949

using namespace std;

int main(int argc,char *argv[])
{

    accountable_confirmer::Process P[NUMBER_OF_PROCESS];
    int portNumber[5] = {9000, 9001, 9002, 9003};
    for(int i = 0; i < NUMBER_OF_PROCESS; i++){
        accountable_confirmer::InitProcess(&P[i], portNumber[i]);
    }

    /* Receive the submitted value */
    for(int i = 0; i < NUMBER_OF_PROCESS; i++){
        accountable_confirmer::ReceiveSubmitProcess(&P[i]);
    }
    return 0;
}