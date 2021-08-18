#include "../includes/utils.hpp"
#include <ftw.h>
 #include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int write_f(const char * path, const struct stat *sb, int tflag, struct FTW* ftw_struct)
{
    (void)ftw_struct;
    (void)tflag;
    std::cout << path << std::endl;
    return (0);
}
void auto_index_html_page(const char * path)
{
    std::cout << nftw(path, write_f, 1024, FTW_DEPTH) <<std::endl;
 
}

int main()
{
    auto_index_html_page("..");
}