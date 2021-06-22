#include "myBind.hpp"

myBind::myBind(int port)
{
    _port = htons(port);
    _adress_family = AF_INET;
    _type_of_accepted_binding = INADDR_ANY;
};

void myBind::bindAddr(int fd)
{
    sockaddr_in addr;
    
    addr.sin_family = _adress_family;
    addr.sin_addr.s_addr = _type_of_accepted_binding;
    addr.sin_port = _port;

    bind(fd, (sockaddr *)(&addr), sizeof(addr));
}