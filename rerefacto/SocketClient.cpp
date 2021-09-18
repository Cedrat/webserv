#include "define.hpp"
#include "SocketClient.hpp"
#include "AMethod.hpp"

SocketClient::SocketClient(size_t port, int host, int fd, std::vector<Config> const & configs, pollfd &s_pollfd) 
: ASocket(port, host, std::time(0), CLIENT) , _method(nullptr) ,_configs(configs), _s_pollfd(s_pollfd)
{
    _request.setFd(fd);
    _request.setConfigs(configs);
}

 void SocketClient::exec()
    {
        if (_method == nullptr)
        {
            _request.receiveData();
            _request.checkFinished();
            if (_request.isFinished())
            {
                _method = _request.getAnswer();
                std::cerr << _request.getRequest() << std::endl;
                _request.reset();
                _s_pollfd.events = POLLOUT; //you are fired when post is back.
                _s_pollfd.revents = 0;
                std::cout << "HEY POLLOUT" << std::endl;
                std::cout << _s_pollfd.fd << std::endl; 
                std::cout << "Event pollfd" << _s_pollfd.events << std::endl;
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
                _method = nullptr;
            }
        }
    }