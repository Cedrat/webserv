#ifndef METHODCGI_HPP
# define METHODCGI_HPP

# include "AMethod.hpp"
# include "../includes/utils.hpp"
# include <map>
# include "Config.hpp"

class MethodCgi : public AMethod
{
    private:
        Config      _config;
        Location    _location;
        std::string _body;

        std::map<std::string, std::string>  _env;
        std::string                         _tmpOut;

    public:
        MethodCgi(int fd, std::string path, std::string header, Config config, Location location, std::string body);
        ~MethodCgi();

        void init();
        void exec(); 

       /* void        processCGI();
        int         execCGI( const char ** args, char ** env );
        std::string readCgiFile();

        std::string createTmpFile();
        char **     convertEnv();

        void setEnv();
        void freeEnv( char ** env );*/
};

#endif