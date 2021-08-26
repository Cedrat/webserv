#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <vector>
#include <poll.h>
#include "../includes/utils.hpp"
#include "Config.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include "Request.hpp"
class Socket
{
    private :
        std::vector<struct pollfd> _sockets;
        std::vector<Config> _config_socket;
        std::vector<Request> _requests;

    public :
                    Socket();
                    ~Socket();

        void        addSocketServer(Config & config);
        void        addSocketClient(Config & config, fd socket_client);

        int                     getNbOfSockets() const;
        std::vector<struct pollfd>               	getSockets() ;
         std::vector<struct pollfd>               	&getRefSockets();
        Config                  getConfig(int index_of_socket_needed) const;
        int                     getIndexRequest(fd fd_to_request);
        int                     getRequestStatus(fd current_fd);
        std::vector<Request>    getRequests();
        Request                 getRequest(fd fd_required);
        Request &               getRefRequest(fd fd_required);
        Config                  getConfigByFd(int current_fd);
        void                    resetRequest(fd request_fd);


        void        removeSocket(fd fd_to_remove);

        bool        isAFdServer(fd i) const;

        void        receiveData(fd fd_to_read);

        void        verifyRequest(size_t index_request);
};

#endif
