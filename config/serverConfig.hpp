#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <vector>
# include <set>
# include <string>

# include "locationConfig.hpp"

class serverConfig
{
    public:
        serverConfig();
        //serverConfig( serverConfig const & src );
        //serverConfig & operator=( serverConfig const & rhs );
        ~serverConfig();

        /*
            void setHost();
            void setPort();
            void setServerNames();
            void setErrorPages();
            void setLocations();
            void setMaxBodySize();

            int getPort() const;
            int getHost() const;
            std::vector<std::string> getServerNames() const;
            std::set<int, std::string> getErrorPages() const;
            std::vector<locationConfig> getLocations() const;
            int getMaxBodySize() const;
        */



    private:
        bool                        _principal_server;
        bool                        _server_or_client;
        int                         _port;
        int                         _host;
        std::vector<std::string>    _server_names;
        std::set<int, std::string>  _error_pages;
        std::vector<locationConfig> _locations;
        int                         _max_body_size;
    
};

#endif