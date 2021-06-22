#ifndef MYBIND_HPP
# define MYBIND_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include "myPoll.hpp"

class myBind
{
    public : 
        myBind(int port);
        void bindAddr(int fd);
    private : 
        sa_family_t _adress_family;
        in_addr_t     _type_of_accepted_binding;
        in_port_t   _port;
};

#endif