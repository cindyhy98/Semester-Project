#include <iostream>
#include "tcp_client.h"
#include <thread>

using boost::asio::ip::tcp;
int main(){
//    try {
//        // create a context object -> boost use it to keep information on your input output
//        boost::asio::io_context io_context;
//
//        // want to connect
//        tcp::resolver resolver(io_context);
//
////        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "1337")
//        auto endpoints = resolver.resolve("127.0.0.1", "1337");
//
//        // create a socket
//        tcp::socket socket_ {io_context};
//        // connect to a socket
//        boost::asio::connect(socket_, endpoints);
//
//
//        // listen to the message
//        while(true) {
//            std::array<char, 128> buf {};
//            boost::system::error_code error;
//
//            size_t len = socket_.read_some(boost::asio::buffer(buf), error);
//
//            if (error == boost::asio::error::eof) {
//                // server cut the connection
//                break;
//            } else if (error) {
//                throw boost::system::system_error(error);
//            }
//
//            std::cout.write(buf.data(), len);
//        }
//    } catch (std::exception& e) {
//        std::cerr << e.what() << std::endl;
//    }
    TCPClient client("localhost", 1337);
    client.OnMessage = [](const std::string& message) {
        std::cout << message;
    };

    // give client a new thread
    std:: thread t( [&client] () { client.Run();});

    while(true) {
        std::string message;
        getline(std::cin, message);

        if (message == "quit")   break;
        message += "\n";

        client.Post(message);
    }

    client.Stop();
    t.join();
    return 0;
}