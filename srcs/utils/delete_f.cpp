#include <ftw.h>
 #include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../includes/utils.hpp"

int remove_f(const char * path, const struct stat *sb, int tflag, struct FTW* ftw_struct)
{
    (void)ftw_struct;
    (void)tflag;
    if (S_ISDIR(sb->st_mode) == 0)
    {
        unlink(path);
    }
    else if(S_ISDIR(sb->st_mode) == 1)
    {
        rmdir(path);
    }
    return (0);
}

int delete_f(const char *path)
{
    struct stat sb;

    stat(path, &sb);
    if (S_ISDIR(sb.st_mode) != 1)
    {
        if (unlink(path) < 0)
            return (-1);
    }
    else
        nftw(path, remove_f, 1024, FTW_DEPTH); 
    
    return (1);
}
