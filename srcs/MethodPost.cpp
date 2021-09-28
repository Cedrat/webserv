#include "MethodPost.hpp"
#include "AMethod.hpp"
#include "AField.hpp"
#include "FieldPost.hpp"

MethodPost::MethodPost(int fd, std::string path, std::string request_received, AField &field) :
AMethod(fd, path, request_received, field)
{
    std::cout << "Welcome to MethodPost" << std::endl;
}


MethodPost::~MethodPost()
{

}

void MethodPost::init()
{
    _fields.setPollin();
    if (_fields.getTransfertEncoding() == "chunked")
    {
        //do something
    }
    else
    {
        _body_received = extractBodyRequest();
        std::cout << "_body_received " << _body_received << std::endl; 
    }
}

void MethodPost::exec()
{
    
}

std::string MethodPost::extractBodyRequest()
{
    std::string copy_request = _header;

    copy_request.erase(0, _header.find("\r\n\r\n"));
    
    return (copy_request);
}