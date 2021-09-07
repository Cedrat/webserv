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

class CGI
{
    public:
        CGI();
        CGI( CGI const & src );
        CGI & operator=( CGI const & rhs );
        ~CGI();

        void        processCGI();
        std::string execCGI( const char ** args, char ** env );
        std::string readCgiFile();

        std::string createTmpFile();
        char **     convertEnv();

        void setEnv();
        void freeEnv( char ** env );
        std::map<std::string, std::string> getEnv();


    private:
        std::map<std::string, std::string>  _env;
        std::string                         _tmpOut;

};

#endif