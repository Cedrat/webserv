#include "../includes/utils.hpp"

size_t get_file_size(std::string path)
{
    struct stat sb;

    stat(path.c_str(), &sb);
    return (sb.st_size); 
}