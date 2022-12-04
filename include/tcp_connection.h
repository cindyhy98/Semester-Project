
#ifndef ACCOUNTABLE_CONFIRMER_TCP_CONNECTION_H
#define ACCOUNTABLE_CONFIRMER_TCP_CONNECTION_H

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <queue>



namespace io = boost::asio;
using boost::asio::ip::tcp;
using net_error = boost::system::error_code;
using MessageHandler = std::function<void(std::string)>;
using ErrorHandler = std::function<void()>;
// a connection is just a better socket
class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
public:
    // create a shared ptr within yourself pointing to yourself
    using pointer = std::shared_ptr<TCPConnection>;
    static pointer Create(tcp::socket&& socket) {
        return pointer(new TCPConnection(std::move(socket)));
    }

    inline const std::string Getusername() const { return _username; };

    tcp::socket& Socket() {
        return _socket;
    }

    // start the connection and write the message
    // void Start(/* ARGUMENTS -- CALLBACKS */)
    void Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler);

    // post message to client
    void Post(const std::string& message);
private:
    explicit TCPConnection(tcp::socket&& socket);

    // Start() call these two
    // wait for a new message from client
    void asyncRead();
    // when asyncRead complete
    void onRead(net_error ec, size_t bytesTransferred);

    // Post() call these two
    void asyncWrite();
    void onWrite(net_error ec, size_t bytesTransferred);

private:
    tcp::socket _socket;
    std::string _message;
    std::string _username;

    std::queue<std::string> _outgoingMessage {};

    // for read and write
    io::streambuf _streamBuf {65536};

    MessageHandler _messageHandler;
    ErrorHandler _errorHandler;
};




#endif //ACCOUNTABLE_CONFIRMER_TCP_CONNECTION_H
