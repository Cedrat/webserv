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
            trim_field(splitted_line[1]);
            _host_name = splitted_line[1];
            std::cout << "Post host name : " << _host_name << std::endl;
        }
        else if (str_to_lower(splitted_line[0]) == "content-length")
        {
            trim_field(splitted_line[1]);
            _content_length = splitted_line[1];
            std::cout << "Post content length : " << _content_length << std::endl;
        }
         else if (str_to_lower(splitted_line[0]) == "transfer-encoding")
        {
            trim_field(splitted_line[1]);
            _transfert_encoding = splitted_line[1].c_str();
            std::cout << "Post transfer-encoding : " << _transfert_encoding << std::endl;
        }
    } 
} 



AMethod *FieldPost::getAMethod() 
{
    verifyMissingData();
    verifyData();
    Config config = _data_request.getConfigs()[find_index_best_config(_data_request.getConfigs(), getHostName(), _data_request.getPort(), _data_request.getHost())];
    Location location = find_best_location(getPath(), config);
    if (_error != OK)
    {
        return (createErrorMethod(config));
    }
    verifyRedirect(location);
    if (_error == MOVED_PERMANENTLY)
    {
        return (createRedirMethod(config, location));
    }

}

void FieldPost::verifyMissingData()
{
    std::cout << "host_name : " << _host_name << std::endl;
    if (_host_name == "")
        _error = BAD_REQUEST; 
}

void FieldPost::verifyData()
{
    if (_content_length.empty())
    {
        if (_transfert_encoding.find("chunked") == std::string::npos)
            _error = BAD_REQUEST;
    }
    if (str_is_not_number(_content_length))
    {
        _error = BAD_REQUEST;
    }
}
AMethod *FieldPost::createErrorMethod(Config config)
{
    std::string header;
    std::string path_error = config.getPathError(_error);
    
    header = "HTTP/1.1 " + get_string_error(_error);
    header += "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n";
    header +=  date_string() + "\n\n";

    std::cout << "ERROR HEADER : " << header << std::endl;

    AMethod *method = new Erreur(_data_request.getFd(), path_error, header);
    return (method);
}
