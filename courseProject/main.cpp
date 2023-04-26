#include "Server.h"

using namespace http;

int main()
{
    Server server = Server("127.0.0.1", 8080); // 127.0.0.1 == INADDR_LOOPBACK - для тестирования
                                               // 192.168.0.101 == ip пк == ip сервера в сети
                                               // 8080
                                               // 40000

    return 0;
}