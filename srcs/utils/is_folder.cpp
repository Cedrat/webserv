#include "../includes/utils.hpp"

bool is_folder(const char * path)
{
    
    if (check_if_file_exist(std::string(path)) == FALSE)
        return (FALSE);
    struct stat sb;

    stat(path, &sb);



    if (S_ISDIR(sb.st_mode))
        return (TRUE);
    return (FALSE);
}