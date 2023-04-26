#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include "exceptions/Socket_error_exception.h"
#include "SocketApplication.h"

using std::ostringstream;
using std::string;

namespace http
{
#define QUEUE_SIZE 10

    class Server : public SocketApplication
    {
    public:
        Server(string ip_address, uint16_t port);

        ~Server();

    private:
        int m_socket;
        int m_client_socket;

        // char m_incoming_message[BUFFER_SIZE];
        string m_server_message;

        struct sockaddr_in m_server_socket_address;
        socklen_t m_server_socket_address_length;
        struct sockaddr_in m_client_socket_address;
        socklen_t m_client_socket_address_length;

        void begin_listen();
        void accept_connection(int client_socket);
        void start_server();
        void close_server();
    };
}
#endif