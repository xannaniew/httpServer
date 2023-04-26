#include "Server.h"

namespace http
{

    Server::Server(string ip_address, uint16_t port) : SocketApplication(ip_address, port), m_socket(), m_client_socket(), /*m_incoming_message{0}*/
                                                       m_server_socket_address(), m_server_socket_address_length(sizeof(m_server_socket_address)),
                                                       m_server_message(), m_client_socket_address(), m_client_socket_address_length()
    {
        initialize_address(&m_server_socket_address, &m_server_socket_address_length);

        start_server();
    }
    Server::~Server()
    {
        close_server();
    }

    void Server::start_server()
    {
        m_socket = initialize_socket(AF_INET, SOCK_STREAM, 0);

        if (bind(m_socket, (sockaddr *)&m_server_socket_address, m_server_socket_address_length) == -1) // назначаем сокету m_socket локальный адрес m_socket_address
                                                                                                        // и передаем длину структуры адреса m_socket_address_length
        {
            throw Socket_error_exception("httpServer::start() : system call 'bind' failed to connect socket to address : status ", errno);
        }
        print("----------------------------------------SERVER STARTED----------------------------------------");

        begin_listen();
    }

    void Server::close_server()
    {
        close_socket(m_socket);
        close_socket(m_client_socket);
        print("----------------------------------------SERVER CLOSED----------------------------------------");
        exit(EXIT_SUCCESS);
    }

    void Server::accept_connection(int m_client_socket)
    {
        m_client_socket = accept(m_socket, (sockaddr *)&m_client_socket_address, &m_client_socket_address_length); // извлекаем из очереди ожидания первый запрос на
                                                                                                                   // подключение и создаем новый подключенный сокет,
                                                                                                                   // присваивая m_client_socket_address и m_client_socket_length
                                                                                                                   // соответствующие значения адреса полученного запроса
        ostringstream message_stream;
        if (m_client_socket == -1 && errno != ECONNABORTED)
        {
            message_stream << "HttpServer::accept_connection(): system call 'accept' failed to receive a request from ADDRESS: ";
            message_stream << inet_ntoa(m_client_socket_address.sin_addr) << " PORT: " << ntohs(m_client_socket_address.sin_port) << " : status ";
            throw Socket_error_exception(message_stream.str(), errno);
        }
        else if (m_client_socket == -1 && errno == ECONNABORTED)
        {
            message_stream << "CLIENT[ " << inet_ntoa(m_client_socket_address.sin_addr) << "] : ";
            message_stream << ntohs(m_client_socket_address.sin_port) << " ] - "
                           << "ABORTED CONNECTION";
            print(message_stream.str());
        }
    }

    void Server::begin_listen()
    {
        if (listen(m_socket, SOMAXCONN) == -1) // помечаем сокет как принимающий запросы входящих соединений с помощью accept
                                               // и определяем максимальный размер очереди ожидающих соединений(QUEUE_SIZE)
        {
            throw Socket_error_exception("httpServer::begin_listen() : system call 'listen' failed to make passive socket : status ", errno);
        }

        ostringstream message_stream;
        message_stream << "----------------------------------SERVER LISTENING ON ADDRESS: " << inet_ntoa(m_server_socket_address.sin_addr);
        // переводим IP адрес из big-endian(стандарт сетевой передачи) в строчный вид из номеров и точек
        message_stream << " PORT: " << ntohs(m_server_socket_address.sin_port);
        // переводим номер порта из формата big-endian(сетевой стандарт) в little-endian(узловой стандарт)

        print(message_stream.str());

        while (true)
        {
            print("----------------------------------------SERVER WAITING FOR CONNECTION----------------------------------------");
            accept_connection(m_client_socket);
            char m_incoming_message[BUFFER_SIZE] = {0};
            receive_message(m_client_socket, m_incoming_message, "CLIENT", m_client_socket_address);

            // close(m_client_socket);
        }
    }
}