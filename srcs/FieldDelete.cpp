#include "AField.hpp"
#include "FieldDelete.hpp"
#include "MethodGet.hpp"
#include "Erreur.hpp"
#include "MethodAi.hpp"

FieldDelete::FieldDelete(std::string str_request, RequestInProgress data_request, pollfd & s_pollfd) : AField(str_request , data_request, s_pollfd)
{
    std::cout << "Created" << std::endl;
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
            std::cout << "host_name in field get " << _host_name << std::endl;
        }
    } 
}

void FieldDelete::verifyMissingData()
{
    std::cout << "host_name : " << _host_name << std::endl;
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

AMethod *FieldDelete::getAMethod()
{
	verifyMissingData();
	verifyData();
	Config config = _data_request.getConfigs()[find_index_best_config(_data_request.getConfigs(), getHostName(), _data_request.getPort(), _data_request.getHost())];
    Location location = find_best_location(getPath(), config);     
	_final_path = construct_path(getPath(), location);
	if (_error != OK)
    {
		return (createErrorMethod(config));
    }
	verifyRedirect(location);
	if (_error == MOVED_PERMANENTLY)
		return (createRedirMethod(config, location));
    if (methodNotAuthorized(location))
    {
        _error = METHOD_NOT_ALLOWED;
        return (createErrorMethod(config));
    }
	if (check_if_file_exist(_final_path) == FALSE)
    {
        _error = NOT_FOUND;
	    return (createErrorMethod(config));
    }
    delFile();
	return (createErrorMethod(config));
}

AMethod *FieldDelete::createErrorMethod(Config config)
{
    std::string header;
    std::string path_error = config.getPathError(_error);
    
    header = "HTTP/1.1 " + get_string_error(_error);
    header += "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n";
    header +=  date_string() + "\n\n";

    std::cout << "ERROR HEADER : " << header << std::endl;

    AMethod *method = new Erreur(_data_request.getFd(), path_error, header,  *this);
    return (method);
}

AMethod *FieldDelete::createRedirMethod(Config config, Location location)
{
    std::string path_error = config.getPathError(_error);

    std::string header = "HTTP/1.1 " + get_string_error(_error);
    header += "\nLocation: " + redir_path(_path, location.getRedirect(), location.getLocation());    
    header += "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n";
    header +=  date_string() + "\n\n";

    AMethod *method = new Erreur(_data_request.getFd(), path_error, header, *this);
    return (method);
}

void FieldDelete::delFile()
{
    if (delete_f(_final_path.c_str()) > 0)
        _error = NO_CONTENT;
    else
        _error = NOT_FOUND;
};