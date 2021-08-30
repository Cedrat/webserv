#include "../includes/utils.hpp"
#include "Socket.hpp"


void timeout(Socket & socket)
{
    if (socket.getServerOrClient() == SERVER)
        return ;
    
    if (std::time(0) - socket.getTimeout() > TIMEOUT)
    {
       socket.setTimeout(-1);
       std::cout << "TIMEOUUUTTT" << std::endl; 
    }
}