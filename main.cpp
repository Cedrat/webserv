#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include "myPoll.hpp"
#include "fonction.hpp"
#include "fcntl.h"

#define PORT 7995

}

int main()
{
    sockfd localhost;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size = sizeof(their_addr);
    fd new_fd;
    fd close_fd;
    struct pollfd fdpoll[1];

    MyPoll fdlistening(1, POLLIN);

    int i = 0;
        while (i < 1000) 
        {
        close_fd = fdlistening.giveSocketAvailable();
        new_fd = accept(close_fd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd < 0)
        std::cout << strerror(errno) << std::endl;
        char buffer[1000];
        int     byte_read; 
        byte_read = recv(new_fd, &buffer, 1000, 0);
        write(1, buffer, byte_read);
        buffer[byte_read] = 0;
        send(new_fd, "BONJOUR\n", 8, 0);
        //close(new_fd);
        i++;
    }
    fdlistening.closeAllSockets();
}