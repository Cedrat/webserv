#ifndef METHODPOSTCGI_HPP
# define METHODPOSTCGI_HPP

# include "MethodPost.hpp"
# include "../includes/define.hpp"
# include "../parsing/ChunkedRequest.hpp"
# include "../fields/AField.hpp"
# include "MethodCgi.hpp"

class ChunkedRequest;

class MethodPostCgi : public AMethod
{
    private:
        Config      _config;
        Location    _location;
        std::string _body;
        std::string _path_file;
        std::string _content_type;

        bool        _cgi_init;
        MethodCgi * _cgi;

        std::string _body_received;
        int         _byte_received;
        bool        _file_received;
        int         _byte_send;
        int         _error;

        ChunkedRequest *_chunked_request;

        std::string _tmp_path;

        std::string extractBodyRequest();
        void        receiveData();
        void        writeFile();
        void        writePreparation();

        void        setHeader();
        void        sendHeader();
        void        sendBody();
        void        setChunkedRequest(ChunkedRequest *);
        void        writeProcessedDataChunked();
        bool        maxBodySizeIsReached();


    public:
        MethodPostCgi( int fd, std::string path, std::string request_received, AField &field,
            Config config, Location location, std::string content_type );
        ~MethodPostCgi();
        
        void init();
        void exec();
        void getBody();
};

#endif