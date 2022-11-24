/* Standard Library */
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <thread>

/* Internal Library */
#include "accountable_confirmer.h"

/* Define */
#define SUBMIT_VALUE 334949
#define SUBMIT_VALUE_FAKE 666666
using namespace std;

void StartThePeerReceiver(){

}

void StartThePeerSender();

int main(int argc,char *argv[])
{
    thread receiver(StartThePeerReceiver);
    thread sender(StartThePeerSender);


    return 0;
}