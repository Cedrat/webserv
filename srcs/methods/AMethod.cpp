#include "AMethod.hpp"
#include "../includes/define.hpp"
#include "../includes/CustomException.hpp"
#include "../fields/AField.hpp"

AMethod::AMethod(int fd, std::string path, std::string header, AField &field)
    : _fd(fd), _header_sent(FALSE), _request_sent(FALSE), _is_finished(FALSE),
    _path(path), _header(header), _fields(field) , _byte_send(0)
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

void AMethod::sendBody()
{
    signal(SIGPIPE, SIG_IGN);
    
    std::fstream fs;
    char buffer[BUFFER_SIZE + 1];
    int ret = 0;

    fs.open(getPath().c_str(),  std::fstream::in | std::fstream::app); 
    if (fs.fail())
    {
        throw(FileDisappearedException());
    }

    fs.seekg(_byte_send);
    fs.read(buffer, BUFFER_SIZE);
    

    buffer[fs.gcount()] = '\0'; //gcount cannot be negative, no possibility of underflow
    ret = ::send(getFd(), buffer, fs.gcount(), 0);
    if (ret == -1)
    {
         throw(UnableToSendException());
    }
    _byte_send += ret;
    if (ret == fs.gcount() && fs.eof())
    {
        setIsFinished(TRUE);
        if (_fields.getError() == BAD_REQUEST)
            throw(BadRequestException());
    }
    fs.close();
}

void AMethod::sendHeader()
{
    int ret;
    ret = send(getFd(), getHeader().c_str(), getHeader().size(), 0); //https://linux.die.net/man/2/send
    if (ret == -1)
    {
         throw(UnableToSendException());
    }
}