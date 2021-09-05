#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

#include <stddef.h>
#include <string>

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
        int             _method;

    public :
        ResponseHTTP();
        ResponseHTTP(const char *, int);
        ~ResponseHTTP();

        void    send();

        void    resetByteSend();
        bool    getFinished() const;
        void    setPathFile(std::string path);
        void    setFdToAnswer(int);
        void    setFinished(bool);
        void    setInProgress(bool);
        void    setPageAutoIndex();
        void    setAutoIndex(bool);
        bool    getInProgress() const;

        std::string const & getPageAutoIndex() const;

        std::string getPath() const;
        ResponseHTTP& operator=(const ResponseHTTP& other);
};
#endif
