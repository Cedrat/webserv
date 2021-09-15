#include "ConfigParser.hpp"

void add_config(const char *path_conf)
{

    (void) path_conf;
    char file[] = "./step.conf";
    try
    {
        ConfigParser cp(file);
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }

}