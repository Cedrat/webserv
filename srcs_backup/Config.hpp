#ifndef CONFIG_HPP
# define CONFIG_HPP
#include <set>
#include <vector>
#include "Location.hpp"
#include <iterator>
#include "../includes/utils.hpp"

# define CLIENT 0
# define SERVER 1

class Location;
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
        Config(void);
        ~Config(void);
       
        int                         getPort(void) const;
        bool                        getServerOrClient(void) const;
        int                         getHost(void) const;
        int                         getMaxBodySize(void) const;
        std::vector<std::string>    getServersNames(void) const;
        std::map<int, std::string>  getErrorPages(void) const;
        std::string                 getPathError(int num_error) const;
        std::vector<Location>       getLocations(void) const;

        bool    IsServerOrClient(void) const;
        bool    IsPrincipalServer(void) const;

        void    setPort(int port);
        void    setServerOrClient(bool soc);
        void    setPrincipalServer(bool);
        void    setHost(int host);
        void    setMaxBodySize(int max_body_size);

        void    addServerName(std::string str);
        void    addErrorPages(int error_nb, std::string path_error);
        void    addLocation(Location location);

        bool    checkIfHostNameIsPresent(std::string host_name) const;

};

#endif