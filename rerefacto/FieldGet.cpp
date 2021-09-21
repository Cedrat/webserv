#include "AField.hpp"
#include "FieldGet.hpp"

FieldGet::FieldGet(std::string str_request, RequestInProgress data_request) : AField(str_request , data_request)
{
    std::cout << "Created" << std::endl;
    fillField();
    //verifyMissingData();
    //verifyData();
	//verifyRedirect();
	//verifyAI();
	//constructFinalPath();
	//Si pas AI/redirect, verifier si le fichier existe.
}

FieldGet::~FieldGet()
{

}

void FieldGet::fillField()
{
    std::vector<std::string> splitted_request = split_string(_str_request, "\n");
    std::vector<std::string> splitted_line;

    _method = split_string(splitted_request[0], " ")[0];
    _path   = split_string(splitted_request[0], " ")[1];
    for (size_t i = 1; i < splitted_request.size(); i++)
    {
        splitted_line = split_string(splitted_request[i], ":");
        if (str_to_lower(splitted_line[0]) == "host:")
        {
            trim(splitted_line[1], ' ');
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
	// if (_error != OK)
	// 	return (createErrorMethod());
	Config config = _data_request.getConfigs()[find_index_best_config(_data_request.getConfigs(), getHostName(), _data_request.getPort(), _data_request.getHost())];
    Location location = find_best_location(getPath(), config);     

	verifyRedirect(location);
	// if (_error == MOVED_PERMANENTLY)
	// 	return (createRedirMethod());
	_final_path = construct_path(getPath(), location);
	if (check_if_file_exist(_final_path))
        {}
	//	return (createErrorMethod(););
	// if auto_index on and path give a folder
    if (isAIPath(_final_path, location))
		{}//return(createAIMethod);
	//return (createGetMethod());
}