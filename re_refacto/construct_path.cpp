#include "../includes/utils.hpp"
#include "Location.hpp"


std::string construct_path(std::string path, Location location)
{
    if (path.rfind("/") == path.size() -1 && location.getAutoIndex() == FALSE)
        path += location.getDefaultFile();
    path = location.getRoot() + path;
    return (path);
}