#include "Socket.hpp"
#include "myPoll.hpp"
#include "fcntl.h"
#include "fonction.hpp"
#include "SocketClient.hpp"


int main()
{
    struct sockaddr_in their_addr;
    socklen_t sin_size = sizeof(their_addr);
    Socket list_of_socket;
    int i = 8400;
    for (size_t p = 0; p < 20; p++)
    {
        list_of_socket.add_sockets_listening(i+p);
    }

    fd * lst_socket;
    lst_socket = list_of_socket.list_sockets();
    MyPoll poll_Fd(20, lst_socket, POLLIN);
    SocketClient fd_client;
    fd temp_fd;
    while (1)
    {
        int     byte_read = -1; 
        fd_client = poll_Fd.giveSocketAvailable();
        fd_client = accept(fd_client, (struct sockaddr *)&their_addr, &sin_size);
        std::cout << "Accept you ! " << std::endl;
        char buffer[1000];
        send(fd_client, "BONJOUR\n", 8, 0);
        byte_read = recv(fd_client, &buffer, 1000, MSG_DONTWAIT);
        write(1, buffer, byte_read);
        buffer[byte_read] = 0;
        close(fd_client);
    }
}