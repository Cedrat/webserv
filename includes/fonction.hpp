#ifndef FONCTIONS_HPP
# define FONCTIONS_HPP

# include <iostream>
# include <cstring>
# include <cerrno>

# include <sys/types.h>
# include <sys/socket.h>
# include <sys/poll.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <netinet/in.h>

# include <vector>

# define ENDOFARRAY -2

typedef int fd;
typedef int sockfd;

fd give_a_listenable_addr(fd fd_to_be_listened);

#endif