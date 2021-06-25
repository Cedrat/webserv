#include "Socket.hpp"

Socket::Socket(void) : _nb_sockets(0)
{

}

Socket::~Socket()
{

}



/*****************
Fonctions membres
*****************/
void bind_addr( fd fd_to_bind, int port, int host )
{
  sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = host;
  addr.sin_port = htons(port);

  if (bind(fd_to_bind, (sockaddr *)(&addr), sizeof(addr)) < 0)
	  throw("ERROR BINDING");
}

void Socket::add_sockets_listening( int port, int host )
{
	fd new_socket;

	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket < 0)
		throw ("ERROR SOCKET CREATION");
    
	bind_addr(new_socket, port, host);
	if (listen(new_socket, BACKLOG) < 0)
		throw ("ERROR LISTENING");

  //Reusable socket
	int enable = 1;
  if ((setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) < 0)
  {
    close(new_socket);
    throw ("setsockopt() failed");
  }

  fcntl(new_socket, F_SETFL, O_NONBLOCK);
	_list_sockets.push_back(new_socket);
	_nb_sockets++;
}

fd* Socket::list_sockets()
{
	fd *fd_array;
	fd_array = new fd[_nb_sockets + 1];
	for (size_t i = 0; i < _nb_sockets; i++)
	{
		fd_array[i] = _list_sockets[i];
	}
	fd_array[_nb_sockets] = ENDOFARRAY;
	return (fd_array);
}
