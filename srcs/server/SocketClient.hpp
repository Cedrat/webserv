#ifndef SOCKETCLIENT_HPP
#define SOCKETCLIENT_HPP

#include "../includes/utils.hpp"
#include "ASocket.hpp"
#include "../parsing/RequestInProgress.hpp"
#include <vector>
#include "../config/Config.hpp"

class AMethod;
class SocketClient : public ASocket
{
    private : 
        AMethod             *_method;
        RequestInProgress   _request;
        std::vector<Config> _configs;
        pollfd              & _s_pollfd;

    public :
        SocketClient(size_t port, int host, int fd, std::vector<Config> const & config, pollfd &s_pollfd);
        ~SocketClient(){
            if (_method != NULL)
                delete _method;
            }

        void exec();
};

#endif