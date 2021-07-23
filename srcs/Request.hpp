#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../includes/utils.hpp"
#include <string>

#define OK 1
#define BAD_REQUEST 0
#define NOT_SUPPORTED 2

class Request
{
    private : 
        std::string _method_line;
        int _error;

    public :
        Request();
        ~Request();
        int    isAValidMethodLine(std::string method_line);
};

#endif