#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "define.hpp"
class Socket
{
    private :
        bool _server_or_client;
        size_t _port;
        int _timeout;
    
    public : 
        Socket(bool server_or_client, size_t port);
        ~Socket();

        bool    getServerOrClient() const;
        int         getTimeout() const;
        size_t      getPort() const;
        
        void        setTimeout(int);

};
#endif