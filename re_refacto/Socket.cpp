#include "Socket.hpp"

Socket::Socket(bool server_or_client) : _server_or_client(server_or_client)
{

}

Socket::~Socket()
{

}

bool Socket::getServerOrClient() const
{
    return(_server_or_client);
}