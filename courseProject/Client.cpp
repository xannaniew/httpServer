#include "Client.h"
#include "exceptions/Socket_error_exception.h"

Client::Client(string server_ip_address, int server_port) : SocketApplication(server_ip_address, server_port), m_socket(),
                                                            m_server_socket_address(), m_server_socket_address_length(sizeof(m_server_socket_address))
{
    initialize_address(&m_server_socket_address, &m_server_socket_address_length);

    start_client();
}

void Client::start_client()
{
    m_socket = initialize_socket(AF_INET, SOCK_STREAM, 0);

    print("----------------------------------------CLIENT STARTED----------------------------------------");

    connect_to_server();
}

void Client::close_client()
{
    close_socket(m_socket);
}

void Client::begin_communicate()
{
    while (true)
    {
        char buffer[BUFFER_SIZE];
        build_message(buffer);
        send_message(buffer);
        receive_message(m_socket, buffer, "SERVER", m_server_socket_address);
    }
}

void Client::send_message(char *buffer)
{
    char buffer[BUFFER_SIZE];
    print("Input message: ");
    scanf("%s", buffer);

    ssize_t bytes_written = write(m_socket, buffer, BUFFER_SIZE);
    if (bytes_written == -1)
    {
        throw Socket_error_exception("httpServer::begin_listen() : system call 'read' failed to retreive data from socket : status ", errno);
    }
}

void Client::build_message(char *buffer)
{
}

void Client::connect_to_server()
{
    if (connect(m_socket, (sockaddr *)&m_server_socket_address, m_server_socket_address_length) == -1)
    {
        throw Socket_error_exception("\nsystem call 'connect' failed to connect to a socket : status ", errno);
    }
}

bool Client::parse_ipv4_address(string &address)
{
    int num_dots, num_digits, current_number = 0;

    for (char character : address)
    {
        if (character == '.')
        {
            num_dots++;
            if (num_digits == 0 || num_digits > 3 || current_number > 255) // если число перед . больше 3-х значного / 255 или числа перед . нет -> ошибка
            {
                return false;
            }

            num_digits = current_number = 0;
        }
        else if (character >= '0' && character <= '9')
        {
            num_digits++;
            current_number += current_number * 10 + (character - '0');

            if (num_digits > 1 && current_number == 0) // проверка на ведущие нули:192.168.01.1 - недопустим, т к в десятичной записи должно быть 192.168.1.1
                                                       // при этом 192.168.1.0 - допустимо (num_digits == 1)
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        if (num_dots != 3 || num_digits == 0 || num_digits > 3 || current_number > 255) // проверка для последнего сегмента
        {
            return false;
        }

        return true;
    }
}