#include "MethodAi.hpp"
#include "../fields/AField.hpp"

MethodAi::MethodAi(int fd, std::string ai_file, std::string header, AField &field) : AMethod(fd, "", header, field) , _ai_file(ai_file)
{

}

MethodAi::~MethodAi()
{
    delete &_fields;
}

void MethodAi::init()
{
   _fields.setPollout(); 
}

void MethodAi::exec()
{
    if (getHeaderSent() == FALSE)
    {
        sendHeader();
        setHeaderSent(TRUE);
    }
    else
    {
        sendBody();
    }
}

void MethodAi::sendBody()
{
    signal(SIGPIPE, SIG_IGN);
    ::send(getFd(), _ai_file.c_str(), _ai_file.size(), 0);
    setIsFinished(TRUE);
}