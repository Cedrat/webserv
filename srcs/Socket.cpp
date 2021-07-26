#include "Socket.hpp"
#include "../includes/utils.hpp"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


Socket::Socket()
{

}

Socket::~Socket()
{

}

pollfd * Socket::getSockets() 
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
    Request new_request;

    new_socket = create_a_listenable_socket(config.getPort());
   _sockets.push_back(new_socket);
   _config_socket.push_back(config);
   _requests.push_back(new_request);
}

bool Socket::isAFdServer(fd fd_to_check) const
{
    for (size_t i = 0; i < _sockets.size(); i++)
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
int Socket::getIndexRequest(fd fd_to_request)
{
    for (size_t i = 0; i < _requests.size() ; i++)
    {
        if (_sockets[i].fd == fd_to_request)
        {
            return (i);
        }
    }
    return (-1);
}
void    Socket::receiveData(fd fd_to_read)
{
    char buffer[BUFFER_SIZE + 1];
    int bytes_read;
    std::string request;
    bytes_read = recv(fd_to_read, &buffer, BUFFER_SIZE, 0);
    buffer[bytes_read] = 0;
    request = buffer;
    while (bytes_read == BUFFER_SIZE)
    {
        bytes_read = recv(fd_to_read, &buffer, BUFFER_SIZE, 0);
        buffer[bytes_read] = 0;
        request += buffer;
        std::cout << "looooop" << std::endl;
    }
    _requests[getIndexRequest(fd_to_read)].addToRequestHeader(request);
    std::cout << "Nb of bytes read : " << request.size() << std::endl;
    std::cout << request << std::endl;
    if (_requests[getIndexRequest(fd_to_read)].getError() == NOT_SUPPORTED)
    {
        response_error_header(505, getConfig(getIndexRequest(fd_to_read)), fd_to_read);
        close(fd_to_read);
        std::cout << "Client closed" << std::endl;
        removeSocket(fd_to_read);
        return ;
    }
    else if (_requests[getIndexRequest(fd_to_read)].getError() == BAD_REQUEST)
    {
        response_error_header(400, getConfig(getIndexRequest(fd_to_read)), fd_to_read); 
        close(fd_to_read);
        std::cout << "Client closed" << std::endl;
        removeSocket(fd_to_read);
        return ;
    }
    else
    {
        std::fstream fs;
        struct stat sb;
        std::string path = "./www" + _requests[getIndexRequest(fd_to_read)].getPathFileRequest();
        if (path == "./www/")
            path += "/index.html";

        std::cout << path << std::endl;
        if (stat(path.c_str(), &sb) == -1)
        {
            response_error_header(404, getConfig(getIndexRequest(fd_to_read)), fd_to_read);
        }
        else
        {
            response_good_file(_requests[getIndexRequest(fd_to_read)].getPathFileRequest(), fd_to_read);
        }
        close(fd_to_read);
        // // std::cout << "Client closed" << std::endl;
        removeSocket(fd_to_read);
        return ;
    }
    if (bytes_read <= 0 || request == "\r\n")
    {
        close(fd_to_read);
        std::cout << "Client closed" << std::endl;
        removeSocket(fd_to_read);
        return ;
    }
    write(1, request.c_str(), request.size());
}

void    Socket::removeSocket(fd fd_to_read)
{
    std::vector<struct pollfd>::iterator pollfd_itbegin = _sockets.begin();
    std::vector<Config>::iterator config_itbegin = _config_socket.begin();
    for (size_t i = 0; i < _sockets.size() ; i++)
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
    Request new_request;

   new_socket.fd = socket_client;
   new_socket.events = POLLIN;
   new_socket.revents = 0;
   _sockets.push_back(new_socket);
   _config_socket.push_back(config);
   _requests.push_back(new_request);
}