#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <vector>
#include <poll.h>
#include "../includes/utils.hpp"
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

        void        addSocketServer(Config config);
        void        addSocketClient(Config config, fd socket_client);
        
        int         getNbOfSockets() const;
        pollfd *    getSockets() ;
        Config      getConfig(int index_of_socket_needed) const;
        
        void        removeSocket(fd fd_to_remove);

        bool        isAFdServer(fd i) const;
       
        void        receiveData(fd fd_to_read); 
};

#endif