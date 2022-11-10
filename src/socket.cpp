#include "socket.h"

#include <unistd.h>     /* for close() */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* for memset() */
#include <netinet/in.h>
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <sys/select.h>
#include <fcntl.h>      /* for non-blocking */

using namespace std;
namespace socket_t {
    struct Socket s;

    int InitServerSocket(struct Socket* s, int portNumber){

        int ret = 1;
        s->port = portNumber;
        s->addrLen = sizeof(struct sockaddr_in);

        /* Create socket for sending/receiving datagrams */
        s->sock = socket(AF_INET, SOCK_DGRAM, 0);
//        fcntl(s->sock, F_SETFL, O_NONBLOCK);
        if (s->sock < 0) {
            perror("sock error\n");
            return -1;
        }

        memset((void*)&s->socketAddr, 0, s->addrLen);
        s->socketAddr.sin_family = AF_INET;                  /* Internet address family */
        s->socketAddr.sin_addr.s_addr = htons(INADDR_ANY);   /* Any incoming interface */
        s->socketAddr.sin_port = htons(s->port);

        /* Bind the addr to socket -> for receiving other's broadcast */
        ret = bind(s->sock, (struct sockaddr*)&s->socketAddr, s->addrLen);
        if (ret < 0) {
            perror("bind error\n");
            return -1;
        }

        return 1;
    }

    int InitBroadcastSocket(struct Socket* s, int portNumber){
        int broadcastEnable = 1;
        int ret = 1;
        s->port = portNumber;
        s->addrLen = sizeof(struct sockaddr_in);

        /* Create socket for sending/receiving datagrams */
        s->sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (s->sock < 0) {
            perror("sock error\n");
            return -1;
        }

        /* Enable broadcast */
        ret = setsockopt(s->sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable));
        if (ret == -1) {
            perror("setsockopt error");
            close(s->sock);
            return -1;
        }

        memset((void*)&s->socketAddr, 0, s->addrLen);
        s->socketAddr.sin_family = AF_INET;             /* Internet address family */
        s->socketAddr.sin_addr.s_addr = htons(INADDR_BROADCAST);
        s->socketAddr.sin_port = htons(s->port);

        return 1;
    }

    void ReceiveMessageFromOthers(struct Socket* s, char* recvBuffer){
        struct sockaddr_in client_addr;
        fd_set readfd;
        int ret = 0;
        ssize_t count;
//        char recvBuffer[1024];

        // Question: how to make it unblocking?
        while(1) {
            FD_ZERO(&readfd);
            FD_SET(s->sock, &readfd);
            ret = select(s->sock+1, &readfd, NULL, NULL, 0);
            if (ret > 0) {
                if (FD_ISSET(s->sock, &readfd)) {
                    count = recvfrom(s->sock, &recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr*)&client_addr, &s->addrLen);
                    printf("\nClient connection information:\n\t IP: %s, Port: %d\n",
                               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                }
            }
        }
    }

    int SendBroadcastMessage(struct Socket* s, char * sendMessage){
        /*  Return the number of characters sent */
        int ret = sendto(s->sock, sendMessage, strlen(sendMessage), 0,
                         (struct sockaddr*) &s->socketAddr, s->addrLen);

        if (ret != -1) {
            return 1;
        } else {
            return -1;
        }
    }
}