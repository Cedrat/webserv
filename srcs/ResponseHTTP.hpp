#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

#include <stddef.h>
#include <string>
#include <vector>

class ResponseHTTP
{
    private :
        std::string     _path_file;
        std::string     _ai_file;
        size_t          _byte_send;
        bool            _in_progress;
        bool            _finished; // implement response HTTP
        int             _fd_to_answer;
        bool            _ai;


        std::string     _path_redirect;

    public :
        ResponseHTTP();
        ResponseHTTP(const char *, int);
        ~ResponseHTTP();

        bool    send();

        void    resetByteSend();
        void    setPathFile(std::string path);
        void    setFdToAnswer(int);
        void    setFinished(bool);
        void    setInProgress(bool);
        void    setPageAutoIndex();
        void    setAutoIndex(bool);
        
        bool    getFinished() const;
        bool    getInProgress() const;

        void    reset();

        std::string const & getPageAutoIndex() const;

        std::string getPath() const;
        ResponseHTTP& operator=(const ResponseHTTP& other);
};
#endif
