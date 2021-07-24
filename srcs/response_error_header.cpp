#include "../includes/utils.hpp"
#include "Config.hpp"
#include <fstream>
#include <iostream>
#include <sys/socket.h>

void response_error_header(int error,  Config config, fd fd_to_answer)
{
    std::string path;
    path = config.getPathError(error);

    std::string line;
    std::ifstream file;
    std:: string page;

    file.open("." + path);
    while (std::getline(file, line))
    {
        page += line + "\n";
    }
    page = "HTTP/1.1 400 Bad Request\nServer: nginx/1.21.1\nDate: Sat, 24 Jul 2021 18:03:03 GMT\nContent-Type: text/html\nContent-Length: 144\nConnection: close\n\n" + page;

    send(fd_to_answer, page.c_str(), page.size(), 0);
}