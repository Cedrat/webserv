#include "Request.hpp"
#include <string>
#include <iostream>

Request::Request() : _error(0)
{
    (void)_error;
}

Request::~Request()
{

}

int Request::isAValidMethodLine(std::string method_line)
{
    char motif[] = "^[A-Z]+ +\\/[!-~]* +HTTP\\/(1\\.0|1\\.1)";
    std::cout << method_line << std::endl;
    if (match_regex(const_cast<char *>(method_line.c_str()), motif) >= 1)
        return (OK);
    return (BAD_REQUEST);
}