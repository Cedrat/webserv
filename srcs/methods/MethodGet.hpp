#ifndef METHODGET_HPP
# define METHODGET_HPP

#include "AMethod.hpp"

class MethodGet : public AMethod
{
    private:
    int _byte_send;

    public :
        MethodGet(int fd, std::string path, std::string header, AField & field);
        virtual ~MethodGet();

        void init();
        void exec();
};

#endif