#include "../includes/utils.hpp"

std::string create_path(Request request, Config config)
{
    std::string path;
    Location location = request.findBestLocation(config);
    
    path = location.getRoot() + request.getPathFileRequest();   

    return (path); 
}