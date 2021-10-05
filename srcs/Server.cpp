#include "Server.hpp"
#include <poll.h>
#include <vector>
#include "Config.hpp"
#include "../includes/utils.hpp"
#include "../srcs/search_in_vector.hpp"
#include "ASocket.hpp"
#include "SocketServer.hpp"
#include "SocketClient.hpp"
#include "copy_value_of_pointer_vector.hpp"
#include "EOFException.hpp"

pollfd * create_a_listenable_socket(size_t port, int host)
{
   struct sockaddr_in   my_addr;
   struct pollfd *mypollfd = new  pollfd; //think to delete
   fd  new_socket = socket(AF_INET, SOCK_STREAM, 0);

    fcntl(new_socket, F_SETFL, O_NONBLOCK);
    if (new_socket == -1)
        throw  "Error when create socket";
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = host;

    if (bind(new_socket, (struct sockaddr *)&my_addr, sizeof(sockaddr)) == -1)
        throw std::invalid_argument("\nError from binding");

    if (listen(new_socket, BACKLOG) == -1)
        throw std::invalid_argument("\nError from listening");

    //fcntl(new_socket, F_SETFL, O_NONBLOCK);
    mypollfd->fd = new_socket;
    mypollfd->events = POLLIN;
    mypollfd->revents = 0;

    return (mypollfd);
}


Server::Server() : _is_running(FALSE)
{
    _pollfds = new std::vector<pollfd *>;
}
Server::~Server()
{}


void    Server::addSocket(ASocket * socket)
{
    _sockets.push_back(socket);
}

void Server::addPollFd(pollfd * datafd)
{
    _pollfds->push_back(datafd);
}

void    Server::addNewSocket(ASocket *socket, pollfd * datafd)
{
    addSocket(socket);
    addPollFd(datafd);
}

void Server::addConfig(Config config)
{
    _configs.push_back(config);
}

void Server::createAndAddSocketServer(size_t port, int host)
{
    ASocket *socket = new SocketServer(port, host);

    _pollfds->push_back(create_a_listenable_socket(port, host));
    _sockets.push_back(socket);
}

void Server::createSocketsServer(void) 
{
    std::vector<std::string> lists_hosts_ports;
    std::string actual_host;

    for (size_t i = 0; i < _configs.size(); i++)
    {
        actual_host = int_to_string(_configs[i].getHost()) + ":" +  int_to_string(_configs[i].getPort());
        if (search_in_vector(lists_hosts_ports, actual_host) == FALSE)
        {
            lists_hosts_ports.push_back(actual_host);
            createAndAddSocketServer(_configs[i].getPort(), _configs[i].getHost());
            std::cout << "CREATE " << actual_host << std::endl;
        }
    }
}

void Server::acceptConnection(void)
{
    std::vector<pollfd> poll_fd_copy = copy_value_of_pointer_vector(*_pollfds);
    
    char buffer[BUFFER_SIZE];
    int ret = 0;

    poll(poll_fd_copy.data(), poll_fd_copy.size(), 1000);
    for (size_t i = 0; i < poll_fd_copy.size() ; i++)
    {
        (*_pollfds)[i]->revents = poll_fd_copy[i].revents; 
    }
    for (size_t i = 0; i < _sockets.size() && i < poll_fd_copy.size(); i++)
    {
        if (poll_fd_copy[i].revents & POLLHUP || check_timeout(_sockets[i]->getTimeout()))
        {
            removeClient(i);
        }
        else if (poll_fd_copy[i].revents & POLLIN)
        {
            try {
                exec_pollin(_sockets[i], poll_fd_copy[i].fd, *(*_pollfds)[i]);
            }
            catch (EOFException const &e)
            {
                removeClient(i);
            }
        }
        else if (poll_fd_copy[i].revents & POLLOUT)
        {
            exec_pollout(_sockets[i], poll_fd_copy[i].fd, poll_fd_copy[i]);
        }
    }

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
			std::cout << "ERROR CATCH" << str << std::endl;
		}
	} 
 
}

void Server::exec_pollin(ASocket *socket, int fd_request,  pollfd & s_pollfd)
{

    if (socket->getType() == SERVER)
    {
        fd fd_client;
        struct sockaddr_in their_addr;
        socklen_t their_size = sizeof(struct sockaddr_in);
            
        std::cout << socket->getHost() << std::endl;
        fd_client = accept(fd_request, (struct sockaddr *)&their_addr, &their_size);
        
        pollfd *new_poll = new pollfd;
        new_poll->fd = fd_client;
        new_poll->events = POLLIN;
        new_poll->revents = 0;
        
        ASocket *new_socket = new SocketClient(socket->getPort(), socket->getHost(), fd_client, _configs, *new_poll, _pollfds);
        _sockets.push_back(new_socket);


        //Request request(fd_client, socket->getHost(),  socket->getPort());
        _pollfds->push_back(new_poll);
        //_requests.push_back(request);
        std::cout << "New client connected : " << fd_client << std::endl;
    }
    else
    {
        std::cout << "I'm a client " << std::endl;
        std::cout << "Event " << s_pollfd.events << std::endl;
        socket->setTimeout(std::time(0));
        socket->exec();
        // s_pollfd.events = POLLOUT;
        // s_pollfd.revents = 0;
    }
}

void Server::exec_pollout(ASocket *socket, int fd_client, pollfd & s_pollfd)
{
    std::cout << "POLLOUT " << std::endl;
    socket->exec();
    //send(fd_client, "You got a new pokemon\n", 23, 0);
    // s_pollfd.events = POLLIN;
    // s_pollfd.revents = 0;
}

void Server::removeClient(size_t index)
{
    std::cout << "Client " << (*_pollfds)[index]->fd << " disconnected" << std::endl;
    close((*_pollfds)[index]->fd);
    delete (*_pollfds)[index];
    _sockets.erase(_sockets.begin() + index);
    _pollfds->erase(_pollfds->begin() + index);
}