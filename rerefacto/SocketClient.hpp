#ifndef SOCKETCLIENT_HPP
#define SOCKETCLIENT_HPP

#include "../includes/utils.hpp"
#include "ASocket.hpp"

class SocketClient : public ASocket
{
    public :
        SocketClient(size_t port, int host);

        void exec();
};

#endif