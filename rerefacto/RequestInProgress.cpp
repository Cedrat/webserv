#include "RequestInProgress.hpp"
#include "Erreur.hpp"
#include "define.hpp"
#include "MethodGenerator.hpp"

RequestInProgress::RequestInProgress() : _is_finished(FALSE)
{

}

RequestInProgress::~RequestInProgress()
{

}

void RequestInProgress::setConfigs(std::vector<Config> configs)
{
    _configs = configs;
}

void RequestInProgress::receiveData()
{
    char buffer[BUFFER_SIZE];
    std::string str_request;
    int ret(0);

    ret = read(_socket_fd, buffer, BUFFER_SIZE);
    buffer[ret] = 0;
    str_request = buffer;

    if (!(_str_request.empty() && str_request == "\r\n"))
        addToRequest(str_request);
}

std::string const & RequestInProgress::getRequest() const
{
    return (this->_str_request);
}

void RequestInProgress::addToRequest(std::string str_request)
{
    this->_str_request += str_request;
}

bool const RequestInProgress::isFinished()
{
    return (this->_is_finished);
}

void RequestInProgress::setFd(int fd)
{
    _socket_fd = fd;
}

 void        RequestInProgress::checkFinished() 
{
    if (_str_request.find("\r\n\r\n") != std::string::npos)
        this->_is_finished = TRUE;
}

void RequestInProgress::reset()
{
    _is_finished = FALSE;
    _str_request = "";
}

AMethod * RequestInProgress::getAnswer()
{
    int error;
    std::string header;
    std::string path_error;
    MethodGenerator method_generator;
    std::string method;

    
    error = checkBasicError();

    path_error = "./default_error_files/default_err" + int_to_string(error) + ".html";
    header = "HTTP/1.1 " + get_string_error(error) + "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n\n";

    method = "ERREUR";

    return (method_generator.generate(method, _socket_fd, path_error, header));
}

int RequestInProgress::checkBasicError()
{
    int ret_error;

    ret_error = check_syntax_request(_str_request);
    if (ret_error != OK)
        return (ret_error);
    if (duplicata(_str_request))
        return (BAD_REQUEST);
    return (OK);
}