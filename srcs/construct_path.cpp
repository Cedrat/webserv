#include "../includes/utils.hpp"
#include "Location.hpp"


std::string create_alias(std::string path, Location location);

std::string construct_path(std::string path, Location location)
{
    //std::cout << "path brut de coffre : " << path << std::endl;
    path = create_alias(path, location);
        //std::cout << "path brut de coffre : " << path << std::endl;

    std::cout << location.getAutoIndex() << std::endl;
    if (check_if_file_exist(path) && is_folder(path.c_str()))
    {
        std::cout << "Folder" << std::endl;
        if (path.rfind("/") != path.size() - 1)
            path += "/";
    }
    if (path.rfind("/") == path.size() - 1 && location.getAutoIndex() == FALSE)
        path += location.getDefaultFile();
    //std::cout << "Construct_path " << path << std::endl;
    return (path);
}

std::string create_alias(std::string path, Location location)
{
    std::string alias = location.getRoot();
    std::string path_location = location.getLocation();
    if (path_location.rfind("/") == path_location.size() - 1)
        path.replace(0, path_location.size() -1 , alias);
    else
        path.replace(0, path_location.size() , alias);

    return (path);
}