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
# include "../includes/Syntax.hpp"

class serverConfig
{
    typedef bool(serverConfig::*checks)();

    public:
        serverConfig();
        //serverConfig( serverConfig const & src );
        ~serverConfig();

        void setHostAndPort( std::vector<std::string> line );
        void setOneHostOrPort( std::string line );
        void setServerNames( std::vector<std::string> line );
        void setErrorPages( std::vector<std::string> line );
        void setMaxClientBodySize( std::vector<std::string> line );
        void setLocation( std::vector<locationConfig> location );
        void setRoot( std::vector<std::string> line );
        void setUncalledDirectives();
        void setDefaultServer( bool value );

        bool checkServerData();
        bool checkRoot();
        bool checkServerNames();
        bool checkErrorPages();
        bool checkMaxClientBodySize();

        bool isEqual(const serverConfig & rhs) const;

        
        int                         getPort() const;
        int                         getHost() const;
        std::vector<std::string>    getServerNames() const;
        std::string                 getRoot() const;
        int                         getMaxClientBodySize() const;
        std::map<int, std::string>  getErrorPages() const;
        std::vector<locationConfig> getLocations() const;
        locationConfig              getOneLocation( size_t id ) const;
        bool                        getDefaultServer() const;

        void                        debug();


    private:
        std::vector<locationConfig> _locations;

        int                         _port;
        int                         _host;
        int                         _max_body_size;
        std::string                 _root;
        std::vector<std::string>    _server_names;
        std::map<int, std::string>  _error_pages;

        bool                        _default_server;
        //bool                        _server_or_client;
  
};

#endif