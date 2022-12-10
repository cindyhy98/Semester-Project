/* Standard Library */
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <fstream>
#include <queue>

/* Internal Library */
#include "accountable_confirmer.h"
#include "message.h"

///* Define */
//#define SUBMIT_VALUE 334949
//#define SUBMIT_VALUE_FAKE 666666

using namespace std;



void StartServer(int serverPortNumber) {
    // Init Server
    TCPServer server(IPV::V4, serverPortNumber);

    server.OnJoin = [](TCPConnection::pointer server) { };

    server.OnLeave = [&serverPortNumber](TCPConnection::pointer server) {
        printf("User has left the server [%d]\n", serverPortNumber);
    };

    server.OnClientMessage = [&server](const std::string& message) {
        server.Broadcast(message);
    };
    server.Run();
}

int main(int argc, char *argv[])
{
    /* Test for correct number of parameters */
//    if (argc < 3)
//    {
//        printf("Not enough input\n");
//        exit(1);
//    }

    int portNumber = atoi(argv[1]);
    vector<int> submitValue;
    vector<int> sendto;
    int peersPortNumber[NUMBER_OF_PROCESSES] = {9001, 9002, 9003, 9004};
    int pid = portNumber - 9000;

    submitValue.clear();    sendto.clear();

    if (pid == 1){
        submitValue.push_back(334949);
        sendto.push_back(0);
    } else if (pid == 2){
        submitValue.push_back(666666);
        sendto.push_back(0);
    } else if (pid == 3){
        submitValue.push_back(666666);
        submitValue.push_back(334949);
        sendto.push_back(2);
        sendto.push_back(1);
    } else if (pid == 4){
        submitValue.push_back(666666);
        submitValue.push_back(334949);
        sendto.push_back(2);
        sendto.push_back(1);
    }

    accountable_confirmer::Peer P;
    accountable_confirmer::InitPeer(&P, pid, portNumber);


    /* Start Server */
    thread s(StartServer, portNumber);
    usleep(100000);

    /* Start client */
    TCPClient c1("localhost", peersPortNumber[0]);
    c1.OnMessage = [&P](const std::string& message) {
        P.recvRawMessage.push(message);
    };
    thread t1( [&c1] () { c1.Run(); for(;;);});
    P.clientThread.emplace_back(move(t1));

    TCPClient c2("localhost", peersPortNumber[1]);
    c2.OnMessage = [&P](const std::string& message) {
        P.recvRawMessage.push(message);
    };
    thread t2( [&c2] () { c2.Run(); for(;;);});
    P.clientThread.emplace_back(move(t2));

    TCPClient c3("localhost", peersPortNumber[2]);
    c3.OnMessage = [&P](const std::string& message) {
        P.recvRawMessage.push(message);
    };
    thread t3( [&c3] () { c3.Run(); for(;;);});
    P.clientThread.emplace_back(move(t3));

    TCPClient c4("localhost", peersPortNumber[3]);
    c4.OnMessage = [&P](const std::string& message) {
        P.recvRawMessage.push(message);
    };
    thread t4( [&c4] () {  c4.Run(); for(;;);});
    P.clientThread.emplace_back(move(t4));

    int size = sendto.size();
    for (int i = 0; i < size; i++){
        printf("[%d] This value(%d) is sent to %d\n", pid, submitValue.at(i), sendto.at(i));
        accountable_confirmer::Submit(&P, submitValue.at(i), sendto.at(i));
        usleep(1000000);
        string submit_message(reinterpret_cast<const char*>(P.serializeMsg.data()), P.serializeMsg.size());
        submit_message += '\n';

        switch (portNumber) {
            case 9001:
                c1.Post(submit_message);
                break;
            case 9002:
                c2.Post(submit_message);
                break;
            case 9003:
                c3.Post(submit_message);
                break;
            case 9004:
                c4.Post(submit_message);
                break;
        }
    }


    while(!P.recvMsgFlag) {
        // keep waiting
    }
    printf("[%d] GET ENOUGH MSG!!\n", pid);

    string aggSign(reinterpret_cast<const char*>(P.serializeAggSign.data()), P.serializeAggSign.size());
    aggSign += '\n';

    switch (portNumber) {
        case 9001:
            printf("[1] Post aggSign\n");
            c1.Post(aggSign);
            break;
        case 9002:
            printf("[2] Post aggSign\n");
            c2.Post(aggSign);
            break;
        case 9003:
            printf("[3] Post aggSign\n");
            c3.Post(aggSign);
            break;
        case 9004:
            printf("[4] Post aggSign\n");
            c4.Post(aggSign);
            break;
    }

    while(!P.detectConflict) {
        // keep waiting
    }

    printf("[%d] DETECT CONFLICT!!\n", pid);

    c1.Stop(); c2.Stop(); c3.Stop(); c4.Stop();
    accountable_confirmer::Close(&P);
    s.join();



    return 0;
}