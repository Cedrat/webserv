#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "define.hpp"
#include <string>
#include "ResponseHTTP.hpp"

class Config;
class Request
{
    private :
        fd _fd;
        size_t _port;
        std::string _request;
        bool _request_completed;
        int     _error;

        std::string _method;
        std::string _path;
        std::string _host_name;
        int         _content_length;
        bool        _in_progress;

        ResponseHTTP _response;
    
    public :
        Request(fd, size_t);
        ~Request();

        fd const &          getFd() const;
        size_t const &      getPort() const;
        bool const &        getRequestCompleted() const;
        int const &         getError() const;
        std::string const & getMethod() const;
        std::string const & getPath() const;
        std::string const & getHostName() const;
        int const &         getContentLength() const;
        std::string const & getRequest() const;        
        bool const &        getInProgress() const;

        void            setInProgress(bool); 
        void            setError(int error); 
        void            setMethod(std::string method);
        void            setPath(std::string path);
        void            setHostName(std::string host_name);
        void            setContentLength(int content_length);
       
       
        void                    setResponseHTTP(Config config);
        ResponseHTTP const &    getResponseHTTP() const;
        
        
        void        addRequest(std::string buffer);
        void        checkSyntaxRequest();

        void        receiveData();
        void        resetRequest();
        void        send();

};

#endif