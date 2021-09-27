#include "../includes/utils.hpp"

std::string create_path(Request request, Config config)
{
    std::string path;
    Location location = request.findBestLocation(config);
    
    path = location.getRoot() + factorised_path(request.getPathFileRequest());
    if (path[path.size()-1] == '/')
        path += location.getDefaultFile();

    return (path); 
}