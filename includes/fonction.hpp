#ifndef FONCTIONS_HPP
# define FONCTIONS_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <cerrno>
# include <unistd.h>
# include <sys/poll.h>

# include "fcntl.h"
# define ENDOFARRAY -2

typedef int fd;
typedef int sockfd;

fd give_a_listenable_addr(fd fd_to_be_listened);

#endif
