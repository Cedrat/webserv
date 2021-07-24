#include "Server.hpp"
#include "../includes/utils.hpp"

Server::Server(void) : _is_running(false)
{

}

Server::~Server(void)
{

}

void Server::setSockets(Socket sockets)
{
   //_sockets = sockets;
   (void)sockets;
}
bool Server::isRunning(void)
{
    return (_is_running);
}

void Server::launchingServ()
{
	_is_running = TRUE;
	while (_is_running)
	{
		try 
		{
			acceptConnection();
		}
		catch (char const* str)
		{
			std::cout << str << std::endl;
		}
	}
}

fd Server::findAvailableServerSocket(pollfd *poll_fd, int nfds)
{
	std::cout << "Waiting for poll()" << std::endl;
    poll(poll_fd, nfds, -1);
    for (int i = 0; i < nfds;i++)
    {
        if (poll_fd[i].revents != 0)
            return (poll_fd[i].fd);
    }

    throw "No fd available for this request in poll";
}

bool Server::isAFdServer(int fd_to_check)
{
	return (_sockets.isAFdServer(fd_to_check));
}

void Server::acceptConnection()
{
    fd fd_to_accept;
    fd fd_client;
    
	fd_to_accept = findAvailableServerSocket(_sockets.getSockets(), getNbOfFd());

	if (isAFdServer(fd_to_accept) == SERVER)
	{
    	struct sockaddr_in their_addr;
   		socklen_t their_size = sizeof(struct sockaddr_in);
   		fd_client = accept(fd_to_accept, (struct sockaddr *)&their_addr, &their_size);
		
		Config config = _sockets.getConfig(_sockets.getIndexRequest(fd_to_accept));
		config.setServerOrClient(CLIENT);

		addSocketClient(config, fd_client);
		std::cout << "New client connected : " << fd_client << std::endl;
	}
	else if (fd_to_accept > 0) 
	{
		std::cout << "Client " << fd_to_accept << " send a message for you !" << std::endl;
		_sockets.receiveData(fd_to_accept);
	}
    
}

void Server::addSocketServer(Config config)
{
    _sockets.addSocketServer(config);
}

void Server::addSocketClient(Config config,fd fd_client)
{
    _sockets.addSocketClient(config, fd_client);
}

int Server::getNbOfFd() const
{
    return (_sockets.getNbOfSockets());
}