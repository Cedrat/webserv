#ifndef CGI_HPP
# define CGI_HPP

# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>

# include <iostream>
# include <cstring>
# include <cstdlib>
# include <cstdio>
# include <map>
# include <string>

# include "Config.hpp"

class CGI
{
    private:
        std::map<std::string, std::string>  _env;
        std::string                         _tmpOut;


    public:
        CGI(Config config, Location location, std::string body);
        //CGI( CGI const & src );
        //CGI & operator=( CGI const & rhs );
        ~CGI();

        void        processCGI();
        int         execCGI( const char ** args, char ** env );
        std::string readCgiFile();

        std::string createTmpFile();
        char **     convertEnv();

        void setEnv();
        void freeEnv( char ** env );


        std::map<std::string, std::string>  getEnv();       //A faire ?
        std::string                         getHeader();    //A faire

};

#endif