#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <vector>
# include <map>
# include <string>
# include <arpa/inet.h>
# include <iostream>

# include "locationConfig.hpp"

class serverConfig
{
    public:
        serverConfig();
        //serverConfig( serverConfig const & src );
        //serverConfig & operator=( serverConfig const & rhs );
        ~serverConfig();

        bool setHostAndPort( std::vector<std::string> line );
        bool setOneHostOrPort( std::string line );
        bool isIP( std::string line, char c );
        bool isPort( std::string line );

        
        bool setServerNames( std::vector<std::string> line );
        bool isAcceptableChar( std::string line );
            //bool setErrorPages( std::vector<std::string> line );
            //void setLocations();
            //bool setMaxBodySize( std::vector<std::string> line );

        int getPort() const;
        int getHost() const;
            /*std::vector<std::string> getServerNames() const;
            std::map<int, std::string> getErrorPages() const;
            std::vector<locationConfig> getLocations() const;
            int getMaxBodySize() const;
        */



    private:
        int                         _port;
        int                         _host;
        std::vector<std::string>    _server_names;
        std::map<int, std::string>  _error_pages;

        //bool                        _principal_server;
        //bool                        _server_or_client;


        //std::vector<locationConfig> _locations;
        int                         _max_body_size;
    
};

#endif