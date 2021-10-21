#include "../includes/utils.hpp"

bool check_if_file_exist(std::string path)
{
    struct stat sb;
    if (stat(path.c_str(), &sb) < 0)
        return (FALSE);
    return (TRUE);
}