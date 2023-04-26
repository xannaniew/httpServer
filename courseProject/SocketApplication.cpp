#include "SocketApplication.h"

SocketApplication::SocketApplication(string ip_address, uint16_t port) : m_ip_address(ip_address), m_port(port) {}

void SocketApplication::initialize_address(struct sockaddr_in *socket_address, socklen_t *socket_address_length)
{
    *socket_address_length = sizeof(*socket_address);
    (*socket_address).sin_family = AF_INET;                         // AF_INET == IPv4 интернет протокол
    (*socket_address).sin_port = htons(m_port);                     // устанавливаем номер порта, преобразуя его
                                                                    // из little-endian (узлового формата используемого на данной машине) в big-endian(сетевой стандарт)
    inet_aton(m_ip_address.c_str(), &((*socket_address).sin_addr)); // устанавливаем значение адреса, преобразуя его
                                                                    // из строки(0.0.0.0) в двоичный формат
}

int SocketApplication::initialize_socket(int domain, int type, int protocol) // будет AF_INET, SOCK_STREAM, 0
{
    int m_socket = socket(domain, type, protocol); // AF_INET == IPv4 интернет домен                                     - int domain
                                                   // SOCK_STREAM == потоковый сокет(полный двусторонний обмен данными)  - int type
                                                   // 0 == протокол по умолчанию                                         - int protocol
    if (m_socket == -1)
    {
        throw Socket_error_exception("\nsystem call 'socket' failed to create a socket : status ", errno);
    }

    return m_socket;
}

void SocketApplication::close_socket(int m_socket)
{
    if (close(m_socket) == -1)
    {
        throw Socket_error_exception("\nsystem call 'close' failed to close a socket : status ", errno);
    }
}

ssize_t SocketApplication::read_line(int fd, void *buffer, size_t size)
{
    ssize_t num_read;                // сколько байтов было прочитано
    size_t total_read;               // общее количество прочитанных байтов на этот момент
    char *buf;                       // указатель типа char* для преобразования буфера
    char character;                  // текущий прочитанный символ
    if (size <= 0 || buffer == NULL) // невалидные входные данные
    {
        errno = EINVAL;
        return -1;
    }
    buf = (char *)buffer; // преобразуем void* -> char* для использования арифметики указателей char*
    total_read = 0;
    for (;;)
    {
        num_read = read(fd, &character, 1);
        if (num_read == -1)
        {
            if (errno == EINTR) // прерывание -> перезапускаем read()
                continue;
            else
                return -1; // какая-то другая ошибка
        }
        else if (num_read == 0) // конец файла
        {
            if (total_read == 0) // если ничего не прочитали -> возвращаем 0
                return 0;
            else // если прочитали >0 -> добавляем '\0'
                break;
        }
        else // num_read должно быть = 1
        {
            if (total_read < size - 1) // отклоняем лишние байты: > (n - 1)
            {
                total_read++;
                *buf++ = character;
            }
            if (character == '\n')
                break;
        }
    }
    *buf = '\0';
    return total_read;
}

void SocketApplication::receive_message(int fd, char *buffer, const char *name, struct sockaddr_in socket_address)
{
    ostringstream message_stream;

    ssize_t bytes_received = read_line(fd, buffer, BUFFER_SIZE);
    if (bytes_received == -1)
    {
        throw Socket_error_exception("SocketApplication::receive_message() : system call 'read' failed to retreive data from socket : status ", errno);
    }
    message_stream << name << "[ " << inet_ntoa(socket_address.sin_addr) << ":" << ntohs(socket_address.sin_port) << " ] :";
    message_stream << buffer;
    print(message_stream.str());
}

string SocketApplication::get_m_ip_address()
{
    return this->m_ip_address;
}

uint16_t SocketApplication::get_m_port()
{
    return this->m_port;
}