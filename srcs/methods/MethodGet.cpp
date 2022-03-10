#include "MethodGet.hpp"
#include "../includes/define.hpp"
#include "../fields/AField.hpp"
#include "../includes/CustomException.hpp"


MethodGet::MethodGet(int fd, std::string path, std::string header, AField  &data_field) : AMethod(fd, path, header, data_field)
{

}

MethodGet::~MethodGet()
{
    delete &_fields;
}


void MethodGet::init()
{
   _fields.setPollout(); 
   _byte_send = 0; 
}

void MethodGet::exec()
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