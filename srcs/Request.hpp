#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../includes/utils.hpp"
#include <string>

#define OK 1
#define BAD_REQUEST 0
#define NOT_SUPPORTED 2

#define ZERO_REQUEST 0
#define METHOD_LINE 1
#define HOST_LINE 2

class Request
{
    private : 
        std::string _method_line;
        std::string _method;
        std::string _path_file_request;
        int _error; 
        int _where_is_request;
        

    public :
        Request();
        ~Request();

        void    setMethod(std::string method);
        void    setError(int error);
        int     getError() const;
        int     isAValidMethodLine(std::string method_line);
        void    addToRequestHeader(std::string request_linei);
};

#endif