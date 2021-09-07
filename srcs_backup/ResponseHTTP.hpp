#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

#include <stddef.h>
#include <string>

class ResponseHTTP
{
    private :
        std::string    _path_file;
        size_t          _byte_send;
        bool            _finished;
        int              _fd_to_answer;

    public :
        ResponseHTTP();
        ResponseHTTP(const char *, int);
        ~ResponseHTTP();

        void    send();

        void    resetByteSend();
        bool    getFinished();
        void    setPathFile(const char* path);
        void    setFdToAnswer(int);
        void    setFinished(bool);
        
        std::string getPath();
        ResponseHTTP& operator=(const ResponseHTTP& other);
};
#endif
