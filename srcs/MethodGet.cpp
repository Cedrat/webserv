#include "MethodGet.hpp"
#include "define.hpp"
#include "AField.hpp"


MethodGet::MethodGet(int fd, std::string path, std::string header, AField  &data_field) : AMethod(fd, path, header, data_field)
{

}

MethodGet::~MethodGet()
{
    delete &_fields;
}


void MethodGet::init()
{
   _fields.setPollout(); 
   _byte_send = 0; 
}

void MethodGet::exec()
{
    if (getHeaderSent() == FALSE)
    {
        sendHeader();
        setHeaderSent(TRUE);
    }
    else
    {
        sendBody();
    }
}

void MethodGet::sendHeader()
{
   send(getFd(), getHeader().c_str(), getHeader().size(), 0); 
}

void MethodGet::sendBody()
{
    signal(SIGPIPE, SIG_IGN);
    
    std::fstream fs;
    char buffer[BUFFER_SIZE + 1];
    int ret = 0;
    fs.open(getPath().c_str(),  std::fstream::in | std::fstream::app); 
    fs.seekg(_byte_send);
    fs.read(buffer, BUFFER_SIZE);
    buffer[fs.gcount()] = '\0'; 
    ret = ::send(getFd(), buffer, fs.gcount(), 0);
    _byte_send += ret;
    if (ret == fs.gcount() && fs.eof())
    {
        setIsFinished(TRUE);
    }
    fs.close();
}