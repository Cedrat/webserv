#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <string>
#include <list>
#include <vector>
#include <iostream>

typedef int fd;
#define ENDOFARRAY -2

class Socket
{
	public : 
		Socket(void);
		~Socket();

		void	add_sockets_binded(int ports);
		fd* 	list_sockets();

	private :
		std::vector<fd> 	_list_sockets;
		int				_nb_sockets;
};

#endif
