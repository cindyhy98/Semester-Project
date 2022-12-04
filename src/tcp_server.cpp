

#include "tcp_server.h"



using boost::asio::ip::tcp;
using net_error = boost::system::error_code;
TCPServer::TCPServer(IPV ipv, int port) : _ipVersion(ipv), _port(port),
_acceptor(_ioContext, tcp::endpoint(ipv == IPV::V4 ? tcp::v4() : tcp::v6(), port)){

}
int TCPServer::Run() {
    // run the io_context
    // a blocking method
    try {
        startAccept();

        // start the thread -> run until there is no more async operation
        _ioContext.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

void TCPServer::Stop(){
    net_error ec;

    // close the socket
    _acceptor.close(ec);

    if (ec) {
        //process error
    }
}

void TCPServer::Broadcast(const std::string& message){
    for (auto& connection: _connections) {
        connection->Post(message);
    }
}

// define async functions
void TCPServer::startAccept(){
    // when we are waiting for a new connection
    // -> create a socket object
    // -> waiting for the io_context
    _socket.emplace(_ioContext);


    // async accept a connection
    // takes arguments similar to async_write -> sockeet, buffer, callback
    _acceptor.async_accept(*_socket, [this](const net_error & error) {
        // create a connection
        // std::move -> clear the optional -> move the object into where we are sending in
        // -> convert *_socket into &&socket
        auto connection = TCPConnection::Create(std::move(*_socket));
        _connections.insert(connection);

        if (OnJoin) {
            OnJoin(connection);
        }

        if (!error) {
            // start the connection
            connection->Start(
                    [this](const std::string& message) { if (OnClientMessage) OnClientMessage(message); },
                [&, weak = std::weak_ptr(connection)] {
                    if (auto shared = weak.lock(); shared && _connections.erase(shared)) {
                        if(OnLeave) OnLeave(shared);
                    }
                }
            );
        }

        // start accepting
        startAccept();
    });
}
