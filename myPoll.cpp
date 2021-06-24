#include "myPoll.hpp"
#include "myBind.hpp"
#include "fonction.hpp"

MyPoll::MyPoll()
{
    _fds = NULL;
    _nfds = 0;
    _timeout = TIMEOUT;
}

MyPoll::MyPoll(int nb_fd, fd* fd_array, short events_needed)
{
    _fds = new pollfd[nb_fd + 1];
    this->_nfds = nb_fd;
    for (size_t i = 0; i < nb_fd; i++)
    {
        _fds[i].fd = fd_array[i]; //TO_DO.error_managembbent
        _fds[i].events = events_needed;
        _fds[i].revents = 0; 
    }
    _timeout = TIMEOUT;
}

int    MyPoll::giveSocketAvailable()
{
    errno = 0;
    poll(_fds,_nfds,_timeout);
    for (size_t i = 0; i < _nfds; i++)
    {
        if (_fds[i].revents & POLLIN)
        {
            std::cout << _fds[i].fd << std::endl;
            _fds[i].revents = 0;
            return (_fds[i].fd);
        }
    }
    return (-1);
}

void MyPoll::closeAllSockets()
{
  for (size_t i = 0; i < _nfds; i++)
    {
        close(_fds[i].fd);
    }  
}