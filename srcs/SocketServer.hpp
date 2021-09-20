#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include "../includes/utils.hpp"
#include "ASocket.hpp"

class SocketServer : public ASocket
{
    public :
        SocketServer(size_t port, int host);
        
        void exec();
};

#endif