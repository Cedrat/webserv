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
        void    addSocket(Config config);
        int     getNbOfFd() const;
        fd      findAvailableServerSocket(pollfd *poll_fd, int nfds);

    private :
        bool _is_running;
        Socket _sockets;
};

#endif