#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

#include <stddef.h>

class ResponseHTTP
{
    private :
        const char *    _path_file;
        size_t          _byte_send;
        bool            _finished;
        int              _fd_to_answer;

    public : 
        ResponseHTTP();
        ResponseHTTP(const char *, int);
        ~ResponseHTTP();

        void    send();
        
        bool    getFinished();
        const char * getPath();
        ResponseHTTP& operator=(const ResponseHTTP& other);
};
#endif