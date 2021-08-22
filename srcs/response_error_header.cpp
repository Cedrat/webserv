#include "../includes/utils.hpp"
#include "Config.hpp"
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <sstream>

void response_error_header(int num_code,  Config config, fd fd_to_answer)
{
    std::string path;
    path = config.getPathError(num_code);

    std::cout << "response_error_header" <<  path << std::endl;
    std::string line;
    std::ifstream file;
    std:: string page;

    file.open("./www/" + path);
    while (std::getline(file, line))
    {
        page += line + "\n";
    }
    page = "HTTP/1.1 " + get_string_error(num_code) + "\nContent-Length: " + int_to_string(page.size()) + "\n\n" + page;

    send(fd_to_answer, page.c_str(), page.size(), 0);
}

void response_good_file(std::string path, fd fd_to_answer, bool ai)
{
    std::string line;
    std::ifstream file;
    std:: string page;
    struct stat sb;

    stat(path.c_str(), &sb);
    if (ai == FALSE || S_ISREG(sb.st_mode))
    {
        file.open(path);
        while (std::getline(file, line))
        {
            page += line + "\n";
        }
    }
    else
    {
        page = create_ai_page(path.c_str());
    }
    page = "HTTP/1.1 " + get_string_error(200) +"\nContent-Length: " + int_to_string(page.size()) + "\n\n" + page;
    send(fd_to_answer, page.c_str(), page.size(), 0); 
}