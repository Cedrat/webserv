#include "SocketServer.hpp"
#include "../includes/define.hpp"

SocketServer::SocketServer(size_t port, int host) 
: ASocket(port, host, -1, SERVER)
{

}

void SocketServer::exec()
{
    //WIP
}
