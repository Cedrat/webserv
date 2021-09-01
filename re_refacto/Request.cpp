#include "Request.hpp"
#include "../includes/utils.hpp"
#include "ResponseHTTP.hpp"
#include "Config.hpp"
#include "Location.hpp"

Request::Request(fd fd_request, size_t port) : _fd(fd_request), _port(port)
{
    resetRequest();
}

Request::~Request()
{

}

fd const & Request::getFd() const
{
    return (_fd);
}

size_t const & Request::getPort() const
{
    return (_port);
}

int const & Request::getError() const
{
    return (_error);
}

void        Request::addRequest(std::string buffer)
{
    _request += buffer;
}

bool const &    Request::getRequestCompleted() const
{
    return (_request_completed);
}

bool const &    Request::getInProgress() const
{
    return (_in_progress);
}

void    Request::setInProgress(bool boolean) 
{
    _in_progress = boolean;
}



std::string const &  Request::getMethod() const
{
    return _method;
}
std::string const & Request::getPath() const
{
    return _path;
}
std::string const & Request::getHostName() const
{
    return _host_name;
}
int const & Request::getContentLength() const
{
    return _content_length; 
}

std::string const & Request::getRequest() const
{
    return (_request);
}

void Request::setError(int number_error)
{
    _error = number_error;
}

void Request::setMethod(std::string method)
{
    _method = method;
}
void Request::setPath(std::string path)
{
    _path = path;
}
void Request::setHostName(std::string host_name)
{
    _host_name = host_name;
}
void Request::setContentLength(int content_length)
{
    _content_length = content_length;
}



void Request::receiveData()
{
    char buffer[BUFFER_SIZE];
    std::string str_request;
    int ret;
    
    ret = read(getFd(), buffer, BUFFER_SIZE);
    if (ret < 0)
        std::cout << "read error" << std::endl;
    buffer[ret] = 0;
    str_request = buffer;
    addRequest(str_request);
    checkSyntaxRequest();
    std::cout << str_request << std::endl;
    if (_request.find("\r\n\r\n") != std::string::npos)
        _request_completed = TRUE;
}

void    Request::checkSyntaxRequest()
{
    std::vector<std::string> all_lines;
    char motif_method[] = "^[A-Z]+ +\\/[!-~]* +HTTP\\/(1\\.0|1\\.1)\r\n$";
    char motif_version_not_supported[] = "^[A-Z]+ +\\/[!-~]* +HTTP\\/[23]\r\n$";
    char motif2[] = "[a-zA-z0-9]+: +.*[^ ]+\r\n";
    all_lines = split_string(_request, "\n");
    if (match_regex(const_cast<char *>((all_lines[0] + "\n").c_str()), motif_version_not_supported) >= 1)
        setError(NOT_SUPPORTED);
    else if (match_regex(const_cast<char *>((all_lines[0] + "\n").c_str()), motif_method) < 1)
        setError(BAD_REQUEST);
    if (getError() == OK)
    {
        for (size_t i = 1; i < all_lines.size(); i++)
        {
            if (match_regex(const_cast<char *>((all_lines[i] + "\n").c_str()), motif2) >= 1)
            {
            }
            else if (all_lines[i].find(" ") != std::string::npos)
            {
                setError(BAD_REQUEST);
            }
        }
    }
}

void    Request::resetRequest()
{
   _request = "";
   _error = OK;
   _request_completed = FALSE; 
   _method = "";
   _path = "";
   _host_name = "";
   _content_length = -1;
   _request_completed = FALSE;
   _response.resetByteSend();
   _response.setInProgress(FALSE);
   _response.setPathFile("");
}

void    Request::setResponseHTTP(Config config)
{
    Location location = find_best_location(*this, config);

    _response.setPathFile(construct_path(_path, location));
    
    if (check_if_file_exist(_response.getPath()) == FALSE)
        setError(NOT_FOUND);
    _response.resetByteSend();
    _response.setFdToAnswer(getFd());
    _response.setFinished(FALSE); 
    std::cout << "Final path is = " << _response.getPath() << std::endl;
}

ResponseHTTP const &    Request::getResponseHTTP() const
{
    return (_response);
}

void Request::send()
{
    std::cout << "Welcome in send" << std::endl;
    _response.send();
}