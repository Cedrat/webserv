#include "Socket.hpp"
//#include "fonction.hpp"
Socket::Socket(void) : _nb_sockets(0)
{

}


Socket::~Socket()
{

}
void bind_addr(fd fd_to_bind, int port)
{
    sockaddr_in addr;

    addr.sin_family = AF_INET;
	//A remplacer par host fourni
	//Ajouter un int host a add_socket_listening et l'utiliser ici
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(fd_to_bind, (sockaddr *)(&addr), sizeof(addr)) < 0)
		throw("ERROR BINDING");
}

void Socket::add_sockets_listening(int port)
{
	fd new_socket;

	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket < 0)
		throw ("ERROR SOCKET CREATION");
	bind_addr(new_socket, port);
	if (listen(new_socket, BACKLOG) < 0)
		throw ("ERROR LISTENING");
	//Socket non bloquant
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


    /*
    Permettre au socket de reutiliser l'addresse
	Mis ici au cas ou on lui trouve une utilité
    */
   /* int option = 1;
    if ((setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option))) < 0)
    {
      std::cerr << "setsockopt() failed" << std::endl;
      close(this->_socket);
      return (-1);
    }*/