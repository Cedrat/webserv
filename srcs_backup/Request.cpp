#include "ResponseHTTP.hpp"
#include "Request.hpp"
#include "Config.hpp"
#include <string>
#include <iostream>
#include <unistd.h>
#include <stddef.h>

std::string extract_method(std::string method_line)
{
    return (method_line.substr(0, method_line.find(" ")));
}

std::string extract_path(std::string method_line)
{
    return (method_line.substr(method_line.find(" ") + 1, method_line.rfind(" ") - method_line.find(" ") - 1));
}

Request::Request() :  _header_completed(FALSE),  _error(OK), _where_is_request(ZERO_REQUEST),_sending_data(FALSE)
{
    (void)_error;
}

Request::~Request()
{

}

void Request::setFinished(bool boolean)
{
    _data_to_send.setFinished(boolean);
}

void Request::setFdAnswer(fd fd_client)
{
    _data_to_send.setFdToAnswer(fd_client);
}

void Request::resetByteSend()
{
    _data_to_send.resetByteSend();
}

void    Request::setResponseHTTP(ResponseHTTP  rep)
{
    _data_to_send = rep;
}

void Request::setPathFileAnswer(const char *path)
{
    _data_to_send.setPathFile(path);
}

bool    Request::getSendingData() const
{
    return (_sending_data);
}

ResponseHTTP    Request::getResponseHTTP() const
{
    return _data_to_send;
}

