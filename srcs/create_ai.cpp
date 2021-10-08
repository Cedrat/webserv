#include "../includes/utils.hpp"
#include <ftw.h>
 #include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //opendir et readdir
#include <sys/types.h>

#include <dirent.h>


std::string create_ai_page(const char * short_path, const char * long_path)
{
    DIR * dir;
    dir = opendir(long_path);
    std::string ai_page;
    std::string name_file;
    std::string str_path(short_path);

    
    ai_page += "<!DOCTYPE html>\n<html>\n";
    struct dirent * info;
    while ((info = readdir(dir)))
    {
        name_file = info->d_name;
        if (DT_DIR == info->d_type)
            name_file += "/";
        ai_page += "<p><a href=\"" + str_path +  name_file + "\">" + " " + name_file + "</a></p>\n";
    }
    ai_page += "</html>";

    free(dir);
    return (ai_page);
}

