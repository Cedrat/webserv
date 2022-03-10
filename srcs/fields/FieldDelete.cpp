#include "AField.hpp"
#include "FieldDelete.hpp"
#include "../methods/MethodGet.hpp"
#include "../methods/Erreur.hpp"
#include "../methods/MethodAi.hpp"
#include "../includes/utils.hpp"

FieldDelete::FieldDelete(std::string str_request, RequestInProgress data_request, pollfd & s_pollfd) : AField(str_request , data_request, s_pollfd)
{
    fillField();
}

FieldDelete::~FieldDelete()
{

}

void FieldDelete::fillField()
{
    std::vector<std::string> splitted_request = split_string(_str_request, "\n");
    std::vector<std::string> splitted_line;

    _method = split_string(splitted_request[0], " ")[0];
    _path   = decoding_http_string(split_string(splitted_request[0], " ")[1]);
    for (size_t i = 1; i < splitted_request.size(); i++)
    {
        splitted_line = split_string(splitted_request[i], ":");
        if (str_to_lower(splitted_line[0]) == "host")
        {
            trim(splitted_line[1], ' ');
            _host_name = splitted_line[1];
        }
    } 
}

void FieldDelete::verifyMissingData()
{
    if (_host_name == "")
        _error = BAD_REQUEST;
}

void FieldDelete::verifyData()
{
    if (check_valid_path(_path) == FALSE)
        _error = BAD_REQUEST;
}

void FieldDelete::verifyRedirect(Location const &location)
{
    if (location.getRedirect().empty() == FALSE)
        _error = MOVED_PERMANENTLY;
}

bool FieldDelete::isAIPath(std::string path, Location const &location)
{
    if (is_folder(path.c_str()) && location.getAutoIndex())
    {
        return (TRUE);
    }
    return (FALSE);
}

std::string construct_delete_path(std::string path, Location location)
{
    path = create_alias(path, location);
    if (check_if_file_exist(path) && is_folder(path.c_str()))
    {
        if (path.rfind("/") != path.size() - 1)
            path += "/";
    }
    return (path);
}

AMethod *FieldDelete::getAMethod()
{
	verifyMissingData();
	verifyData();
	Config config = _data_request.getConfigs()[find_index_best_config(_data_request.getConfigs(), getHostName(), _data_request.getPort(), _data_request.getHost())];
    Location location = find_best_location(getPath(), config);     
	_final_path = construct_delete_path(getPath(), location);
	if (_error != OK)
    {
		return (createErrorMethod(config, location));
    }
	verifyRedirect(location);
	if (_error == MOVED_PERMANENTLY)
		return (createRedirMethod(config, location));
    if (methodNotAuthorized(location))
    {
        _error = METHOD_NOT_ALLOWED;
        return (createErrorMethod(config, location));
    }
	if (check_if_file_exist(_final_path) == FALSE)
    {
        _error = NOT_FOUND;
	    return (createErrorMethod(config, location));
    }
    delFile();
	return (createErrorMethod(config, location));
}

AMethod *FieldDelete::createErrorMethod(Config config, Location location)
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
    

    AMethod *method = new Erreur(_data_request.getFd(), path_error, header,  *this);
    return (method);
}

void FieldDelete::delFile()
{
    if (delete_f(_final_path.c_str()) > 0)
        _error = NO_CONTENT;
    else
        _error = NOT_FOUND;
};