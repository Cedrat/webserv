#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>
#include <unistd.h>

int main()
{
    int socked_test;
    sockaddr_in addr;

    socked_test = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(7996);
    errno = 0;
    if (bind(socked_test, (sockaddr *)(&addr), sizeof(addr)) < 0)
        std::cout << "Fail : " << strerror(errno) << " " <<std::endl;
    if (listen(socked_test, 10) < 0)
        std::cout << "Fail : " << strerror(errno) << " " <<std::endl;
    socklen_t addrlen = sizeof(sockaddr);
    int connection;
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 38\n\n<title>WebServ</title><p>Miaouuuuu</p>";
    char *cat = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 16\n\n<p>Miaouuuuu</p>";
    while (1)
    {
        connection = accept(socked_test, (struct sockaddr*)&addr, (socklen_t *)&addrlen);
        if (connection < 0)
            std::cout << "Fail : " << strerror(errno) << " " <<std::endl;


        char buffer[100];
        read(connection, buffer, 100);
        std::cout << buffer << std::endl;

        //send(connection, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 34<title>nice to see you</title>", 89, 0);
        send(connection, hello, strlen(hello), 0);
        //send(connection, cat, strlen(cat), 0);

        close(connection);
    }
    close(socked_test);
}