#include "SocketServer.hpp"
#include "define.hpp"

SocketServer::SocketServer(size_t port, int host) 
: ASocket(port, host, -1, SERVER)
{

}
