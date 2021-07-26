#include "../includes/utils.hpp"
#include "Config.hpp"
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <string>
#include <stdio.h>
#include <cstdlib>

void response_error_header(int num_code,  Config config, fd fd_to_answer)
{
    std::string path;
    path = config.getPathError(num_code);


    std::string line;
    std::ifstream file;
    std:: string page;

    file.open("./www/" + path);
    while (std::getline(file, line))
    {
        page += line + "\n";
    }
    page = "HTTP/1.1 " + get_string_error(num_code) + "\n\n" + page;

    send(fd_to_answer, page.c_str(), page.size(), 0);
}

void response_good_file(std::string path, fd fd_to_answer)
{
   std::string line;
    std::ifstream file;
    std:: string page;
    if (path == "/")
        path += "index.html";
    file.open("./www" + path);
    
    while (std::getline(file, line))
    {
        page += line + "\n";
    }
    page = "HTTP/1.1 " + get_string_error(200) + "\n\n" + page;

    send(fd_to_answer, page.c_str(), page.size(), 0); 
}