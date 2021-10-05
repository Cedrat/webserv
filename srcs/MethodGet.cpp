#include "MethodGet.hpp"
#include "define.hpp"
#include "AField.hpp"


MethodGet::MethodGet(int fd, std::string path, std::string header, AField  &data_field) : AMethod(fd, path, header, data_field)
{

}

MethodGet::~MethodGet()
{
    
}


void MethodGet::init()
{
   _fields.setPollout(); 
   _byte_send = 0;
   _s_pollfd = NULL;
}

void MethodGet::exec()
{
    if (getHeaderSent() == FALSE)
    {
        sendHeader();
        setHeaderSent(TRUE);
        openFd();
    }
    else if (_s_pollfd->revents)
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

void MethodGet::openFd()
{
   int fd_to_read = open(getPath().c_str(), O_RDONLY, S_IRUSR);

   _s_pollfd = new pollfd;
   _s_pollfd->fd = fd_to_read;
   _s_pollfd->events = POLLOUT + POLLIN;
   _s_pollfd->revents = 0;

   _fields.addPollFd(_s_pollfd);
}

void MethodGet::sendBody()
{
    signal(SIGPIPE, SIG_IGN);

    char buffer[BUFFER_SIZE + 1];
    int ret = 0;
    ret = read(_s_pollfd->fd, buffer, BUFFER_SIZE);
    buffer[ret] = '\0'; 

    int ret_send;
    ret_send = ::send(getFd(), buffer, ret, 0);

    _byte_send += ret_send;
    if (ret != ret_send)
    {
        lseek(_s_pollfd->fd, _byte_send, SEEK_SET);
    }
    if (_byte_send == get_file_size(getPath()))
    {
        setIsFinished(TRUE);
        close(_s_pollfd->fd);
        _fields.removePollFd(_s_pollfd);
        delete (_s_pollfd);
        _s_pollfd = NULL;
    }
}