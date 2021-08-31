#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "define.hpp"
#include <string>

class ResponseHTTP;
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

        void            setError(int error); 
        void            setMethod(std::string method);
        void            setPath(std::string path);
        void            setHostName(std::string host_name);
        void            setContentLength(int content_length);
        
        
        void        addRequest(std::string buffer);
        void        checkSyntaxRequest();

        void        receiveData();
        void        resetRequest();

};

#endif