#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <string>
#include <list>
#include <vector>
#include <iostream>

# include <sys/types.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/poll.h>
# include <unistd.h>
# include <cstring>
# include <cerrno>

typedef int fd;
#define ENDOFARRAY -2
#define BACKLOG 10

class Socket
{
	public : 
		Socket(void);
		~Socket();

		void	add_sockets_listening(int ports);
		fd* 	list_sockets();

	private :
		std::vector<fd> 	_list_sockets;
		int				_nb_sockets;
};

#endif
