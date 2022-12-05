/* Standard Library */
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <fstream>

/* Internal Library */
#include "accountable_confirmer.h"
#include "message.h"

///* Define */
//#define SUBMIT_VALUE 334949
//#define SUBMIT_VALUE_FAKE 666666

using namespace std;

#define TOTAL_USER 4

int userCount = 0;
int postFlag = 0;

accountable_confirmer::Peer P;
accountable_confirmer::AccountableConfirmer AC;

bool ParseAggSignMessage(string message, message::SubmitAggSign* recvAggSign) {

    int start = 0; int end = (int)message.find("::");
    string pid_str(message.substr(start, end));
    recvAggSign->pid = atoi(pid_str.c_str());

//    if (recvAggSign->pid == P.id) {
//        /* Ignore the message that comes from yourself */
//        printf("Ignore aggSig coming from yourself\n");
//        return false;
//    }

    start = end+2; end = (int)message.find("::");
    string value_str(message.substr(start, end-start));
    recvAggSign->value = atoi(value_str.c_str());

    start = end+2; end = (int)message.find("::", start);
    string sig_sz_str(message.substr(start, end-start));
    int sig_sz = atoi(sig_sz_str.c_str());

    start = end+2; end = (int)message.find("::", start);
    string sig_str(message.substr(start, end-start));

    byte sig_byte[sig_str.length()];
    memcpy(sig_byte, sig_str.data(), sig_sz);
    byte* sig_tmp = sig_byte;
    blsSignatureDeserialize(&recvAggSign->aggSig, sig_tmp, sig_sz);

    return true;
}

bool ParseSubmitMessage(string message, message::SubmitMsg* recvMsg) {

    int start = 0; int end = (int)message.find("::");
    string pid_str(message.substr(start, end));
    recvMsg->pid = atoi(pid_str.c_str());

//    if (recvMsg->pid == P.id) {
//        /* Ignore the message that comes from yourself */
//        printf("Ignore message coming from yourself\n");
//        return false;
//    }

    start = end+2; end = (int)message.find("::", start);
    string value_str(message.substr(start, end-start));
    recvMsg->value = atoi(value_str.c_str());

    start = end+2; end = (int)message.find("::", start);
    string sig_sz_str(message.substr(start, end-start));
    int sig_sz = atoi(sig_sz_str.c_str());

    start = end+2; end = (int)message.find("::", start);
    string sig_str(message.substr(start, end-start));

    byte sig_byte[sig_str.length()];
    memcpy(sig_byte, sig_str.data(), sig_sz);
    byte* sig_tmp = sig_byte;
    blsSignatureDeserialize(&recvMsg->sig, sig_tmp, sig_sz);

    start = end+2; end = (int)message.find("::", start);
    string pub_sz_str(message.substr(start, end-start));
    int pub_sz = atoi(pub_sz_str.c_str());

    start = end+2; end = (int)message.find("::", start);
    string pub_str(message.substr(start, end-start));

    byte pub_byte[pub_str.length()];
    memcpy(pub_byte, pub_str.data(), pub_sz);
    byte* pub_tmp = pub_byte;
    blsPublicKeyDeserialize(&recvMsg->pub, pub_tmp, pub_sz);

    return true;
}

void ParseMessage(const std::string& message) {

    int start = 0; int end = (int)message.find("::");
    string message_type(message.substr(start, end));
    string trim_message = message.substr(end+2, message.length());

    if (message_type == "submit") {
        message::SubmitMsg* recvMsg;
        if (ParseSubmitMessage(trim_message, recvMsg)) {
            P.recvMsgQueue.push(*recvMsg);
            printf("Push recvMsg to queue\n");
        }

    } else if(message_type == "aggSign") {
        message::SubmitAggSign* recvAggSign;
        if (ParseAggSignMessage(trim_message, recvAggSign)) {
            P.recvAggSignQueue.push(*recvAggSign);
            printf("Push recvAggSign to queue\n");
        }


    } else {
        // couldn't parse
    }

    return;

}

void StartServer(int serverPortNumber) {
    // Init Server
    TCPServer server(IPV::V4, serverPortNumber);

    server.OnJoin = [&serverPortNumber](TCPConnection::pointer server) {
        userCount++;
        if (userCount == TOTAL_USER)    postFlag = 1;
//        printf("User has joined the server [%d]\n", serverPortNumber);
//        std::cout << "User has joined the server : " << server->Getusername() << std::endl;
    };

    server.OnLeave = [](TCPConnection::pointer server) {
        std::cout << "User has left the server: " << server->Getusername() << std::endl;
    };

    server.OnClientMessage = [&server](const std::string& message) {
        // parse the message
        //...
        // send the message to the server
        server.Broadcast(message);
    };
    server.Run();
}

int main(int argc, char *argv[])
{
    /* Test for correct number of parameters */
    if (argc < 3)
    {
        exit(1);
    }

    int portNumber = atoi(argv[1]);
    int submitValue = atoi(argv[2]);
    int peersPortNumber[NUMBER_OF_PROCESSES] = {9000, 9001, 9002, 9003};
    int pid = portNumber - 9000;

    /* Start Server */
    thread s(StartServer, portNumber);
    usleep(1000);
    /* Start client */
    TCPClient c0("localhost", peersPortNumber[0]);
    c0.OnMessage = ParseMessage;
    thread t0( [&c0] () { printf("run c0\n"); c0.Run();});
    P.clientThread.emplace_back(move(t0));

    TCPClient c1("localhost", peersPortNumber[1]);
    c1.OnMessage = ParseMessage;
    thread t1( [&c1] () { printf("run c1\n");c1.Run();});
    P.clientThread.emplace_back(move(t1));

    TCPClient c2("localhost", peersPortNumber[2]);
    c2.OnMessage = ParseMessage;
    thread t2( [&c2] () { printf("run c2\n"); c2.Run();});
    P.clientThread.emplace_back(move(t2));

    TCPClient c3("localhost", peersPortNumber[3]);
    c3.OnMessage = ParseMessage;
    thread t3( [&c3] () { printf("run c3\n"); c3.Run();});
    P.clientThread.emplace_back(move(t3));



    accountable_confirmer::InitAccountableConfirmer(&AC);
    accountable_confirmer::InitPeer(&P, &AC, pid, portNumber);

    accountable_confirmer::Submit(&P, &AC, submitValue);
    string submit_message((char*)P.serializeMsg.data());
//    string submit_message(reinterpret_cast<const char*>(P.serializeMsg.data()), P.serializeMsg.size());
    submit_message += '\n';
    usleep(10);


    switch (portNumber) {
        case 9000:
            printf("[0] post\n");
            c0.Post(submit_message);
        case 9001:
            printf("[1] post\n");
            c1.Post(submit_message);
        case 9002:
            printf("[2] post\n");
            c2.Post(submit_message);
        case 9003:
            printf("[3] post\n");
            c3.Post(submit_message);
    }

//
//    c0.Stop(); c1.Stop(); c2.Stop(); c3.Stop();
//    accountable_confirmer::Close(&P);
//    s.join();


//    if(closeFlag) {
//        c0.Stop(); c1.Stop(); c2.Stop(); c3.Stop();
//        accountable_confirmer::Close(&P);
//        s.join();
//        runRecv.join();
//    }


    return 0;
}