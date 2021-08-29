#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "define.hpp"
#include <string>

class Request
{
    private :
        fd _fd;
        std::string _request;
    
    public :
        Request(fd);
        ~Request();

        fd const &  getFd() const;
        
        
        void        addRequest(std::string buffer);
};

#endif