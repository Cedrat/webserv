#include "Request.hpp"

Request::Request(fd fd_request) : _fd(fd_request)
{

}

Request::~Request()
{

}

fd const & Request::getFd() const
{
    return (_fd);
}

void        Request::addRequest(std::string buffer)
{
    _request += buffer;
}