void Request::setSendingData(bool boolean)
{
    _sending_data = boolean;
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

void Request::send()
{
   _data_to_send.send();
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

int         Request::getError() const
{
    return (_error);
}

void Request::verifyMethod(Config  config)
{
    Location location = findBestLocation(config);

    if (location.checkIfMethodIsPresent(getMethod()) == FALSE)
    {
        setError(METHOD_NOT_ALLOWED);
    }
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

    //checkDuplicate(request);
    if (_where_is_request == ZERO_REQUEST)
    {
        checkAndAddMethod(request);
    }
    if (_where_is_request == HOST_LINE)
    {
        checkAndAddHostName(request);
    }
    if (getError() == OK)
        checkSyntaxRequest(request);
}

void Request::setWhereIsRequest(int where_is_request)
{
   _where_is_request = where_is_request;
}

std::string Request::getHostName() const
{
    return (_host_name);
}

std::string Request::getRequest() const
{
    return (_request);
}

void        Request::addToRequest(std::string request)
{
    _request += request;

}
void    Request::verifyHostName(Config config)
{
    (void) config;
    checkAndAddHostName(_request);
    if (getHostName() == "")
         setError(400);
}

int Request::getWhereIsRequest() const
{
    return(_where_is_request);
}

void    Request::checkDuplicate(std::string request)
{
    std::string request_line;

    request_line = request.substr(0, request.find("\n"));
    request_line += "\n";
    if (isAValidMethodLine(request_line) == OK && _where_is_request != ZERO_REQUEST)
        setError(BAD_REQUEST);
    char motif[] = "Host: [A-Za-z.]+[:0-9]*\r\n";
    if (_where_is_request == REQUEST_FINISHED && match_regex(const_cast<char *>(request.c_str()), motif) >= 1)
    {
        //setError(BAD_REQUEST);
    }
}

void Request::checkDuplicate(std::vector<std::string> all_lines)
{

    int nb_host(0);
    char motif[] = "Host: [A-Za-z.]+[:0-9]*\r\n";
    for (size_t i = 1; i < all_lines.size(); i++)
    {
        if (match_regex(const_cast<char *>((all_lines[i] + "\n").c_str()), motif) >= 1)
        {
            nb_host++;
            checkAndAddHostName(all_lines[i]);
            if (nb_host > 1)
            {
                setError(BAD_REQUEST);
                return ;
            }
        }
    }
}
void    Request::checkSyntaxRequest(std::string request)
{
    std::vector<std::string> all_lines;
    char motif_method[] = "^[A-Z]+ +\\/[!-~]* +HTTP\\/(1\\.0|1\\.1)\r\n$";
    char motif_version_not_supported[] = "^[A-Z]+ +\\/[!-~]* +HTTP\\/[23]\r\n$";
    char motif2[] = "[a-zA-z0-9]+: .+\r\n";
    all_lines = split_string(request, "\n");
    for (size_t i = 1; i < all_lines.size(); i++)
    {
        if (isAValidMethodLine(all_lines[i] + "\n") == OK)
        {

        }
        else if (match_regex(const_cast<char *>((all_lines[i] + "\n").c_str()), motif2) >= 1)
        {
        }
        else if (all_lines[i].find(" ") != std::string::npos)
        {
            setError(BAD_REQUEST);
        }
    }
}

void Request::resetRequest()
{
    _request = "";
    _method_line = "";
    _method = "";
    _path_file_request = "";
    _host_name = "";
    _header_completed = FALSE;
    _keep_alive = TRUE;
    _error = OK;
    _where_is_request = ZERO_REQUEST;

}

void Request::setKeepAlive(bool boolean)
{
    _keep_alive = boolean;
}

bool Request::getKeepAlive()
{
    return (_keep_alive);
}

void Request::checkPath()
{
    if (check_valid_path(_path_file_request) == FALSE)
        setError(BAD_REQUEST);
}

void    Request::checkSyntaxRequest()
{
    std::vector<std::string> all_lines;
    std::string lines = _request;

    if (lines.find("\r\n\r\n") != std::string::npos)
        lines.erase(lines.find("\r\n\r\n"), lines.size() - (lines.find("\r\n\r\n")));
    char motif2[] = "[a-zA-z0-9]+: .+\r\n";
    all_lines = split_string(lines, "\n");
    checkDuplicate(all_lines);

    if (isAValidMethodLine(all_lines[0] + "\n") != OK)
        setError(BAD_REQUEST);
    for (size_t i = 1; i < all_lines.size(); i++)
    {
        if (match_regex(const_cast<char *>((all_lines[i] + "\n").c_str()), motif2) >= 1)
        {
            //
        }
        else if (all_lines[i].find(" ") != std::string::npos)
        {
            setError(BAD_REQUEST);
        }
    }
}

void Request::setContentLength(int content_length)
{
    _str_content_length = content_length;
}

int Request::getStrContentLength() const
{
    return(_str_content_length);
}

void Request::checkAndAddMethod(std::string request)
{
    std::string request_line;
    request_line = request.substr(0, request.find("\n"));
    request_line += "\n";

    if (isAValidMethodLine(request_line) == OK)
    {
        setMethod(extract_method(request_line));
        if (check_valid_path(extract_path(request_line)))
        {
            setPathFileRequest(decoding_http_string(factorised_path(extract_path(request_line))));
        }
        else
        {
            setError(BAD_REQUEST);
        }
    }
    else if (isAValidMethodLine(request_line) == NOT_SUPPORTED)
    {
        setError(NOT_SUPPORTED);
    }
    else if (isAValidMethodLine(request_line) == BAD_REQUEST)
        setError(BAD_REQUEST);
    _where_is_request = HOST_LINE;
}

void Request::checkAndAddHostName(std::string request)
{
    char motif[] = "[a-zA-z0-9]+: +.+\r\n";
    if (_where_is_request == HOST_LINE && match_regex(const_cast<char *>(request.c_str()), motif) >= 1 && getError() == OK)
    {
        _host_name = request.substr(request.find("Host: ") + 6, request.find("\n", request.find("Host: ") + 6) - (request.find("Host: ") + 7));
        _where_is_request = REQUEST_FINISHED;
    }
}

void Request::checkAndAddContentLength(std::string request)
{
    char motif[] = "Content-Length:";
    if (match_regex(const_cast<char *>(request.c_str()), motif) >= 1)
    {
        _str_content_length = string_to_int(request.substr(request.find("Content-Length: ") + 16, request.find("\n", request.find("Content-Length: ") + 16) - (request.find("Content-Length: ") + 17)));
        _where_is_request = REQUEST_FINISHED;
    }
}


bool Request::getHeaderCompleted() const
{
    return (_header_completed);
}

void Request::setHeaderCompleted(bool boolean)
{
    _header_completed = boolean;
}