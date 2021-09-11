#include "../includes/utils.hpp"
#include <ftw.h>
 #include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //opendir et readdir
#include <sys/types.h>

#include <dirent.h>


std::string create_ai_page(const char * path)
{
    DIR * dir;
    dir = opendir(path);
    std::string ai_page;
    std::string name_file;
    std::string str_path(path);

    
    ai_page += "<!DOCTYPE html>\n<html>\n";
    struct dirent * info;
    while ((info = readdir(dir)))
    {
        name_file = info->d_name;
        ai_page += "<p><a href=\"" + str_path + "/" + name_file + "\">" + " " + name_file + "</a></p>\n";
    }
    ai_page += "</html>";

    std::cout << "AI PAGE CREATED" << std::endl;

    return (ai_page);
}

