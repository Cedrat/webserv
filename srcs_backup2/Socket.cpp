#include "Socket.hpp"
#include "../includes/utils.hpp"

Socket::Socket(bool server_or_client, size_t port, int host) : 
_server_or_client(server_or_client), _port(port), _host(host)
{
    _timeout = std::time(0);
}

Socket::~Socket()
{

}

bool Socket::getServerOrClient() const
{
    return(_server_or_client);
}

size_t Socket::getPort() const
{
    return (_port);
}
int const & Socket::getHost() const
{
    return (_host);
}


int Socket::getTimeout() const
{
    return (_timeout);
}

void  Socket::setTimeout(int time) 
{
    _timeout = time;
}