#include "../includes/utils.hpp"

bool check_if_file_exist(std::string path)
{
    struct stat sb;
    std::cout << "path in file exist << " << path << std::endl;
    if (stat(path.c_str(), &sb) < 0)
        return (FALSE);
    return (TRUE);
}