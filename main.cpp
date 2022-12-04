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

void ParseAggSignMessage(string message, message::SubmitAggSignMsg* recvAggSign) {
    int start = 0; int end = (int)message.find("::");
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
}

void ParseSubmitMessage(string message, message::SubmitMsg* recvMsg) {
    int start = 0; int end = (int)message.find("::");
    string pid_str(message.substr(start, end));
    recvMsg->submitPid = atoi(pid_str.c_str());

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
}

void StartServer(int serverPortNumber) {
    // Init Server
    TCPServer server(IPV::V4, serverPortNumber);

    server.OnJoin = [&serverPortNumber](TCPConnection::pointer server) {
        userCount++;
        if (userCount == TOTAL_USER)    postFlag = 1;
        printf("User has joined the server [%d]\n", serverPortNumber);
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
    if (argc < 3)                     /* Test for correct number of parameters */
    {
        exit(1);
    }

    bool closeFlag = 0;
    int portNumber = atoi(argv[1]);
    int submitValue = atoi(argv[2]);
    int peersPortNumber[NUMBER_OF_PROCESSES] = {9000, 9001, 9002, 9003};

    accountable_confirmer::Process P;

    // Start Server
    thread s(StartServer, portNumber);

    // Start client
    TCPClient c0("localhost", peersPortNumber[0]);
    c0.OnMessage = [&P](const std::string& message) {
        // store them in process

        int start = 0; int end = (int)message.find("::");
        string message_type(message.substr(start, end));
        printf("Recv Message type = %s\n", message_type.c_str());

        string trim_message = message.substr(end+2, message.length());

        if (message_type == "submit") {
            message::SubmitMsg* recvMsg;
            ParseSubmitMessage(trim_message, recvMsg);
            printf("Submitpid = %d, pid = %d\n", recvMsg->submitPid, P.id);
            accountable_confirmer::SubmitMsgVerify(&P.ac, recvMsg);
//            P.recvMsgQueue.push(*recvMsg);

        } else if(message_type == "aggSign") {
            message::SubmitAggSignMsg* recvAggSign;
            ParseAggSignMessage(trim_message, recvAggSign);
            P.recvAggSignQueue.push(*recvAggSign);
        } else {
            // couldn't parse
        }


    };
    thread t0( [&c0] () { printf("run c0\n"); c0.Run();});
    P.clientThread.push_back(move(t0));

    TCPClient c1("localhost", peersPortNumber[1]);
    c1.OnMessage = [&P](const std::string& message) {
        // store them in process

        int start = 0; int end = (int)message.find("::");
        string message_type(message.substr(start, end));
        printf("Recv Message type = %s\n", message_type.c_str());

        string trim_message = message.substr(end+2, message.length());

        if (message_type == "submit") {
            message::SubmitMsg* recvMsg;
            ParseSubmitMessage(trim_message, recvMsg);
            printf("Submitpid = %d, pid = %d\n", recvMsg->submitPid, P.id);
            accountable_confirmer::SubmitMsgVerify(&P.ac, recvMsg);
//            P.recvMsgQueue.push(*recvMsg);

        } else if(message_type == "aggSign") {
            message::SubmitAggSignMsg* recvAggSign;
            ParseAggSignMessage(trim_message, recvAggSign);
            P.recvAggSignQueue.push(*recvAggSign);
        } else {
            // couldn't parse
        }

    };
    thread t1( [&c1] () { printf("run c1\n");c1.Run();});
    P.clientThread.push_back(move(t1));

    TCPClient c2("localhost", peersPortNumber[2]);
    c2.OnMessage = [&P](const std::string& message) {
        // store them in process

        int start = 0; int end = (int)message.find("::");
        string message_type(message.substr(start, end));
        printf("Recv Message type = %s\n", message_type.c_str());

        string trim_message = message.substr(end+2, message.length());

        if (message_type == "submit") {
            message::SubmitMsg* recvMsg;
            ParseSubmitMessage(trim_message, recvMsg);
            printf("Submitpid = %d, pid = %d\n", recvMsg->submitPid, P.id);
            accountable_confirmer::SubmitMsgVerify(&P.ac, recvMsg);
//            P.recvMsgQueue.push(*recvMsg);

        } else if(message_type == "aggSign") {
            message::SubmitAggSignMsg* recvAggSign;
            ParseAggSignMessage(trim_message, recvAggSign);
            P.recvAggSignQueue.push(*recvAggSign);
        } else {
            // couldn't parse
        }
    };
    thread t2( [&c2] () { printf("run c2\n"); c2.Run();});
    P.clientThread.push_back(move(t2));

    TCPClient c3("localhost", peersPortNumber[3]);
    c2.OnMessage = [&P](const std::string& message) {
        // store them in process

        int start = 0; int end = (int)message.find("::");
        string message_type(message.substr(start, end));
        printf("Recv Message type = %s\n", message_type.c_str());

        string trim_message = message.substr(end+2, message.length());

        if (message_type == "submit") {
            message::SubmitMsg* recvMsg;
            ParseSubmitMessage(trim_message, recvMsg);
            printf("Submitpid = %d, pid = %d\n", recvMsg->submitPid, P.id);
            accountable_confirmer::SubmitMsgVerify(&P.ac, recvMsg);
            //P.recvMsgQueue.push(*recvMsg);

        } else if(message_type == "aggSign") {
            message::SubmitAggSignMsg* recvAggSign;
            ParseAggSignMessage(trim_message, recvAggSign);
            P.recvAggSignQueue.push(*recvAggSign);
        } else {
            // couldn't parse
        }
    };
    thread t3( [&c3] () { printf("run c3\n"); c3.Run();});
    P.clientThread.push_back(move(t3));


    accountable_confirmer::InitProcess(&P, portNumber, peersPortNumber);
    accountable_confirmer::Submit(&P, submitValue);
    string submit_message((char*)P.serializeMsg.data());
//    string submit_message(reinterpret_cast<const char*>(P.serializeMsg.data()), P.serializeMsg.size());
    submit_message += '\n';
    usleep(10);


    switch (portNumber) {
        case 9000:
            c0.Post(submit_message);
        case 9001:
            c1.Post(submit_message);
        case 9002:
            c2.Post(submit_message);
        case 9003:
            c3.Post(submit_message);
    }

    usleep(10);

//    for (int i = 0; i < 10; i++){
//        printf("check recvmsg...\n");
//        if (accountable_confirmer::CheckRecvMsg(&P)) {
//            break;
//        } else {
//
//        }
//    }
//    printf("[%d] received enough submit\n", P.id);
//    closeFlag = 1;

    string aggsign_message((char*)P.serializeAggSign.data());
//    string submit_message(reinterpret_cast<const char*>(P.serializeMsg.data()), P.serializeMsg.size());
    aggsign_message += '\n';

//    int detect = 0;
//    if(accountable_confirmer::CheckRecvMsg(&P)) {
//        detect = accountable_confirmer::CheckRecvAggSignature(&P);
//    }
//    while(!accountable_confirmer::CheckRecvMsg(&P)) {
//        printf("=================Keep Checking the RecvMsg===================\n");
//    }

//    if(detect) {
//        printf("====================Detect Faulty====================\n");
//    } else {
//        printf("====================No Faulty====================\n");
//    }

    usleep(10);
    closeFlag = 1;

    /*
    if(closeFlag) {
        c0.Stop(); c1.Stop(); c2.Stop(); c3.Stop();
        accountable_confirmer::Close(&P);
        s.join();
    }

    */
    return 0;
}