#include <ftw.h>
 #include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

void delete_f(const char *path)
{
    nftw(path, remove_f, 1024, FTW_DEPTH); 
}