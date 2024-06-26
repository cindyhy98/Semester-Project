#include "tcp_client.h"


namespace io = boost::asio;
using boost::asio::ip::tcp;
using net_error = boost::system::error_code;


//TCPClient::TCPClient(const std::string &address, int port) : _socket(_ioContext), _workGuard(_ioContext.get_executor()){
//    tcp::resolver resolver(_ioContext);
//    _endpoints = resolver.resolve(address, std::to_string(port));
//}

TCPClient::TCPClient() {
    _socket = std::make_shared<tcp::socket>(_ioContext);
}


void TCPClient::Init(const std::string &address, int port) {
    tcp::resolver resolver(_ioContext);
    _endpoints = resolver.resolve(address, std::to_string(port));
}

void TCPClient::Run() {

    // connect to the endpoint
    io::async_connect(*_socket, _endpoints, [this](net_error ec, tcp::endpoint ep) {
        if (!ec) {
            asyncRead();
        } else {
            std::cout << "Error: " << ec.what() << std::endl;
        }
    });

    _ioContext.run();
}

void TCPClient::Stop() {
    net_error ec;

    // close the socket
    _socket->close(ec);

    if (ec) {
        //process error
    }

}

void TCPClient::Post(const std::string& message) {
    // when request to write a message to the client ->
    // 1. keep it in a queue

    bool queueIdle = _outgoingMessage.empty();
    _outgoingMessage.push(message);

    if (queueIdle) {
        asyncWrite();
    }

    // if the queue is not idle -> already transmit it
}

void TCPClient::asyncRead() {
    io::async_read_until(*_socket, _streamBuf, '\n',[this] (net_error ec, size_t bytesTransferred) {
        onRead(ec, bytesTransferred);
    });
}

void TCPClient::onRead(net_error ec, size_t bytesTransferred) {
    if (ec) {
        // close the socket when there is an error

//        std::cout << "error in client read = " << ec.what() <<std::endl;
        Stop();
        return;
    }

    std::stringstream message;
    // .rdbuf() is going to consume the bytes on the buffer
    message  << std::istream(&_streamBuf).rdbuf();
    _streamBuf.consume(bytesTransferred);
    OnMessage(message.str());
    asyncRead();
}

// Post() call these two
void TCPClient::asyncWrite(){
    io::async_write(*_socket, io::buffer(_outgoingMessage.front()), [this] (net_error ec, size_t bytesTransferred) {
        onWrite(ec, bytesTransferred);
    });
}
void TCPClient::onWrite(net_error ec, size_t bytesTransferred){
    if (ec) {
        // close the socket when there is an error
        std::cout << "error in client write = " << ec.what() << std::endl;
        Stop();
        return;
    }

    _outgoingMessage.pop();

    if (!_outgoingMessage.empty()) {
        asyncWrite();
    }
}
