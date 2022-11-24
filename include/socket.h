#ifndef ACCOUNTABLE_CONFIRMER_BROADCAST_H
#define ACCOUNTABLE_CONFIRMER_BROADCAST_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;
namespace socket_t {
    struct Socket {
        /* socketAddr can be either server address (receiving messages) or broadcast address (broadcast to others)*/
        struct sockaddr_in socketAddr;
        int port;
        unsigned int addrLen;
        int sock;
    };

    // return 1 if success; else fail
    int InitServerSocket(struct Socket *s, int portNumber);

    // return 1 if success; else fail
    int InitBroadcastSocket(struct Socket *s, int portNumber);

    void ReceiveMessage(struct Socket *s, char* recvBuffer);

    // return 1 if success; else fail??
    int BroadcastMessage(struct Socket *s, char *sendMessage);

    int SetNonblocking(int fd);
}

#endif //ACCOUNTABLE_CONFIRMER_BROADCAST_H
