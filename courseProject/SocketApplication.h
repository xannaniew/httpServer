#ifndef SOCKETAPPLICATION_H
#define SOCKETAPPLICATION_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include "exceptions/Socket_error_exception.h"

#define BUFFER_SIZE 1024

using std::cout;
using std::endl;
using std::ostringstream;
using std::string;

void print(const string message)
{
    cout << message << endl;
}

class SocketApplication
{
public:
    SocketApplication(string ip_address, uint16_t port);
    int initialize_socket(int domain, int type, int protocol);
    void initialize_address(struct sockaddr_in *socket_address, socklen_t *socket_address_length);
    void close_socket(int m_socket);
    ssize_t read_line(int fd, void *buffer, size_t size);
    string get_m_ip_address();
    uint16_t get_m_port();
    void receive_message(int fd, char *buffer, const char *name, struct sockaddr_in socket_address);

private:
    string m_ip_address;
    uint16_t m_port;
};

#endif