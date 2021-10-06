#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/utils.hpp"

class ASocket;
class SocketServer;
class Config;

class Server
{
    private :
        std::vector<ASocket *>      _sockets;
        std::vector<struct pollfd *>  _pollfds;
        std::vector<Config>         _configs;
        bool                        _is_running;
        
        void addSocket(ASocket *socket);
        void addPollFd(pollfd * datafd);

    public : 
        Server();
        ~Server();

        void createSocketsServer();
        void createAndAddSocketServer(size_t port, int host);

        void addNewSocket(ASocket *socket, pollfd * datafd);
        void addConfig(Config config);
        void launchingServer(void);
        void acceptConnection(void);

        void exec_pollin(ASocket *socket, int current_fd, pollfd & s_pollfd);
        void exec_pollout(ASocket *socket, int fd_client, pollfd & s_pollfd);

        void removeClient(size_t index);

        void endServer();
};

#endif