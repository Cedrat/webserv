#ifndef SERVER_HPP
# define SERVER_HPP

#include "Socket.hpp"

class Server
{
    public :
                Server(void);
                ~Server(void);
        bool    isRunning(void);
        void    setSockets(Socket sockets);

    private :
        bool _is_running;
        //Socket _sockets;
};

#endif