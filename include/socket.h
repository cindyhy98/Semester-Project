#ifndef ACCOUNTABLE_CONFIRMER_BROADCAST_H
#define ACCOUNTABLE_CONFIRMER_BROADCAST_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;
namespace socket_t {
    struct Socket {
        /* socketAddr can be either server address (receiving others message) or broadcast address (sending broadcast to others)*/
        struct sockaddr_in socketAddr;
        int port;
        unsigned int addrLen;
        int sock;
    };

    // return 1 if success; else fail
    int InitServerSocket(struct Socket *s, int portNumber);

    // return 1 if success; else fail
    int InitBroadcastSocket(struct Socket *s, int portNumber);

    void ReceiveMessageFromOthers(struct Socket *s);

    // return 1 if success; else fail??
    int SendBroadcastMessage(struct Socket *s, char *sendString);
}

#endif //ACCOUNTABLE_CONFIRMER_BROADCAST_H
