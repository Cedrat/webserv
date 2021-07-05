#include "Socket.hpp"
#include "utils.hpp"

Socket::Socket()
{

}

Socket::~Socket()
{

}

pollfd *  Socket::getSockets() 
{
    if (_sockets.size() == 0)
        throw "No Sockets";
    return (&_sockets[0]);
}
static pollfd create_a_listenable_socket(int port)
{
   struct sockaddr_in   my_addr;
   struct pollfd        mypollfd;
   fd  new_socket = socket(AF_INET, SOCK_STREAM, 0);
   
   if (new_socket == -1)
        throw "Error when create socket";

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(new_socket, (struct sockaddr *)&my_addr, sizeof(sockaddr)) == -1)
        throw "Error from binding";
    
    if (listen(new_socket, BACKLOG) == -1)
        throw "Error from listening";
    
    mypollfd.fd = new_socket;
    mypollfd.events = POLLIN;
    mypollfd.revents = 0;

    return (mypollfd);
}

void Socket::addSocketServer(Config config)
{
    pollfd new_socket;

    new_socket = create_a_listenable_socket(config.getPort());
   _sockets.push_back(new_socket);
   _config_socket.push_back(config);
}

bool Socket::isAFdServer(fd fd_to_check) const
{
    for (int i = 0; i < _sockets.size(); i++)
    {
        if (_sockets[i].fd == fd_to_check)
            return (_config_socket[i].getServerOrClient());
    }
    std::cout << "Fd not exist" << std::endl;
    return (CLIENT);
}

int Socket::getNbOfSockets() const
{
    return (_sockets.size());
}

Config Socket::getConfig(int i) const
{
    return (_config_socket[i]);
}

void    Socket::receiveData(fd fd_to_read)
{
    char buffer[128];
    int bytes_read;

    bytes_read = recv(fd_to_read, &buffer, 128, 0);
    if (bytes_read <= 0)
    {
        close(fd_to_read);
        std::cout << "Client closed" << std::endl;
        removeSocket(fd_to_read);
        return ;
    }
    buffer[bytes_read] = 0;
    write(1, &buffer, bytes_read);
}

void    Socket::removeSocket(fd fd_to_read)
{
    std::vector<struct pollfd>::iterator pollfd_itbegin = _sockets.begin();
    std::vector<Config>::iterator config_itbegin = _config_socket.begin();
    for (int i = 0; i < _sockets.size() ; i++)
    {
        if (_sockets[i].fd == fd_to_read)
        {
            _sockets.erase(pollfd_itbegin + i);
            _config_socket.erase(config_itbegin + i);
            return ;
        }
    }
}

void Socket::addSocketClient(Config config, fd socket_client) 
{
    pollfd new_socket;

   new_socket.fd = socket_client;
   new_socket.events = POLLIN;
   new_socket.revents = 0;
   _sockets.push_back(new_socket);
   _config_socket.push_back(config);
}