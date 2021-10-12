#ifndef METHODPOSTCGI_HPP
# define METHODPOSTCGI_HPP

# include "MethodPost.hpp"
# include "define.hpp"
# include "ChunkedRequest.hpp"
# include "AField.hpp"
# include "MethodCgi.hpp"

class ChunkedRequest;

class MethodPostCgi : public MethodPost
{
    private:
        Config      _config;
        Location    _location;
        std::string _body;
        std::string _path_file;

        bool        _cgi_init;
        bool        _get_body;
        MethodCgi * _cgi;

    public:
        MethodPostCgi( int fd, std::string path, std::string request_received, AField &field,
            Config config, Location location );
        ~MethodPostCgi();
        void exec();
        void getBody();
};

#endif