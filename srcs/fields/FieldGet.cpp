#include "AField.hpp"
#include "FieldGet.hpp"
#include "../methods/MethodGet.hpp"
#include "../methods/Erreur.hpp"
#include "../methods/MethodAi.hpp"
#include "../methods/MethodCgi.hpp"

FieldGet::FieldGet(std::string str_request, RequestInProgress data_request, pollfd &s_pollfd) : AField(str_request , data_request, s_pollfd)
{
	fillField();
}

FieldGet::~FieldGet()
{

}

void FieldGet::fillField()
{
	std::vector<std::string> splitted_request = split_string(_str_request, "\n");
	std::vector<std::string> splitted_line;

	splitted_line = split_string(splitted_request[0], " ");
	_method = split_string(splitted_request[0], " ")[0];
	if (splitted_line.size() > 1)
		_path   = decoding_http_string(split_string(splitted_request[0], " ")[1]);
	if (_path.empty()==FALSE)
		decompose_path(_path, _query);
	for (size_t i = 1; i < splitted_request.size(); i++)
	{
		splitted_line = split_string(splitted_request[i], ":");
		if (str_to_lower(splitted_line[0]) == "host")
		{
			trim_field(splitted_line[1]);
			_host_name = splitted_line[1];
		}
	} 
}

void FieldGet::verifyMissingData()
{
	if (_host_name == "")
		_error = BAD_REQUEST;
}

void FieldGet::verifyData()
{
	if (check_valid_path(_path) == FALSE)
		_error = BAD_REQUEST;
}

void FieldGet::verifyRedirect(Location const &location)
{
	if (location.getRedirect().empty() == FALSE)
		_error = MOVED_PERMANENTLY;
}

bool FieldGet::isAIPath(std::string path, Location const &location)
{
	if (is_folder(path.c_str()) && location.getAutoIndex())
	{
		return (TRUE);
	}
	return (FALSE);
}

AMethod *FieldGet::getAMethod()
{
	verifyMissingData();
	verifyData();
	Config config = _data_request.getConfigs()[find_index_best_config(_data_request.getConfigs(), getHostName(), _data_request.getPort(), _data_request.getHost())];
	Location location = find_best_location(getPath(), config);     
	_final_path = construct_path(getPath(), location);
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
	if (isAIPath(_final_path, location) && check_if_file_exist(_final_path) && is_folder(_final_path.c_str()))
	{
		return(createAiMethod());
	}
	if (check_if_file_exist(_final_path) == FALSE || is_folder(_final_path.c_str()))
	{
		_error = NOT_FOUND;
		return (createErrorMethod(config, location));
	}	
	if (check_authorization(_final_path.c_str()) == FALSE)
	{
		_error = FORBIDDEN;
		return (createErrorMethod(config, location));
	}
	if (isCgiPath(_path, location.getCgiExtension()) && location.getCgiExtension() != "0" && location.getCgiBinary() != "0")
	{
		return(createCgiMethod(config, location));
	}
	return (createGetMethod());
}

AMethod *FieldGet::createGetMethod()
{	
	std::string header;

	header = "HTTP/1.1 200 OK\nContent-Length: " + int_to_string(get_file_size(_final_path)) + "\n";
	header +=  date_string() + "\n\n";
	
	AMethod * method = new MethodGet(_data_request.getFd(), _final_path, header, *this);
	return(method);
}

AMethod *FieldGet::createErrorMethod(Config config, Location location)
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

AMethod *FieldGet::createAiMethod()
{
	std::string ai_file = create_ai_page(_path.c_str(),_final_path.c_str());

	std::string header = "HTTP/1.1 " + get_string_error(_error);
	header += "\nContent-Length: " + int_to_string(ai_file.size()) + "\n";
	header +=  date_string() + "\n\n";

	AMethod *method = new MethodAi(_data_request.getFd(), ai_file, header,  *this);
	return (method);
}

AMethod *FieldGet::createCgiMethod(Config config, Location location)
{
	AMethod *method = new MethodCgi(_data_request.getFd(), _final_path, "", config, location, "", _method, *this, ""); //fd, path to file, header, config, location, body
	return (method);
}