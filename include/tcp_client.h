
#ifndef ACCOUNTABLE_CONFIRMER_TCP_CLIENT_H
#define ACCOUNTABLE_CONFIRMER_TCP_CLIENT_H

#include <boost/asio.hpp>
#include <iostream>
#include <queue>


namespace io = boost::asio;
using boost::asio::ip::tcp;
using net_error = boost::system::error_code;
using MessageHandler = std::function<void(std::string)>;

class TCPClient {

public:
    // create a shared ptr within yourself pointing to yourself
//    using pointer = std::shared_ptr<TCPClient>;
//    static pointer Create() {
//        return pointer(new TCPClient());
//    }
//
//    tcp::socket& Socket() {
//        return _socket;
//    }


    TCPClient(const std::string& address, int port);

    void Init(const std::string &address, int port);
    void Run();
    void Stop();
    void Post(const std::string& message);
private:
//    TCPClient();
    // Start() call these two
    // wait for a new message from server
    void asyncRead();
    // when asyncRead complete
    void onRead(net_error ec, size_t bytesTransferred);

    // Post() call these two
    void asyncWrite();
    void onWrite(net_error ec, size_t bytesTransferred);
public:
    MessageHandler OnMessage;
private:
    io::io_context _ioContext;
    tcp::socket _socket ;
    tcp::resolver::results_type _endpoints;
    io::streambuf _streamBuf{65536};
//    io::streambuf& _streamBuf;
    std::queue<std::string> _outgoingMessage {};

};




#endif //ACCOUNTABLE_CONFIRMER_TCP_CLIENT_H
