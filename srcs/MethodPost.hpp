#ifndef METHODPOST_HPP
# define METHODPOST_HPP


#include "AMethod.hpp"

class MethodPost: public AMethod
{
    private :
        std::string _body_received;
        int         _byte_received;
        bool        _file_received;

        std::string extractBodyRequest();
        void        receiveData();
        void        writeFile();
        void        writePreparation();

        void        sendHeader();
        void        sendBody();

    public :
        MethodPost(int fd, std::string path, std::string request_received, AField &field);
        ~MethodPost();

    void init();
    void exec();
};

#endif