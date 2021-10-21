#include "../includes/utils.hpp"

void decompose_path(std::string &path, std::string &query)
{
    std::vector<std::string> splitted_path = split_string(path, "?");

    if (splitted_path.size() > 1)
    {
        path =  splitted_path[0];
        query = splitted_path[1];
    }
    else
    {
        path = splitted_path[0];
    }
    
}