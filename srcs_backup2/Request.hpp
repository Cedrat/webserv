#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "define.hpp"
#include <string>
#include "ResponseHTTP.hpp"

class Config;

//STATUS DEFINE
# define PARSING_REQUEST 1
# define POST_REQUEST 2
# define POST_CGI_REQUEST 3
# define SEND_HEADER 4
# define SEND_ERROR_HEADER 5
# define SEND_BODY 6
# define REQUEST_ENDING 7

class Request
{
    private :
        fd _fd;
        int     _host;
        size_t _port;
        std::string _request;
        bool _request_completed;
        std::string _redirect_path;
        int     _error;

        std::string _method;
        std::string _path;
        std::string _host_name;
        int         _str_content_length;
        bool        _in_progress;
        int         _status;
        ResponseHTTP _response;
    
    public :
        Request(fd, int, size_t);
        ~Request();

        fd const &          getFd() const;
        size_t const &      getPort() const;
        bool const &        getRequestCompleted() const;
        int const &         getError() const;
        std::string const & getMethod() const;
        std::string const & getPath() const;
        std::string const & getHostName() const;
        int const &         getStrContentLength() const;
        std::string const & getRequest() const;        
        bool const &        getInProgress() const;
        int const &         getStatus() const;
        int const &         getHost() const;
        std::string const & getRedirectPath() const;

        void            setInProgress(bool); 
        void            setError(int error);
        void            setStatus(const int status);
        void            setMethod(std::string method);
        void            setPath(std::string path);
        void            setHostName(std::string host_name);
        void            setContentLength(int content_length);
        void            setHost(const int host);
        void            setRedirectPath(std::string const redirect_path);
       
       
        void                    setResponseHTTPGet(Config config);
        void                    setResponseHTTPDelete(Config config);
        void                    setResponseHTTPError(Config config);
        void                    setResponseHTTPPost(Config config);
        ResponseHTTP const &    getResponseHTTP() const;
        
        
        void        addRequest(std::string buffer);
        void        checkSyntaxRequest();

        void        receiveData(std::vector<Config> & configs);
        void        resetRequest();
        void        send();

};

#endif