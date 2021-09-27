#include "../includes/utils.hpp"

bool is_folder(const char * path)
{
    struct stat sb;

    stat(path, &sb);

    if (S_ISDIR(sb.st_mode))
        return (TRUE);
    return (FALSE);
}