
#ifndef ACCOUNTABLE_CONFIRMER_SERVER_H
#define ACCOUNTABLE_CONFIRMER_SERVER_H

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "tcp_connection.h"
#include <functional>
#include <optional>
#include <unordered_set>



namespace io = boost::asio;
using boost::asio::ip::tcp;
using net_error = boost::system::error_code;
enum class IPV {
    V4,
    V6
};


class TCPServer {
    // whenever someone join the server -> call OnJoinHandler -> send a connection to it
    using OnJoinHandler = std::function<void(TCPConnection::pointer)>;
    using OnleaveHandler = std::function<void(TCPConnection::pointer)>;
    using OnClientMessageHandler = std::function<void(std::string)>;

public:
    TCPServer(IPV ipv, int port);
    int Run();

    void Stop();
    // When using in ac -> change the message type
    void Broadcast(const std::string& message);
private:
    void startAccept();

public:
    OnJoinHandler OnJoin;
    OnleaveHandler OnLeave;
    OnClientMessageHandler OnClientMessage;

private:
    IPV _ipVersion;
    int _port;

    io::io_context _ioContext;
    tcp::acceptor _acceptor;
    std::optional<tcp::socket> _socket;
    std::unordered_set<TCPConnection::pointer> _connections {};
};



#endif //ACCOUNTABLE_CONFIRMER_SERVER_H
