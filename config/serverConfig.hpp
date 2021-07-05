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

        bool checkServerData();

        void setHostAndPort( std::vector<std::string> line );
        bool checkHostAndPort();
        bool setOneHostOrPort( std::string line );
        

        bool isIP( std::string line, char c );
        bool isPort( std::string line );

        
        void setServerNames( std::vector<std::string> line );
        bool checkServerNames();
        bool isAcceptableName( std::string line );


        void setErrorPages( std::vector<std::string> line );
        bool checkErrorPages();

            //void setLocations();
        
        
        void setMaxClientBodySize( std::vector<std::string> line );
        bool checkMaxClientBodySize();


        int getPort() const;
        int getHost() const;
        std::vector<std::string> getServerNames() const;
        int getMaxClientBodySize() const;
        /*    std::map<int, std::string> getErrorPages() const;
            std::vector<locationConfig> getLocations() const;
            
        */



    private:
        std::string                 _tmpPortOrHost;
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