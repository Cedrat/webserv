#ifndef METHODAI_HPP
# define METHODAI_HPP

#include "AMethod.hpp"

class MethodAi : public AMethod
{
    private :
        std::string _ai_file;
        void sendHeader();
        void sendBody();
    
    public :
       MethodAi(int fd, std::string ai_file, std::string header, AField &field);
       ~MethodAi();

       void init();
       void exec(); 
};

#endif