#ifndef MYPOLL_HPP
# define MYPOLL_HPP

#include "fonction.hpp"
#include <poll.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>
#include <unistd.h>

#define TIMEOUT -1
class MyPoll
{
    public :
        MyPoll();
        MyPoll(int nb_fd,fd * array_fd, short events_needed);
        int giveSocketAvailable();
        void closeAllSockets();

    private :
        struct pollfd   *_fds;
        nfds_t          _nfds;
        int             _timeout;
};

/* struct pollfd {
    int   fd;          Descripteur de fichier 
    short events;      Événements attendus    
    short revents;     Événements détectés    
}; */

#endif