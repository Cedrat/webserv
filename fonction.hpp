#ifndef FONCTIONS_HPP
# define FONCTIONS_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <poll.h>
#include <cerrno>
#include <unistd.h>
#include "fonction.hpp"
#include "fcntl.h"
# define ENDOFARRAY -2
typedef int fd;
typedef int sockfd;

fd give_a_listenable_addr(fd fd_to_be_listened);

#endif