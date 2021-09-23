#include "../includes/utils.hpp"
#include "FieldPost.hpp"
#include "Erreur.hpp"
 
FieldPost::FieldPost(std::string str_request, RequestInProgress data_request) : AField(str_request, data_request)
{
    fillField();
}

FieldPost::~FieldPost()
{

}

std::string const & FieldPost::getTransfertEncoding()
{
    return (_transfert_encoding);
}

void FieldPost::setTransfertEncoding(std::string const transfert_encoding)
{
    _transfert_encoding = transfert_encoding;
}

std::string const & FieldPost::getContentLength()
{
    return (_content_length);
}

void FieldPost::setContentLength(std::string const & length)
{
    _content_length = length;
}

void FieldPost::fillField()
{
    std::vector<std::string> splitted_request = split_string(_str_request, "\n");
    std::vector<std::string> splitted_line;

    _method = split_string(splitted_request[0], " ")[0];
    _path   = split_string(splitted_request[0], " ")[1];
    for (size_t i = 1; i < splitted_request.size(); i++)
    {
        splitted_line = split_string(splitted_request[i], ":");
        std::cout << "Splitted request " << i << " " << splitted_request[i] << std::endl;
        std::cout << "splitted line [0] " << str_to_lower(splitted_line[0]) << std::endl;
        if (str_to_lower(splitted_line[0]) == "host")
        {
            trim(splitted_line[1], ' ');
            _host_name = splitted_line[1];
            std::cout << "Post host name : " << _host_name << std::endl;
        }
        else if (str_to_lower(splitted_line[0]) == "content-length")
        {
            trim(splitted_line[1], ' ');
            _content_length = splitted_line[1];
            std::cout << "Post content length : " << _content_length << std::endl;
        }
         else if (str_to_lower(splitted_line[0]) == "transfer-encoding")
        {
            trim(splitted_line[1], ' ');
            _host_name = atoi(splitted_line[1].c_str());
            std::cout << "Post transfer-encoding : " << _transfert_encoding << std::endl;
        }
    } 
} 

void FieldPost::verifyMissingData()
{
    std::cout << "host_name : " << _host_name << std::endl;
    if (_host_name == "")
        _error = BAD_REQUEST;
}


AMethod *FieldPost::getAMethod() 
{
    return (new Erreur(_data_request.getFd(), _path, _header));
}