#include "../includes/utils.hpp"

bool isCgiPath(std::string path, std::string extension)
{
    size_t pos = 0;

    if ((pos = path.find(extension)) != std::string::npos)
    {
        if (!path[pos + extension.size()])
            return true;
    }
    return false;
}