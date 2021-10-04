#include "AField.hpp"
#include "FieldGet.hpp"
#include "MethodGet.hpp"
#include "Erreur.hpp"
#include "MethodAi.hpp"
#include "MethodCgi.hpp"

FieldGet::FieldGet(std::string str_request, RequestInProgress data_request, pollfd &s_pollfd) : AField(str_request , data_request, s_pollfd)
{
	std::cout << "Created" << std::endl;
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
		_path   = split_string(splitted_request[0], " ")[1];
	if (_path.empty()==FALSE)
		decompose_path(_path, _query);
	for (size_t i = 1; i < splitted_request.size(); i++)
	{
		splitted_line = split_string(splitted_request[i], ":");
		//std::cout << "Splitted request " << i << " " << splitted_request[i] << std::endl;
		//std::cout << "splitted line [0] " << str_to_lower(splitted_line[0]) << std::endl;
		if (str_to_lower(splitted_line[0]) == "host")
		{
			trim_field(splitted_line[1]);
			_host_name = splitted_line[1];
			//std::cout << "host_name in field get " << _host_name << std::endl;
		}
	} 
}

void FieldGet::verifyMissingData()
{
	std::cout << "host_name : " << _host_name << std::endl;
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
		return (createErrorMethod(config));
	}
	verifyRedirect(location);
	if (_error == MOVED_PERMANENTLY)
		return (createRedirMethod(config, location));
	if (check_if_file_exist(_final_path) == FALSE)
	{
		_error = NOT_FOUND;
		return (createErrorMethod(config));
	}	
	if (isAIPath(_final_path, location))
	{
		return(createAiMethod());
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
	
	std::cout << header << std::endl;
	std::cout << "final path " << _final_path << std::endl;

	AMethod * method = new MethodGet(_data_request.getFd(), _final_path, header, *this);
	return(method);
}

AMethod *FieldGet::createErrorMethod(Config config)
{
	std::string header;
	std::string path_error = config.getPathError(_error);
	
	header = "HTTP/1.1 " + get_string_error(_error);
	header += "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n";
	header +=  date_string() + "\n\n";

	std::cout << "ERROR HEADER : " << header << std::endl;

	AMethod *method = new Erreur(_data_request.getFd(), path_error, header, *this);
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
	//std::cout << _method << std::endl;

	AMethod *method = new MethodCgi(_data_request.getFd(), _final_path, "", config, location, "", _method, *this); //fd, path to file, header, config, location, body
	return (method);
}