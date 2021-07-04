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
        std::vector<struct pollfd> _sockets;
        std::vector<Config> _config_socket;

    public :
        Socket();
        ~Socket();
        void addSocket(Config config);
        int         getNbOfSockets() const;
        pollfd *    getSockets() ;
        Config      getConfig(int i) const;

};

#endif