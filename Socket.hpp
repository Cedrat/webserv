#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <vector>
#include <poll.h>
#include "Config.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
class Socket
{
    private :
        std::vector<pollfd> _sockets;
        std::vector<Config> _config_socket;

    public :
        Socket();
        ~Socket();
        void addSocket(Config config);

};

#endif