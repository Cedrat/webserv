#include "ResponseHTTP.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include "define.hpp"
#include <iostream>


ResponseHTTP::ResponseHTTP() : _byte_send(0), _finished(FALSE)
{}

ResponseHTTP::ResponseHTTP(const char * path, int client_fd) : _path_file(path),
_byte_send(0), _finished(FALSE) , _fd_to_answer(client_fd)
{}

ResponseHTTP::~ResponseHTTP()
{}

void    ResponseHTTP::setPathFile(const char *path)
{
    _path_file = path;
}

void ResponseHTTP::setFdToAnswer(int fd_client)
{
    _fd_to_answer= fd_client;
}
void ResponseHTTP::send()
{
    std::fstream fs;
    char buffer[BUFFER_SIZE];


	std::cout << _path_file << "blanco" << std::endl;
    fs.open(_path_file.c_str(), std::fstream::binary | std::fstream::in);
    fs.seekg(_byte_send);

    fs.read(buffer, BUFFER_SIZE);
    std::cout << "bisounours" << _byte_send << std::endl;
    //std::cout << buffer << std::endl;
    if (fs)
        ::send(_fd_to_answer, buffer, BUFFER_SIZE, 0);
    else
    {
        ::send(_fd_to_answer, buffer, fs.gcount(), 0);
        std::cout << "Finished boy" << std::endl;
        _finished = TRUE;
    }
    _byte_send += fs.gcount();
    std::cout << "Is Finished ?" << _finished << std::endl;
    fs.close();
}

bool ResponseHTTP::getFinished()
{
    return (_finished);
}

void ResponseHTTP::setFinished(bool boolean)
{
    _finished = boolean;
}

void ResponseHTTP::resetByteSend()
{
    _byte_send = 0;
}

std::string ResponseHTTP::getPath()
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
