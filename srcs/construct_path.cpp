#include "../includes/utils.hpp"
#include "Location.hpp"


std::string create_alias(std::string path, Location location);

std::string construct_path(std::string path, Location location)
{
    path = create_alias(path, location);
    if (check_if_file_exist(path) && is_folder(path.c_str()))
    {
        if (path.rfind("/") != path.size() - 1)
            path += "/";
    }
    if (path.rfind("/") == path.size() - 1 && location.getAutoIndex() == FALSE)
        path += location.getDefaultFile();
    return (path);
}

std::string construct_path_post(std::string path, Location location)
{
    path = create_alias(path, location);

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