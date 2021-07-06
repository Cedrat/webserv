#ifndef SERVER_HPP
# define SERVER_HPP

#include "Socket.hpp"
#include "../includes/utils.hpp"

class Server
{
    private :
        bool _is_running;
        Socket _sockets;
   
    public :
                Server(void);
                ~Server(void);
        
        void    setSockets(Socket sockets);
        
        int     getNbOfFd() const;

        bool    isRunning(void);
        
        void    addSocketServer(Config config);
        void    addSocketClient(Config config, fd socket_client);
        
        void    launchingServ();
        void    acceptConnection();
        fd      findAvailableServerSocket(pollfd *poll_fd, int nfds);
        bool    isAFdServer(fd fd_to_check); 

};

#endif