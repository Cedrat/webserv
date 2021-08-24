#include "Server.hpp"
#include "../includes/utils.hpp"
#include "ResponseHTTP.hpp"
#include "cstdlib"

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
			acceptConnection(_sockets.getSockets(), getNbOfFd());
		}
		catch (char const* str)
		{
			std::cout << str << std::endl;
		}
	}
}

void	Server::acceptConnection(std::vector<struct pollfd> pollfd, int nfds)
{
	std::cout << "Waiting for poll()" << std::endl;
	struct pollfd* poll_fd = pollfd.data();
    poll(poll_fd, nfds, -1);
    for (int i = 0; i < nfds;i++)
    {
        if (poll_fd[i].revents != 0 && isAFdServer(poll_fd[i].fd) == SERVER)
            addClient(poll_fd[i].fd);
		else if (poll_fd[i].revents & POLLHUP && isAFdServer(poll_fd[i].fd) == CLIENT)
		{
			close(poll_fd[i].fd);
			_sockets.removeSocket(poll_fd[i].fd);
			nfds--;
			std::cout << "Client closed" << std::endl;
		}
		else if (poll_fd[i].revents & POLLOUT && isAFdServer(poll_fd[i].fd) == CLIENT)
		{
			std::cout << "send Data" << std::endl;
			sendData(poll_fd[i].fd);

			if (_sockets.getRequest(poll_fd[i].fd).getError() == 400)
			{
					close(poll_fd[i].fd);
				_sockets.removeSocket(poll_fd[i].fd);
				nfds--;
				std::cout << "Client closed" << std::endl;
			}
			else if (_sockets.getRequest(poll_fd[i].fd).getSendingData() == TRUE)
				std::cout << "transfer in progress" << std::endl;
			else
			{
				_sockets.resetRequest(poll_fd[i].fd);
				poll_fd[i].events = POLLIN;
				poll_fd[i].revents = 0;
			}
		}
		else if (poll_fd[i].revents & POLLIN && isAFdServer(poll_fd[i].fd) == CLIENT)
		{
			receiveData(poll_fd[i].fd);
			std::cout << "re" << std::endl;
		}
    }
}

void	Server::receiveData(fd fd_client)
{
	_sockets.receiveData(fd_client);
}

void	Server::sendData(fd fd_client)
{
	Request&  request = _sockets.getRefRequest(fd_client);
	Config	config = _sockets.getConfigByFd(fd_client);

	struct stat sb;
	std::string path = create_path(request, config);
	std::cout << request.getPathFileRequest() << std::endl;
	std::cout << "path required " << path << std::endl;
	if (request.getSendingData() == FALSE)
	{
		if (stat(path.c_str(), &sb) == -1 || (S_ISREG(sb.st_mode) == 0 && S_ISDIR(sb.st_mode) == 0))
		{
			response_error_header(404, config, fd_client);
		}
		else if (request.getError() == OK && request.getMethod() == "GET")
		{
			response_good_file(path, fd_client, 0);
			request.setPathFileAnswer(path.c_str());
			request.setSendingData(TRUE);
			request.setFinished(FALSE);
			request.setFdAnswer(fd_client);
		}
		else if (request.getError() == OK && request.getMethod() == "DELETE")
		{
			delete_and_give_response(path.c_str(), fd_client);
			std::cout << "DELETE COMPLETED" << std::endl;
		}
		else
			response_error_header(request.getError(), config, fd_client);
	}
	else
	{
		std::cout << "blop" << std::endl;
		//std::cout << "ANANAS " << request.getResponseHTTP().getPath() << fd_client << std::endl;
		request.send();
		if (request.getResponseHTTP().getFinished() == TRUE)
		{
			request.setSendingData(FALSE);
			request.resetByteSend();
		}
	}
}

bool Server::isAFdServer(int fd_to_check)
{
	return (_sockets.isAFdServer(fd_to_check));
}

// void Server::acceptConnection()
// {
//     fd fd_to_accept;
//     fd fd_client;

// 	fd_to_accept = findAvailableServerSocket(_sockets.getSockets(), getNbOfFd());

// 	if (isAFdServer(fd_to_accept) == SERVER)
// 	{
//     	struct sockaddr_in their_addr;
//    		socklen_t their_size = sizeof(struct sockaddr_in);
//    		fd_client = accept(fd_to_accept, (struct sockaddr *)&their_addr, &their_size);

// 		Config config = _sockets.getConfig(_sockets.getIndexRequest(fd_to_accept));
// 		config.setServerOrClient(CLIENT);

// 		addSocketClient(config, fd_client);
// 		std::cout << "New client connected : " << fd_client << std::endl;
// 	}
// 	else if (fd_to_accept > 0)
// 	{
// 		std::cout << "Client " << fd_to_accept << " send a message for you !" << std::endl;
// 		_sockets.receiveData(fd_to_accept);
// 	}

// }

void Server::addSocketServer(Config & config)
{
    _sockets.addSocketServer(config);
}

void Server::addSocketClient(Config & config,fd fd_client)
{
    _sockets.addSocketClient(config, fd_client);
}

int Server::getNbOfFd() const
{
    return (_sockets.getNbOfSockets());
}

void Server::addClient(fd new_fd_client)
{
		fd fd_client;

		struct sockaddr_in their_addr;
   		socklen_t their_size = sizeof(struct sockaddr_in);
   		fd_client = accept(new_fd_client, (struct sockaddr *)&their_addr, &their_size);

		Config * config = new Config;
		*config = _sockets.getConfig(_sockets.getIndexRequest(new_fd_client));
		config->setServerOrClient(CLIENT);

		addSocketClient(*config, fd_client);
		std::cout << "New client connected : " << fd_client << std::endl;
}
