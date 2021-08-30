#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "define.hpp"
#include <string>

class Request
{
    private :
        fd _fd;
        size_t _port;
        std::string _request;
        bool _request_completed;
    
    public :
        Request(fd, size_t);
        ~Request();

        fd const &      getFd() const;
        size_t const &  getPort() const;
        bool const &    getRequestCompleted() const;
        
        
        void        addRequest(std::string buffer);

        void        receiveData();
};

#endif