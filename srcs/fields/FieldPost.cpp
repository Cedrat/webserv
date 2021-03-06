#include "../includes/utils.hpp"
#include "FieldPost.hpp"
#include "../methods/Erreur.hpp"
#include "../methods/MethodPost.hpp"
#include "../methods/MethodUpload.hpp"
 
FieldPost::FieldPost(std::string str_request, RequestInProgress data_request, pollfd & s_pollfd) : AField(str_request, data_request, s_pollfd)
{
    fillField();
}

FieldPost::~FieldPost()
{

}

std::string const & FieldPost::getTransfertEncoding() const
{
    return (_transfert_encoding);
}

void FieldPost::setTransfertEncoding(std::string const transfert_encoding)
{
    _transfert_encoding = transfert_encoding;
}

std::string const & FieldPost::getStrContentLength() const 
{
    return (_str_content_length);
}

int const & FieldPost::getContentLength() const 
{
    return (_content_length);
}

std::string  const & FieldPost::getContentType() const 
{
    return (_content_type);
}

void FieldPost::setContentLength(std::string const & length)
{
    _str_content_length = length;
}

void FieldPost::fillField()
{
    std::string copy_str_request = _str_request;
    copy_str_request.erase(_str_request.find("\r\n\r\n"), _str_request.size() -1);
    std::vector<std::string> splitted_request = split_string(copy_str_request, "\n");
    std::vector<std::string> splitted_line;

    _method = split_string(splitted_request[0], " ")[0];
    _path   = decoding_http_string(split_string(splitted_request[0], " ")[1]);
    for (size_t i = 1; i < splitted_request.size(); i++)
    {
        splitted_line = split_string(splitted_request[i], ":");
        if (str_to_lower(splitted_line[0]) == "host")
        {
            trim_field(splitted_line[1]);
            _host_name = splitted_line[1];
        }
        else if (str_to_lower(splitted_line[0]) == "content-length")
        {
            trim_field(splitted_line[1]);
            _str_content_length = splitted_line[1];
        }
         else if (str_to_lower(splitted_line[0]) == "transfer-encoding")
        {
            trim_field(splitted_line[1]);
            _transfert_encoding = splitted_line[1].c_str();
        }
        else if (str_to_lower(splitted_line[0]) == "content-type")
        {
            trim_field(splitted_line[1]);
            _content_type = splitted_line[1].c_str();
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
        return (createErrorMethod(config, location));
    }
    if (methodNotAuthorized(location))
    {
        _error = METHOD_NOT_ALLOWED;
        return (createErrorMethod(config, location));
    }
    _final_path = construct_path_post(getPath(), location);
    if (isCgiPath(_path, location.getCgiExtension()))
    {
        _final_path = createRandomFileName(PATH_TMP);
    }
    else if (location.getUploadFolder().empty() == FALSE)
    {
        _final_path = createPathUploadFolder(location.getUploadFolder());
    }
    checkValidPath(location);
    if (_error != OK)
    {
        return (createErrorMethod(config, location));
    }
    verifyRedirect(location);
    if (_error == MOVED_PERMANENTLY)
    {
        return (createRedirMethod(config, location));
    }
    checkBodySize(config);
    if (_error != OK)
    {
        return (createErrorMethod(config, location));
    }
    if (isCgiPath(_path, location.getCgiExtension()) && location.getCgiExtension() != "0" && location.getCgiBinary() != "0")
	{
		return (createCgiMethod(config, location));
    }
	
    if (location.getUploadFolder().empty() == FALSE && _content_type.find("multipart/form-data;") == 0)
    {
        return (new MethodUpload(_data_request.getFd(), _final_path, _str_request, *this ));
    }
    return (new MethodPost(_data_request.getFd(), _final_path, _str_request, *this ));
}

void FieldPost::verifyMissingData()
{
    if (_host_name == "")
        _error = BAD_REQUEST; 
}

void FieldPost::verifyData()
{
    if (_str_content_length.empty())
    {
        if (_transfert_encoding.find("chunked") == std::string::npos)
            _error = BAD_REQUEST;
    }
    if (str_is_not_number(_str_content_length))
    {
        _error = BAD_REQUEST;
    }
    else
    {
        _content_length = atoi(_str_content_length.c_str());
    }
}
AMethod *FieldPost::createErrorMethod(Config config, Location location)
{
    std::string header;
    std::string path_error = config.getPathError(_error);
    
    header = "HTTP/1.1 " + get_string_error(_error);
    header += "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n";
    if(_error == METHOD_NOT_ALLOWED)
    {
        header+= fields_allowed(location);
    }
    header +=  date_string() + "\n\n";

    AMethod *method = new Erreur(_data_request.getFd(), path_error, header, *this);
    return (method);
}

void FieldPost::checkValidPath(Location location)
{
        if (check_valid_path(_path) == FALSE)
    {   
        std::cout << 1 << std::endl;
        _error = BAD_REQUEST;
    }
    if (check_if_file_exist(_final_path) && is_folder(_final_path.c_str()))
    {
        if (_content_type.find("multipart/form-data;") != 0 || location.getUploadFolder().empty())
        {
            _error = BAD_REQUEST;
        }
    }
    if (check_if_file_exist(remove_chars_after_the_last_token(_final_path, '/')) == FALSE)
    {
        std::cout << 3 << std::endl;
        _error = BAD_REQUEST;
    }   

}

std::string FieldPost::createPathUploadFolder(std::string upload_folder)
{
    std::vector<std::string> splitted_path = split_string(_final_path, "/");
    std::string temp_path;

    trim(upload_folder, '.');
    std::cout << _content_type << std::endl;
    if (_final_path.find(upload_folder) == std::string::npos)
        temp_path = "." + upload_folder + splitted_path[splitted_path.size() - 1];
    else if (_content_type.find("multipart/form-data;") == 0)
        temp_path = "." + upload_folder;
    else 
        temp_path = "." + upload_folder + splitted_path[splitted_path.size() - 1];
    std::cout << temp_path << std::endl;
    return (temp_path);
}

void FieldPost::checkBodySize(Config const &config)
{
    if (atoi(_str_content_length.c_str()) > config.getMaxBodySize())
        _error = ENTITY_TOO_LARGE;
}

AMethod *FieldPost::createCgiMethod(Config config, Location location)
{
    AMethod * method = new MethodPostCgi(_data_request.getFd(), _final_path, _str_request, 
            *this, config, location, _content_type);
    return method;
}