#ifndef METHODGET_HPP
# define METHODGET_HPP

#include "AMethod.hpp"

class MethodGet : public AMethod
{
    private:

    void sendHeader();
    void sendBody();
    void openFd();

    int _byte_send;
    pollfd * _s_pollfd;

    public :
        MethodGet(int fd, std::string path, std::string header, AField & field);
        ~MethodGet();

        void init();
        void exec();
};

#endif