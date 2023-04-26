#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "SocketApplication.h"

using std::string;

class Client : public SocketApplication
{
public:
    Client(string server_ip_address, int server_port);
    void start_client();
    void close_client();
    void connect_to_server();
    void send_message(char *buffer);
    void begin_communicate();
    void build_message(char *buffer);
    static bool parse_ipv4_address(string &address);

private:
    int m_socket;
    struct sockaddr_in m_server_socket_address;
    socklen_t m_server_socket_address_length;
};

#endif