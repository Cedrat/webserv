#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <vector>
# include <map>
# include <string>
# include <arpa/inet.h>
# include <iostream>
# include <fstream>
# include <sys/stat.h>

# include "locationConfig.hpp"

class serverConfig
{
    public:
        serverConfig();
        //serverConfig( serverConfig const & src );
        //serverConfig & operator=( serverConfig const & rhs );
        ~serverConfig();

        void setHostAndPort( std::vector<std::string> line );
        void setServerNames( std::vector<std::string> line );
        void setErrorPages( std::vector<std::string> line );
        void setMaxClientBodySize( std::vector<std::string> line );
        void setLocationBlock( std::vector<locationConfig> location );
        void setRoot( std::vector<std::string> line );
        void setUncalledDirectives();

        bool checkServerData();
        bool checkHostAndPort();
        bool setOneHostOrPort( std::string line );
        bool checkRoot();
        bool checkServerNames();
        bool isAcceptableName( std::string line );
        bool checkErrorPages();
        bool checkMaxClientBodySize();


        bool isIP( std::string line, char c );
        bool isPort( std::string line );

        
        int                         getPort() const;
        int                         getHost() const;
        std::vector<std::string>    getServerNames() const;
        std::string                 getRoot() const;
        int                         getMaxClientBodySize() const;
        std::map<int, std::string>  getErrorPages() const;
        std::vector<locationConfig> getLocations() const;
        locationConfig              getSpecificLocation( size_t id ) const;



    private:
        std::string                 _tmpPortOrHost;
        int                         _port;
        int                         _host;
        int                         _max_body_size;
        std::string                 _root;
        std::vector<std::string>    _server_names;
        std::map<int, std::string>  _error_pages;

        //bool                        _principal_server;
        //bool                        _server_or_client;

        std::vector<locationConfig> _locations;
        
};

#endif