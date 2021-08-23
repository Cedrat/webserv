#include "ResponseHTTP.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include "define.hpp"
#include <iostream>


ResponseHTTP::ResponseHTTP()
{}

ResponseHTTP::ResponseHTTP(const char * path, int client_fd) : _path_file(path), 
_byte_send(0), _finished(FALSE) , _fd_to_answer(client_fd)
{}

ResponseHTTP::~ResponseHTTP()
{}


void ResponseHTTP::send()
{
    std::fstream fs;
    int ret(0);
    char buffer[BUFFER_SIZE];

    fs.open(_path_file, std::fstream::binary | std::fstream::in);
    fs.seekg(_byte_send);

    fs.read(buffer, BUFFER_SIZE);
    std::cout << "bisounours" << _path_file << std::endl;
    std::cout << buffer << std::endl;
    if (fs)
        ::send(_fd_to_answer, buffer, BUFFER_SIZE, 0);
    else
        ::send(_fd_to_answer, buffer, fs.gcount(), 0);
    _byte_send += fs.gcount();
    std::cout << "ret is" << ret << std::endl;
    fs.close();
}

bool ResponseHTTP::getFinished()
{
    return (_finished);
}

const char * ResponseHTTP::getPath()
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