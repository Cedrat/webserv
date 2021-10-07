#ifndef METHODCGI_HPP
# define METHODCGI_HPP

# include "AMethod.hpp"
# include "../includes/utils.hpp"
# include "Config.hpp"

# define BUFFER_CGI 50

class MethodCgi : public AMethod
{
    private:
        Config      _config;
        Location    _location;
        std::string _body;
        std::string _method;

        std::map<std::string, std::string>  _env;
        std::string                         _tmp_out;

        std::string _header_cgi;
        std::string _body_cgi;
        int         _sent;
        pid_t       _pid;
        
        bool        _read_ended;
        bool        _pid_ended;
        size_t      _readed;

    public:
        MethodCgi(int fd, std::string path, std::string header, 
                Config config, Location location, std::string body, std::string method, AField & field);
        ~MethodCgi();

        void init();
        void setEnv();

        char **     convertEnv();
        void        freeEnv( char ** env );
        std::string createTmpFile();
        
        void        exec();
        void        processCGI();
        void        execCGI( const char ** args, char ** env );
        void        readCgiFile();
        void        extractHeader();
        void        adaptHeader();
        void        sendBody();
        void        setErrorResponse();
        
};

#endif