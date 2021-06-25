#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "../includes/fonction.hpp"

# define BACKLOG 10

class Socket
{
	public :
		Socket(void);
		~Socket();

		void	addSocketsListening( int port, int host );
		fd* 	listSockets();

	private :
		std::vector<fd> 	_list_sockets;
		int					_nb_sockets;
};

#endif
