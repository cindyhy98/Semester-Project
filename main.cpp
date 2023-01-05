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

using namespace std;

#define MAX_PEERS_NUMBER 20

vector<int> configParam[4];

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

void ParseConfigParam(int index, string str){
    stringstream ss;
    ss << str;
    while(getline(ss, str, ' ')){
        // need to ignore the first value -> it's the parameter's name
        configParam[index].push_back(atoi(str.c_str()));
    }

}

bool LoadConfig(string filename) {
    ifstream file(filename);
    string str;
    if (file.is_open()) {
        int index = 0;
        configParam[index].clear();
        while (getline(file, str)) {
            ParseConfigParam(index, str);
            index++;
        }
        file.close();
        return true;
    } else {
        cout << "Unable to open file";
        return false;
    }
}

int main(int argc, char *argv[])
{
    /* Test for correct number of parameters */
    if (argc < 2) {
        printf("Not enough input\n");
        exit(1);
    }

    string file_path = argv[1];
    LoadConfig(file_path);

    int totalPeers = configParam[0][1];
    int id = configParam[1][1];
    vector<int> submitValue;
    submitValue.clear();
    for (int i = 0; i < configParam[2][1]; i++){
        submitValue.push_back(configParam[2][i+2]);
    }
    vector<int> sendto;
    sendto.clear();
    for (int i = 0; i < configParam[2][1]; i++){
        sendto.push_back(configParam[3][i+1]);
    }

    /* Start Server */
    thread s(StartServer, id+8000);
    printf("[%d] Server Started\n", id+8000);
    usleep(100000);

    accountable_confirmer::Peer P;
    accountable_confirmer::InitPeer(&P, id, totalPeers);


    /* Submit */
    int size = sendto.size();
    printf("[%d] sendto size: %d\n",id, size);
    for (int i = 0; i < size; i++){

        accountable_confirmer::Submit(&P, submitValue.at(i), sendto.at(i));
        usleep(100000);

    }
    while(!P.detectConflict) {
        // keep waiting
    }

    printf("[%d] DETECT CONFLICT!!\n", id);

    accountable_confirmer::Close(&P);
    s.join();



    return 0;
}