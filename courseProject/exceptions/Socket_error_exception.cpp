#include "Socket_error_exception.h"

Socket_error_exception::Socket_error_exception(string message, int error_code)
{
    this->message = message;
    this->error_code = error_code;
    message.append(to_string(error_code));
}

Socket_error_exception::~Socket_error_exception() = default;

int Socket_error_exception::status() const noexcept
{
    return error_code;
}

const char *Socket_error_exception::what() const noexcept
{
    return message.c_str();
}