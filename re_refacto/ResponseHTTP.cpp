#include "ResponseHTTP.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include "define.hpp"
#include <iostream>
#include "../includes/utils.hpp"
#include <csignal>


ResponseHTTP::ResponseHTTP() : _byte_send(0), _finished(FALSE)
{}

ResponseHTTP::ResponseHTTP(const char * path, int client_fd) : _path_file(path),
_byte_send(0), _finished(FALSE) , _fd_to_answer(client_fd)
{}

ResponseHTTP::~ResponseHTTP()
{}

void    ResponseHTTP::setPathFile(std::string path)
{
    _path_file = path;
}

void ResponseHTTP::setFdToAnswer(int fd_client)
{
    _fd_to_answer= fd_client;
}
void ResponseHTTP::send()
{
    signal(SIGPIPE, SIG_IGN);
    std::fstream fs;
    char buffer[BUFFER_SIZE + 1];
    int ret = 0;
    std::cout << "PATH : " << _path_file << std::endl; 
    fs.open(_path_file.c_str(),  std::fstream::in | std::fstream::app); 
    fs.seekg(_byte_send);
    fs.read(buffer, BUFFER_SIZE);
    buffer[fs.gcount()] = '\0'; 
    std::cout << "how much read? " << fs.gcount() << std::endl;
    ret = ::send(_fd_to_answer, buffer, fs.gcount(), 0);
    _byte_send += ret;
    if (fs.eof())
    {
        _finished = TRUE;
    }
    std::cout << ret << "BYTE SEND" << std::endl;
    fs.close();
}

bool ResponseHTTP::getFinished() const
{
    return (_finished);
}

bool ResponseHTTP::getInProgress() const
{
    return (_in_progress);
}

void ResponseHTTP::setFinished(bool boolean)
{
    _finished = boolean;
}

void ResponseHTTP::setInProgress(bool boolean)
{
    _in_progress = boolean;
}


void ResponseHTTP::resetByteSend()
{
    _byte_send = 0;
}

std::string ResponseHTTP::getPath() const
{
    return (_path_file);
}

 ResponseHTTP& ResponseHTTP::operator=(const ResponseHTTP& other)
 {
     _path_file = other._path_file;
     _byte_send = other._byte_send;
     _finished = other._finished;
     _fd_to_answer = other._fd_to_answer;

     return (*this);
 }
