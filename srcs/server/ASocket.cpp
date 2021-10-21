#include "ASocket.hpp"
ASocket::ASocket(size_t port, int host, int timeout, int type) : _port(port), _host(host), _timeout(timeout) , _type(type)
{

}

ASocket::~ASocket()
{

}
int const & ASocket::getTimeout() const
{
    return (_timeout);
}

int const & ASocket::getHost() const
{
    return (_host);
}

size_t const & ASocket::getPort() const
{
    return (_port);
} 

void ASocket::setTimeout(int actual_time)
{
    _timeout = actual_time;
}

int const & ASocket::getType() const
{
    return (_type);
}