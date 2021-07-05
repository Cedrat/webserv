#ifndef SERVER_HPP
# define SERVER_HPP

#include "Socket.hpp"
#include "utils.hpp"

class Server
{
    public :
                Server(void);
                ~Server(void);
        bool    isRunning(void);
        void    setSockets(Socket sockets);
        void    launchingServ();
        void    acceptConnection();
        void    addSocketServer(Config config);
        void    addSocketClient(Config config, fd socket_client);
        int     getNbOfFd() const;
        fd      findAvailableServerSocket(pollfd *poll_fd, int nfds);
        bool    thisFdIsServer(fd fd_to_check); 

    private :
        bool _is_running;
        Socket _sockets;
};

#endif