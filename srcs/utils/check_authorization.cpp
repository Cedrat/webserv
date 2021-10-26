#include "../includes/utils.hpp"

bool check_authorization(const char * path)
{
    struct stat stat_right;

    stat(path, &stat_right);
    if(stat_right.st_mode & S_IRUSR)
        return (TRUE);
    return (FALSE);
}