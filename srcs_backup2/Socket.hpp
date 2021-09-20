#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "define.hpp"
class Socket
{
    private :
        bool _server_or_client;
        size_t _port;
        int _host;
        int _timeout;
    
    public : 
        Socket(bool server_or_client, size_t port, int host);
        ~Socket();

        bool    getServerOrClient() const;
        int         getTimeout() const;
        size_t      getPort() const;
        int const &      getHost() const;
        
        void        setTimeout(int);
};
#endif