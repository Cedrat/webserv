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
                std::cerr << _request.getRequest() << std::endl;
                _request.reset();
                _method->init();
                std::cout << "HEY POLLOUT" << std::endl;
                std::cout << _s_pollfd.fd << std::endl; 
                std::cout << "Event pollfd" << _s_pollfd.events << std::endl;
            }
        }
        else 
        {
            std::cout << "POST EXEC" << std::endl;
            _method->exec();
            if (_method->getIsFinished())
            {
                std::cerr << "fin prematurée" << std::endl;
                _s_pollfd.events = POLLIN;
                _s_pollfd.revents = 0;
                delete _method;
                _method = NULL;
            }
        }
    }