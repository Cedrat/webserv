#ifndef CONFIG_HPP
# define CONFIG_HPP
#include <set>
#include <vector>
#include "Location.hpp"
#include <iterator>
#include "../includes/define.hpp"

# include "../includes/Syntax.hpp"
# include "../includes/utils.hpp"
# include "../includes/Formatting.hpp"

# define CHECK_SERVER_NB 3
# define LIMIT_MAX_BODY_SIZE 10000000
class Location;
class Config
{
    typedef bool(Config::*checks)();
    
    private : 
        std::vector<Location>       _locations;

        bool                        _principal_server;  //=_default_server
        bool                        _server_or_client;

        size_t                      _port;
        int                         _host;
        int                         _max_body_size;
        std::vector<std::string>    _server_names;
        std::map<int, std::string>  _error_pages;
        std::string                 _ip;


    public :
        Config(void);
        virtual ~Config(void);
        Config( Config const & src );
        Config & operator=( Config const & rhs );

        //Assignation des valeurs
        void setHostAndPort( std::vector<std::string> line );
        void setOneHostOrPort( std::string line );
        void setServerNames( std::vector<std::string> line );
        void setErrorPages( std::vector<std::string> line );
        void setMaxClientBodySize( std::vector<std::string> line );
        void setLocation( std::vector<Location> location );
        void setUncalledDirectives();
        void setPrincipalServer( bool value );
        void setServerOrClient(bool server_or_client);


        //Check des valeurs
        bool checkServerData();
        bool checkServerNames();
        bool checkErrorPages();
        bool checkMaxClientBodySize();
        bool isEqual(const Config & rhs);



        //Recuperation des valeurs
        size_t                      getPort() const;
        int                         getHost() const;
        std::string                 getIP() const;
        std::vector<std::string>    getServerNames() const;
        int                         getMaxBodySize() const;
        std::map<int, std::string>  getErrorPages() const;
        std::vector<Location>       getLocations() const;
        Location                    getOneLocation( size_t id ) const;
        std::string                 getPathError(int num_error) const;
        
        bool                        IsServerOrClient(void) const;
        bool                        IsPrincipalServer(void) const;

        void                        debug();




        bool    checkIfHostNameIsPresent(std::string host_name) const;

};

#endif