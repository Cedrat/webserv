#include "../includes/utils.hpp"
#include "Socket.hpp"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

Socket::Socket()
{

}

Socket::~Socket()
{

}

std::vector<struct pollfd> Socket::getSockets()
{
    if (_sockets.size() == 0)
        throw "No Sockets";
    return (_sockets);
}

std::vector<struct pollfd> & Socket::getRefSockets()
{
    if (_sockets.size() == 0)
        throw "No Sockets";
    return (_sockets);
}
pollfd create_a_listenable_socket(int port)
{
   struct sockaddr_in   my_addr;
   struct pollfd        mypollfd;
   fd  new_socket = socket(AF_INET, SOCK_STREAM, 0);

    fcntl(new_socket, F_SETFL, O_NONBLOCK, SO_REUSEADDR);
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

void Socket::addSocketServer(Config & config)
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

Config Socket::getConfigByFd(fd current_fd)
{
    return (_config_socket[getIndexRequest(current_fd)]);
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

void    Socket::resetRequest(fd request_fd)
{
    _requests[getIndexRequest(request_fd)].resetRequest();
}

void    Socket::receiveData(fd fd_to_read)
{
    char buffer[BUFFER_SIZE + 1];
    int bytes_read;
    std::string str_request;
    bytes_read = recv(fd_to_read, &buffer, BUFFER_SIZE, 0);
    std::cout << "bytes_read = " << bytes_read << std::endl;
    buffer[bytes_read] = 0;
    str_request = buffer;
    if (bytes_read < 0)
    {   std::cout << "we return here" << std::endl;
        _requests[getIndexRequest(fd_to_read)].setWhereIsRequest(ZERO_REQUEST);
        _requests[getIndexRequest(fd_to_read)].setError(200);
        return ;
    }
    std::cout << "HEADER " << _requests[getIndexRequest(fd_to_read)].getHeaderCompleted() << std::endl;
    if (bytes_read > 0 && _requests[getIndexRequest(fd_to_read)].getHeaderCompleted() == FALSE)
    {

        if (_requests[getIndexRequest(fd_to_read)].getRequest() == "" && str_request == "\r\n")
        {
            return ;
        }
         if (_requests[getIndexRequest(fd_to_read)].getRequest() == "")
        {
            _requests[getIndexRequest(fd_to_read)].checkAndAddMethod(str_request);
        }
        _requests[getIndexRequest(fd_to_read)].addToRequest(str_request);
        if (_requests[getIndexRequest(fd_to_read)].getRequest().find("\n") != std::string::npos)
        {
            _requests[getIndexRequest(fd_to_read)].checkSyntaxRequest();

        }
        if (_requests[getIndexRequest(fd_to_read)].getError() == BAD_REQUEST)
        {
             _requests[getIndexRequest(fd_to_read)].setKeepAlive(FALSE);
            _sockets[getIndexRequest(fd_to_read)].events = POLLOUT;
        }
        if (_requests[getIndexRequest(fd_to_read)].getRequest().find("\r\n\r\n") != std::string::npos)
        {
            //_requests[getIndexRequest(fd_to_read)].addToRequestHeader(_requests[getIndexRequest(fd_to_read)].getRequest());
            verifyRequest(getIndexRequest(fd_to_read));
            _requests[getIndexRequest(fd_to_read)].setHeaderCompleted(TRUE);
            std::cout << "num error " <<_requests[getIndexRequest(fd_to_read)].getError() << std::endl;
            if (_requests[getIndexRequest(fd_to_read)].getMethod() != "POST")
                _sockets[getIndexRequest(fd_to_read)].events = POLLOUT;
            else
                _requests[getIndexRequest(fd_to_read)].checkAndAddContentLength(_requests[getIndexRequest(fd_to_read)].getRequest());
            str_request.erase(0, str_request.find("\r\n\r\n") + 4);
            std::cout << "Request completed" << std::endl;
        }
    }
    std::cout << "Bytes read " << bytes_read << " Header completed  " <<_requests[getIndexRequest(fd_to_read)].getHeaderCompleted() << " Method " << _requests[getIndexRequest(fd_to_read)].getMethod() << std:: endl;
    if (bytes_read > 0 && _requests[getIndexRequest(fd_to_read)].getHeaderCompleted() == TRUE  && _requests[getIndexRequest(fd_to_read)].getMethod() == "POST")
    {
        std::cout << "CONTENT LENGTH" << _requests[getIndexRequest(fd_to_read)].getStrContentLength() << std::endl;
        if ((_requests[getIndexRequest(fd_to_read)].getStrContentLength() - (int)str_request.size()) > 0)
        {
            create_file(create_path(_requests[getIndexRequest(fd_to_read)],getConfigByFd(fd_to_read)), str_request);
            _requests[getIndexRequest(fd_to_read)].setContentLength(_requests[getIndexRequest(fd_to_read)].getStrContentLength() - (int)str_request.size());
            std::cout << _requests[getIndexRequest(fd_to_read)].getStrContentLength() << "str" << str_request.size() << std::endl;
        }
        else
        {
            create_file(create_path(_requests[getIndexRequest(fd_to_read)],getConfigByFd(fd_to_read)), str_request);
            _sockets[getIndexRequest(fd_to_read)].events = POLLOUT;
            std::cout << "POST FINISHED" << std::endl;
        }
    }
}

void    Socket::removeSocket(fd fd_to_read)
{
    std::vector<struct pollfd>::iterator pollfd_itbegin = _sockets.begin();
    std::vector<Config>::iterator config_itbegin = _config_socket.begin();
	std::vector<Request>::iterator requests_itbegin = _requests.begin();
    for (size_t i = 0; i < _sockets.size() ; i++)
    {
        if (_sockets[i].fd == fd_to_read)
        {
            _sockets.erase(pollfd_itbegin + i);
            _config_socket.erase(config_itbegin + i);
			_requests.erase(requests_itbegin + i);
            std::cout << _sockets.size() << std::endl;
            return ;
        }
    }
}


Request    Socket::getRequest(fd fd_request)
{
    return (_requests[getIndexRequest(fd_request)]);
}

Request & Socket::getRefRequest(fd fd_request)
{
    return (_requests[getIndexRequest(fd_request)]);
}
void Socket::addSocketClient(Config & config, fd socket_client)
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

void Socket::verifyRequest(size_t index_request)
{
    _requests[index_request].verifyMethod(_config_socket[index_request]);
    if (_requests[index_request].getError() == 200)
        _requests[index_request].verifyHostName(_config_socket[index_request]);
}

int Socket::getRequestStatus(fd current_fd)
{
	return (_requests[getIndexRequest(current_fd)].getWhereIsRequest());
}
