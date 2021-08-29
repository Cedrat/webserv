#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "define.hpp"

class Request;
class Config;
class Socket;
class Server
{
    private :
        bool _is_running;

        std::vector<struct pollfd> _poll_fds;
        std::vector<Socket> _sockets;
        std::vector<Config> _configs;
        std::vector<Request> _requests;

    public :
        Server(void);
        ~Server();

        void    createSocketsServer(void);
        void    createAndAddSocketServer(size_t port);
        void    createAndAddSocketClient(fd new_fd_client);

        void    addConfig(Config const & config);

        void    launchingServer();
        void    acceptConnection(void);
        void    removeSocket(size_t index);
};

#endif