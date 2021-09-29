#ifndef METHODPOST_HPP
# define METHODPOST_HPP


#include "AMethod.hpp"

class MethodPost: public AMethod
{
    private :
        std::string _body_received;

        std::string extractBodyRequest();

    public :
    MethodPost(int fd, std::string path, std::string request_received, AField &field);
    ~MethodPost();

    void init();
    void exec();
};

#endif