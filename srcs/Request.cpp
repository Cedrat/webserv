#include "Request.hpp"
#include <string>
#include <iostream>

std::string extract_method(std::string method_line)
{
    return (method_line.substr(0, method_line.find(" ")));
}

Request::Request() : _error(0), _where_is_request(ZERO_REQUEST)
{
    (void)_error;
}

Request::~Request()
{

}

int Request::isAValidMethodLine(std::string method_line)
{
    char motif[] = "^[A-Z]+ +\\/[!-~]* +HTTP\\/(1\\.0|1\\.1)\r\n$";
    char motif_version_not_supported[] = "^[A-Z]+ +\\/[!-~]* +HTTP\\/[23]\r\n$";
    if (match_regex(const_cast<char *>(method_line.c_str()), motif) >= 1 && count_words(method_line) == 3)
        return (OK);
    else if (match_regex(const_cast<char *>(method_line.c_str()), motif_version_not_supported) >= 1 && count_words(method_line) == 3)
        return (NOT_SUPPORTED);
    return (BAD_REQUEST);
}

void Request::setMethod(std::string method)
{
        _method = method;
}

void Request::setError(int error)
{
    _error = error;
}

int Request::getError() const
{
    return (_error);
}

void Request::addToRequestHeader(std::string request_line)
{
    if (_where_is_request == ZERO_REQUEST)
    {
        if (isAValidMethodLine(request_line) == OK)
        {
            setMethod(extract_method(request_line));
            //requested_file set method
        }
        else if (isAValidMethodLine(request_line) == NOT_SUPPORTED)
            setError(NOT_SUPPORTED);
        else if (isAValidMethodLine(request_line) == BAD_REQUEST)
            setError(BAD_REQUEST);
    }
}