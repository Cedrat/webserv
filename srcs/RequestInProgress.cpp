#include "RequestInProgress.hpp"
#include "Erreur.hpp"
#include "define.hpp"
#include "Generator.hpp"
#include "EOFException.hpp"

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

void RequestInProgress::setHost(int const host)
{
    _host = host;
}

void RequestInProgress::setPort(size_t const port)
{
    _port = port;
}

void RequestInProgress::receiveData()
{
    char buffer[BUFFER_SIZE + 1];
    std::string str_request;
    int ret(0);

    ret = read(_socket_fd, buffer, BUFFER_SIZE);
    if (ret == 0)
    {
        throw EOFException();
    }
    if (ret < 0)
        ret = 0;
    buffer[ret] = 0;
    str_request.append(buffer, ret);
    write(2, buffer, ret);

    if (!(_str_request.empty() && str_request == "\r\n"))
        addToRequest(str_request);
}

std::string const & RequestInProgress::getRequest() const
{
    return (this->_str_request);
}

void RequestInProgress::addToRequest(std::string str_request)
{
    write(1, str_request.c_str(), str_request.size());
    this->_str_request += str_request;
}

bool RequestInProgress::isFinished()
{
    return (this->_is_finished);
}

void RequestInProgress::setFd(int fd)
{
    _socket_fd = fd;
}

std::vector<Config> const & RequestInProgress::getConfigs() const
{
    return (_configs);
}

int const & RequestInProgress::getHost() const
{
    return (_host);
}

size_t const & RequestInProgress::getPort() const
{
    return (_port);
}

 void        RequestInProgress::checkFinished() 
{
    if (_str_request.find("\r\n\r\n") != std::string::npos || _str_request.size() > MAX_REQUEST_SIZE)
        this->_is_finished = TRUE;
}

void RequestInProgress::reset()
{
    _is_finished = FALSE;
    _str_request = "";
}
std::string extract_method(std::string str_request);
std::string extract_path(std::string str_request);
std::string extract_host_name(std::string str_request);

AMethod * RequestInProgress::getAnswer(pollfd &s_pollfd)
{
    int error;
    std::string header;
    std::string file_path;
    FieldGenerator field_generator;
    std::string method;
    
    error = checkBasicError();
    if (error != OK)
    {
        file_path = "./srcs/default_error_files/default_err" + int_to_string(error) + ".html";
        header = "HTTP/1.1 " + get_string_error(error) + "\nContent-Length: " + int_to_string(get_file_size(file_path)) + "\n\n";

        method = "ERREUR";
        return (new Erreur(_socket_fd, file_path, header,  * new FieldGet(_str_request, *this, s_pollfd)));
    }
    method = extract_method(_str_request);

    AField *field = field_generator.generate(method, _str_request, *this, s_pollfd);

    return (field->getAMethod());
}

int RequestInProgress::checkBasicError()
{
    int ret_error;

    std::string header_request = extract_header_request(_str_request);
    ret_error = check_syntax_request(header_request);
    if (ret_error != OK)
        return (ret_error);
    if (duplicata(header_request))
        return (BAD_REQUEST);
    return (OK);
}

int const & RequestInProgress::getFd() const
{
    return (_socket_fd);
}

int RequestInProgress::checkCommonError()
{

    std::string method = extract_method(_str_request);
    std::string path = extract_path(_str_request);
    std::string host_name = extract_host_name(_str_request);

    Config best_config = _configs[find_index_best_config(_configs,host_name,_host,_port)];

    return (1);
}

std::string extract_method(std::string str_request)
{
    std::vector<std::string> splitted_string = split_string(str_request, "\n");
    std::vector<std::string> splitted_line = split_string(splitted_string[0], " ");

    return (splitted_line[0]);
}

std::string extract_path(std::string str_request)
{
    std::vector<std::string> splitted_string = split_string(str_request, "\n");
    std::vector<std::string> splitted_line = split_string(splitted_string[0], " ");

    return (decoding_http_string(splitted_line[1]));
}

std::string extract_host_name(std::string str_request)
{
    std::vector<std::string> splitted_string = split_string(str_request, "\n");
    std::vector<std::string> splitted_line;

    for (size_t i = 0; i < splitted_string.size(); i++)
    {
        splitted_line = split_string(splitted_string[0], ":");
        splitted_line[0] = str_to_lower(splitted_line[0]);
        if (splitted_line[0] == "host")
        {
            trim(splitted_line[1], ' ');
            return (splitted_line[1]);
        }
    }

    return "";
}