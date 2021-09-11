#include "../includes/utils.hpp"
#include "Location.hpp"


std::string create_alias(std::string path, Location location);

std::string construct_path(std::string path, Location location)
{
    path = create_alias(path, location);
    std::cout << location.getAutoIndex() << std::endl;
    if (path.rfind("/") == path.size() -1 && location.getAutoIndex() == FALSE)
        path += location.getDefaultFile();
    std::cout << "Construct_path " << path << std::endl;
    return (path);
}

std::string create_alias(std::string path, Location location)
{
    std::string alias = location.getRoot();
    std::string path_location = location.getLocation();

    path.replace(0, path_location.size() - 1, alias);

    return (path);
}