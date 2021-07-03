#include "Server.hpp"

Server::Server(void) : _is_running(false)
{

}

Server::~Server(void)
{

}

void Server::setSockets(Socket sockets)
{
   // _sockets = sockets;
}
bool Server::isRunning(void)
{
    return (_is_running);
}


