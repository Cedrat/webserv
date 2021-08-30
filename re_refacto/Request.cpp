#include "Request.hpp"
#include "../includes/utils.hpp"

Request::Request(fd fd_request, size_t port) : _fd(fd_request), _port(port)
{
    _request_completed = FALSE;
}

Request::~Request()
{

}

fd const & Request::getFd() const
{
    return (_fd);
}

size_t const & Request::getPort() const
{
    return (_port);
}

void        Request::addRequest(std::string buffer)
{
    _request += buffer;
}

bool const &    Request::getRequestCompleted() const
{
    return (_request_completed);
}

void Request::receiveData()
{
    char buffer[BUFFER_SIZE];
    std::string str_request;
    int ret;
    
    ret = read(getFd(), buffer, BUFFER_SIZE);
    if (ret < 0)
        std::cout << "read error" << std::endl;
    buffer[ret] = 0;
    str_request = buffer;
    addRequest(str_request);

    if (_request.find("\r\n\r\n") != std::string::npos)
        _request_completed = TRUE;
}