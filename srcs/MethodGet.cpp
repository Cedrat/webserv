#include "MethodGet.hpp"
#include "define.hpp"


MethodGet::MethodGet(int fd, std::string path, std::string header) : AMethod(fd, path, header)
{

}

MethodGet::~MethodGet()
{
    
}








void MethodGet::init()
{
   _byte_send = 0; 
}

void MethodGet::exec()
{
    if (getHeaderSent() == FALSE)
    {
        sendHeader();
        setHeaderSent(TRUE);
        std::cerr << "Header sent" << std::endl;
    }
    else
    {
        std::cout << "body sent" << std::endl;
        sendBody();
        //setIsFinished(TRUE);
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
    std::cout << "PATH : " << getPath() << std::endl; 
    fs.open(getPath().c_str(),  std::fstream::in | std::fstream::app); 
    fs.seekg(_byte_send);
    fs.read(buffer, BUFFER_SIZE);
    buffer[fs.gcount()] = '\0'; 
    std::cout << "how much read? " << fs.gcount() << std::endl;
    ret = ::send(getFd(), buffer, fs.gcount(), 0);
    _byte_send += ret;
    if (ret == fs.gcount() && fs.eof())
    {
        setIsFinished(TRUE);
    }
    std::cout << ret << "BYTE SEND" << std::endl;
    fs.close();
}