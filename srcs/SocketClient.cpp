#include "define.hpp"
#include "SocketClient.hpp"
#include "AMethod.hpp"

SocketClient::SocketClient(size_t port, int host, int fd, std::vector<Config> const & configs, pollfd &s_pollfd) 
: ASocket(port, host, std::time(0), CLIENT) , _method(NULL) ,_configs(configs), _s_pollfd(s_pollfd)
{
    _request.setFd(fd);
    _request.setConfigs(configs);
    _request.setHost(host);
    _request.setPort(port);
}

 void SocketClient::exec()
    {
        if (_method == NULL)
        {
            _request.receiveData();
            _request.checkFinished();
            if (_request.isFinished())
            {
                _method = _request.getAnswer(_s_pollfd);
                _request.reset();
                _method->init();
            }
        }
        else 
        {
            _method->exec();
            if (_method->getIsFinished())
            {
                _s_pollfd.events = POLLIN;
                _s_pollfd.revents = 0;
                delete _method;
                _method = NULL;
            }
        }
    }