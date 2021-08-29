#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "define.hpp"
class Socket
{
    private :
        bool _server_or_client;
    
    public : 
        Socket(bool server_or_client);
        ~Socket();

        bool    getServerOrClient() const;
};

#endif