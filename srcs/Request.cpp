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

std::string Request::getMethod() const
{
    return (_method);
}

void Request::verifyMethod(Config config)
{
    Location location = findBestLocation(config);

    if (location.checkIfMethodIsPresent(getMethod()) == FALSE)
        setError(405); 
}

Location Request::findBestLocation(Config config)
{
   std::vector<Location>    locations;
   Location                 best_location;
   size_t                   nb_of_precision = 0;

   locations = config.getLocations();

   std::vector<Location>::iterator  it_begin = locations.begin();
   std::vector<Location>::iterator  it_end = locations.end();

   for (int i = 0; it_begin != it_end; i++, it_begin++)
   {
       if ((getPathFileRequest().find(locations[i].getLocation()) != std::string::npos) && nb_of_char_in_str('/', locations[i].getLocation()) > nb_of_precision)
       {
            best_location = locations[i];
            nb_of_precision = nb_of_char_in_str('/', locations[i].getLocation());
       }
   }
   return (best_location);
}

void Request::addToRequestHeader(std::string request)
{
    std::string request_line;
    //std::cout << request_line << std::endl;
    if (_where_is_request == ZERO_REQUEST)
    {
        request_line = request.substr(0, request.find("\n"));
        request_line += "\n";
        if (isAValidMethodLine(request_line) == OK)
        {
            setMethod(extract_method(request_line));
            setPathFileRequest(extract_path(request_line)); 
            setError(OK);
            //std::cout << "Passage ici" << std::endl;
            //requested_file set method
        }
        else if (isAValidMethodLine(request_line) == NOT_SUPPORTED)
            setError(NOT_SUPPORTED);
        else if (isAValidMethodLine(request_line) == BAD_REQUEST)
            setError(BAD_REQUEST);
        _where_is_request = HOST_LINE;
    }
    char motif[] = "Host: [A-Za-z.]+[:0-9]*\r\n";
    if (_where_is_request == HOST_LINE && match_regex(const_cast<char *>(request.c_str()), motif) >= 1)
    {
        _host_name = request.substr(request.find("Host: ") + 6, request.find("\n", request.find("Host: ") + 6) - (request.find("Host: ") + 7));
        std::cout << "L'Host name retenu est" << _host_name << std::endl;
        _where_is_request = REQUEST_FINISHED;
    }
}

void Request::setWhereIsRequest(int where_is_request)
{
   _where_is_request = where_is_request; 
}

std::string Request::getHostName() const
{
    return (_host_name);
}

void    Request::verifyHostName(Config config) 
{
    if (config.checkIfHostNameIsPresent(getHostName()) == FALSE) // a finir aprés pause.
        setError(666); 
}

int Request::getWhereIsRequest() const
{
    return(_where_is_request);
}