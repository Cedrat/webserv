#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <vector>
#include <poll.h>
#include "utils.hpp"
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
        void addSocketServer(Config config);
        int         getNbOfSockets() const;
        pollfd *    getSockets() ;
        Config      getConfig(int i) const;
        bool        isAFdServer(fd i) const;
        void        receiveData(fd fd_to_read);
        void        addSocketClient(Config config, fd socket_client);
        void        removeSocket(fd fd_to_read);

};

#endif