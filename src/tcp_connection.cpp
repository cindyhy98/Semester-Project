
#include "tcp_connection.h"
#include <iostream>


using boost::asio::ip::tcp;
using net_error = boost::system::error_code;

// init our socket variable to take the io_context
TCPConnection::TCPConnection(tcp::socket&& socket) : _socket(std::move(socket)){
    net_error ec;

    // hold the username
    std::stringstream name;
    name << _socket.remote_endpoint();

    _username = name.str();
}

void TCPConnection::Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler) {
    _messageHandler = std::move(messageHandler);
    _errorHandler = std::move(errorHandler);

    // Start the asyncRead
    asyncRead();

}

void TCPConnection::Post(const std::string& message) {
    // when request to write a message to the client -> keep it in a queue

    bool queueIdle = _outgoingMessage.empty();
    _outgoingMessage.push(message);

    if (queueIdle) {
        asyncWrite();
    }

    // if the queue is not idle -> already transmit it

}

void TCPConnection::asyncRead() {
    io::async_read_until(_socket, _streamBuf, "\n", [self = shared_from_this()] (net_error ec, size_t bytesTransferred) {
        self->onRead(ec, bytesTransferred);
    });
}

void TCPConnection::onRead(net_error ec, size_t bytesTransferred) {
    if (ec) {
        // close the socket when there is an error
        _socket.close(ec);

        // add an error handler
        _errorHandler();
        return;
    }

    std::stringstream message;
    // .rdbuf() is going to consume the bytes on the buffer
    message << std::istream(&_streamBuf).rdbuf();

    // show the message from the client on the screen
    _messageHandler(message.str());
    asyncRead();
}

void TCPConnection::asyncWrite() {
    io::async_write(_socket, io::buffer(_outgoingMessage.front()), [self = shared_from_this()] (net_error ec, size_t bytesTransferred) {
        self->onWrite(ec, bytesTransferred);
    });
}

void TCPConnection::onWrite(net_error ec, size_t bytesTransferred) {
    if (ec) {
        // close the socket when there is an error
        _socket.close(ec);

        // add an error handler
        _errorHandler();
        return;
    }

    _outgoingMessage.pop();

    if (!_outgoingMessage.empty()) {
        asyncWrite();
    }
}
