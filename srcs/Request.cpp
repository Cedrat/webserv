#include "Request.hpp"
#include <string>
#include <iostream>

std::string extract_method(std::string method_line)
{
    return (method_line.substr(0, method_line.find(" ")));
}

std::string extract_path(std::string method_line)
{
    return (method_line.substr(method_line.find(" ") + 1, method_line.rfind(" ") - method_line.find(" ") - 1));
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

void Request::setPathFileRequest(std::string path_file)
{
    _path_file_request = path_file;
}

int Request::getError() const
{
    return (_error);
}

std::string Request::getPathFileRequest(void) const
{
    return (_path_file_request);
}

void Request::addToRequestHeader(std::string request_line)
{
    std::cout << request_line << std::endl;
    request_line = request_line.substr(0, request_line.find("\n"));
    request_line += "\n";
    std::cout << request_line << std::endl;
    if (_where_is_request == ZERO_REQUEST)
    {
        if (isAValidMethodLine(request_line) == OK)
        {
            setMethod(extract_method(request_line));
            setPathFileRequest(extract_path(request_line)); 
            setError(OK);
            std::cout << "Passage ici" << std::endl;
            //requested_file set method
        }
        else if (isAValidMethodLine(request_line) == NOT_SUPPORTED)
            setError(NOT_SUPPORTED);
        else if (isAValidMethodLine(request_line) == BAD_REQUEST)
            setError(BAD_REQUEST);
    }
}