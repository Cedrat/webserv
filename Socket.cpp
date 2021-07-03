#include "Socket.hpp"
#include "utils.hpp"

Socket::Socket()
{

}

Socket::~Socket()
{

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

void Socket::addSocket(Config config)
{
    pollfd new_socket;

    new_socket = create_a_listenable_socket(config.getPort());
   _sockets.push_back(new_socket);
   _config_socket.push_back(config);
}