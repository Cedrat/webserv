#include "Server.hpp"
#include "search_in_vector.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "Request.hpp"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>

pollfd create_a_listenable_socket(size_t port)
{
   struct sockaddr_in   my_addr;
   struct pollfd        mypollfd;
   fd  new_socket = socket(AF_INET, SOCK_STREAM, 0);

    fcntl(new_socket, F_SETFL, O_NONBLOCK | SO_REUSEADDR);
    if (new_socket == -1)
        throw "Error when create socket";

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(new_socket, (struct sockaddr *)&my_addr, sizeof(sockaddr)) == -1)
        throw "Error from binding";

    if (listen(new_socket, BACKLOG) == -1)
        throw "Error from listening";

    //fcntl(new_socket, F_SETFL, O_NONBLOCK);
    mypollfd.fd = new_socket;
    mypollfd.events = POLLIN;
    mypollfd.revents = 0;

    return (mypollfd);
}

Server::Server() : _is_running(FALSE)
{

}

Server::~Server()
{

}

void Server::createSocketsServer(void) 
{
    std::vector<size_t> lists_ports;

    for (size_t i = 0; i < _configs.size(); i++)
    {
        if (search_in_vector(lists_ports, _configs[i].getPort()) == FALSE)
        {
            lists_ports.push_back(_configs[i].getPort());
            createAndAddSocketServer(_configs[i].getPort());
            std::cout << "CREATE" << std::endl;
        }
    }
}

void Server::createAndAddSocketServer(size_t port)
{
    Socket socket(SERVER);

    _poll_fds.push_back(create_a_listenable_socket(port));
    _sockets.push_back(socket);
}

void Server::addConfig(Config const & config)
{
    _configs.push_back(config);
}

void Server::launchingServer(void)
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

void Server::acceptConnection(void)
{
    struct pollfd* poll_fd = _poll_fds.data();
    char buffer[BUFFER_SIZE];
    int ret;
    std::cout << "waiting for poll()" << std::endl;

    poll(poll_fd, _poll_fds.size(), -1);
    for (size_t i = 0; i < _poll_fds.size(); i++)
    {
        if (_poll_fds[i].revents != 0 && _sockets[i].getServerOrClient() == SERVER)
        {
            createAndAddSocketClient(_poll_fds[i].fd);
        }
        else if (_poll_fds[i].revents & POLLHUP)
		{
			close(_poll_fds[i].fd);
			removeSocket(i);
			std::cout << "Client disconnected by ragequit" << std::endl;
		}
        else if (_poll_fds[i].revents & POLLIN && _sockets[i].getServerOrClient() == CLIENT)
		{
            ret = read(_poll_fds[i].fd, buffer, BUFFER_SIZE);
            write(1, buffer, ret);
		}
    }
}

void Server::createAndAddSocketClient(fd new_fd_client)
{
		fd fd_client;
		struct sockaddr_in their_addr;
   		socklen_t their_size = sizeof(struct sockaddr_in);
        Socket socket(CLIENT);
        pollfd new_socket;


        
        _sockets.push_back(socket);
   		fd_client = accept(new_fd_client, (struct sockaddr *)&their_addr, &their_size);

		new_socket.fd = fd_client;
        new_socket.events = POLLIN;
        new_socket.revents = 0;

        _poll_fds.push_back(new_socket);
		std::cout << "New client connected : " << fd_client << std::endl;
}

void Server::removeSocket(size_t index)
{
    _sockets.erase(_sockets.begin() + index);
    _poll_fds.erase(_poll_fds.begin() + index);
}