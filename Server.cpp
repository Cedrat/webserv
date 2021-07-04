#include "Server.hpp"
#include "utils.hpp"

Server::Server(void) : _is_running(false)
{

}

Server::~Server(void)
{

}

void Server::setSockets(Socket sockets)
{
   //_sockets = sockets;
}
bool Server::isRunning(void)
{
    return (_is_running);
}

void Server::launchingServ()
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

fd Server::findAvailableServerSocket(pollfd *poll_fd, int nfds)
{
    poll(poll_fd, nfds, -1);
    for (int i = 0; i < nfds;i++)
    {
        if (poll_fd[i].revents != 0 && _sockets.getConfig(i).getServerOrClient() == SERVER)
            return (poll_fd[i].fd);
    }
    throw "No fd available for this request in poll";
}

void Server::acceptConnection()
{
    fd fd_to_accept;
    fd fd_client;

    fd_to_accept = findAvailableServerSocket(_sockets.getSockets(), getNbOfFd());

    struct sockaddr_in their_addr;
    socklen_t their_size = sizeof(struct sockaddr_in);
    fd_client = accept(fd_to_accept, (struct sockaddr *)&their_addr, &their_size);
    
}

void Server::addSocket(Config config)
{
    _sockets.addSocket(config);
}

int Server::getNbOfFd() const
{
    return (_sockets.getNbOfSockets());
}