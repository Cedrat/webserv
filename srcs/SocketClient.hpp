#ifndef SOCKETCLIENT_HPP
# define SOCKETCLIENT_HPP

# include <string>
# include "fonction.hpp"
# include <vector>

class SocketClient
{
	public :
		SocketClient(void);
		~SocketClient();
		void addSocketClient(fd sockfd);

		fd 				&operator[](size_t idx);
		const	fd 	&operator[](size_t idx) const;
		void			closeAndRemoveSocket(size_t idx);


	private :
		size_t					_nb_sockets;
		std::vector<fd> _array_of_socket;
};

#endif
