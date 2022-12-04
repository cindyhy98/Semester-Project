#include <iostream>
#include "tcp_server.h"

using boost::asio::ip::tcp;

int main(){
    TCPServer server{IPV::V4, 1337};

    server.OnJoin = [](TCPConnection::pointer server) {
        std::cout << "User has joined the server: " << server->Getusername() << std::endl;
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
//    server.Run();
    std::thread t( [&server] () { server.Run();});
    t.join();
    return 0;
}