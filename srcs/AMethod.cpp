#include "AMethod.hpp"


AMethod::AMethod(int fd, std::string path, std::string header, AField &field)
    : _fd(fd), _header_sent(FALSE), _request_sent(FALSE), _is_finished(FALSE),
    _path(path), _header(header), _fields(field)
{

}

void AMethod::setHeaderSent(bool const value)
{
    _header_sent = value;
}

int const & AMethod::getFd()
{
    return (_fd);
}

void AMethod::setRequestSent(bool const value)
{
    _request_sent = value;
}

void AMethod::setIsFinished(bool const value)
{
    _is_finished = value;
}

void AMethod::setPath(std::string const path)
{
    _path = path;
}

void AMethod::setHeader(std::string const header)
{
    _header = header;
}

bool const & AMethod::getHeaderSent()
{
    return (this->_header_sent);
}

bool const & AMethod::getRequestSent()
{
    return (this->_request_sent);
}

bool const & AMethod::getIsFinished()
{
    return (this->_is_finished);
}

std::string const & AMethod::getPath()
{
    return (this->_path);
}

std::string const & AMethod::getHeader()
{
    return (this->_header);
}