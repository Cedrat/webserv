#ifndef CONFIG_HPP
# define CONFIG_HPP
#include <set>
#include <vector>
#include "Location.hpp"
class Config
{
    private : 
        bool                        _principal_server;
        bool                        _server_or_client;
        int                         _port;
        int                         _host;
        int                         _max_body_size;
        std::vector<std::string>    _server_names;
        std::set<int, std::string>  _error_pages;
        std::vector<Location>       _locations;

    public :
        Config();
        ~Config();
        int getPort() ;
};

#endif