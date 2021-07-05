#ifndef CONFIG_HPP
# define CONFIG_HPP
#include <set>
#include <vector>
#include "Location.hpp"
#include <iterator>

# define CLIENT 0
# define SERVER 1

class Config
{
    private : 
        bool                        _principal_server;
        bool                        _server_or_client;
        int                         _port;
        int                         _host;
        int                         _max_body_size;
        std::vector<std::string>    _server_names;
        std::map<int, std::string>  _error_pages;
        std::vector<Location>       _locations;

    public :
        Config();
        ~Config();
        int getPort() const;
        void setPort(int port);
        void setServerOrClient(bool soc);
        bool getServerOrClient() const;

        void setPrincipalServer(bool);
        void setHost(int host);
        void setMaxBodySize(int max_body_size);
};

#endif